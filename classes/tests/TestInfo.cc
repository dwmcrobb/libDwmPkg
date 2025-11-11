//===========================================================================
// @(#) $DwmPath$
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2025
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//  3. The names of the authors and copyright holders may not be used to
//     endorse or promote products derived from this software without
//     specific prior written permission.
//
//  IN NO EVENT SHALL DANIEL W. MCROBB BE LIABLE TO ANY PARTY FOR
//  DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES,
//  INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE,
//  EVEN IF DANIEL W. MCROBB HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//
//  THE SOFTWARE PROVIDED HEREIN IS ON AN "AS IS" BASIS, AND
//  DANIEL W. MCROBB HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
//  UPDATES, ENHANCEMENTS, OR MODIFICATIONS. DANIEL W. MCROBB MAKES NO
//  REPRESENTATIONS AND EXTENDS NO WARRANTIES OF ANY KIND, EITHER
//  IMPLIED OR EXPRESS, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
//  OR THAT THE USE OF THIS SOFTWARE WILL NOT INFRINGE ANY PATENT,
//  TRADEMARK OR OTHER RIGHTS.
//===========================================================================

//---------------------------------------------------------------------------
//!  \file TestInfo.cc
//!  \author Daniel W. McRobb
//!  \brief Unit tests for Dwm::Pkg::Info
//---------------------------------------------------------------------------

#include <cassert>
#include <regex>

#include "DwmPkgInfo.hh"

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
inline constexpr const Dwm::Pkg::Info __attribute__((used))
g_info1(DWM_PKG_TYPE_HDR, "g_info1", DWM_PKG_STATUS_RC, "0.0.1",
         "Daniel McRobb 2025 " DWM_PKG_SYM_JACKOLANTERN DWM_PKG_SYM_GHOST " ",
        "mcplex.net");

        //         "\xE2\x96\xB6 mcplex.net");

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
template <typename T>
static bool ParseAsDwmPkgInfo(const T & info,
                              std::map<std::string,std::string> & result)
{
  bool  rc = false;
  std::string  rgxstr("(" DWM_PKG_TYPE_HDR "|" DWM_PKG_TYPE_LIB "|"
                      DWM_PKG_TYPE_EXE "|" DWM_PKG_TYPE_DOC ")(.+)"
                      "(" DWM_PKG_STATUS_DEV "|" DWM_PKG_STATUS_RC
                      "|" DWM_PKG_STATUS_REL ")(.+)"
                      "(" DWM_PKG_SYM_COPYRIGHT ")(.+)"
                      "(" DWM_PKG_SYM_CALENDAR " )"
                      "((Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)"
                      " [ 123][0-9] [0-9][0-9][0-9][0-9])" " (.*)");
  std::regex  rgx(rgxstr,std::regex::ECMAScript|std::regex::optimize);
  std::smatch sm;
  std::string  v(g_info1.view());
#if 1
  std::cerr << "v size: " << v.size() << " length: " << v.length()
            << " value: '" << v << "'\n";
#endif
  if (std::regex_match(v, sm, rgx)) {
    if (sm.size() == 11) {
      result.clear();
      result["type"] = sm[1].str();
      result["name"] = sm[2].str();
      result["status"] = sm[3].str();
      result["version"] = sm[4].str();
      result["copyright"] = sm[6].str();
      result["date"] = sm[8].str();
      result["other"] = sm[10].str();
      rc = true;
    }
  }
  return rc;
}

template <typename T>
static bool TestParse(const T & info)
{
  std::map<std::string,std::string>  infoMap;
  assert(ParseAsDwmPkgInfo(info, infoMap));
  assert(std::string(info.type()) == infoMap["type"]);
  assert(std::string(info.name()) == infoMap["name"]);
  assert(std::string(info.status()) == infoMap["status"]);
  assert(std::string(info.version()) == infoMap["version"]);
  assert(std::string(info.copyright()) == infoMap["copyright"]);
  assert(std::string(info.datetime()) == infoMap["date"]);
#if 0
  std::cerr << "info.other(): '" << info.other()
            << "', infoMap[\"other\"].size(): '"
            << infoMap["other"].size() << "'\n";
#endif
  assert(std::string(info.other()) == infoMap["other"]);

  return true;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  assert(TestParse(g_info1));
  
  std::map<std::string,std::string>  infoMap;
  if (ParseAsDwmPkgInfo(g_info1, infoMap)) {
    for (const auto & imi : infoMap) {
      std::cout << imi.first << ": \"" << imi.second << "\"\n";
    }
  }
  return 0;
  
  assert(g_info1.type() == DWM_PKG_TYPE_HDR);
  assert(g_info1.name() == "g_info1");
  assert(g_info1.status() == DWM_PKG_STATUS_RC);
  assert(g_info1.version() == "0.0.1");
  assert(g_info1.copyright() == "Daniel McRobb 2025 " DWM_PKG_SYM_JACKOLANTERN DWM_PKG_SYM_GHOST " ");
  assert(g_info1.other() == "\xE2\x96\xB6 mcplex.net");

  constexpr const Dwm::Pkg::Info
    maininfo1(DWM_PKG_TYPE_EXE, "maininfo1", DWM_PKG_STATUS_REL, "1.0.0",
              "Daniel McRobb 2025 " DWM_PKG_SYM_JACKOLANTERN DWM_PKG_SYM_GHOST " ",
              "\xE2\x96\xB6 mcplex.net");
  assert(TestParse(maininfo1));

  assert(maininfo1.type() == DWM_PKG_TYPE_EXE);
  assert(maininfo1.name() == "maininfo1");
  assert(maininfo1.status() == DWM_PKG_STATUS_REL);
  assert(maininfo1.version() == "1.0.0");
  assert(maininfo1.copyright()
         == "Daniel McRobb 2025 " DWM_PKG_SYM_JACKOLANTERN DWM_PKG_SYM_GHOST
         " ");
  assert(maininfo1.other() == "\xE2\x96\xB6 mcplex.net");

  assert(maininfo1 != g_info1);
  assert(g_info1 < maininfo1);
  
  return 0;
}
