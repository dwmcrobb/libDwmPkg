//===========================================================================
// @(#) $DwmPath: dwm/libDwm/trunk/apps/dwmwhat/dwmwhat.cc 8496 $
// @(#) $Id: dwmwhat.cc 8496 2016-05-08 04:55:39Z dwm $
//===========================================================================
//  Copyright (c) Daniel W. McRobb 2016, 2025
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
//!  \file svnwhat.cc
//!  \brief Specialized version of 'what' command
//---------------------------------------------------------------------------

extern "C" {
  #include <sys/mman.h>  // for mmap()
  #include <sys/stat.h>  // for stat()
  #include <fcntl.h>     // for open()
  #include <libgen.h>    // for basename()
  #include <unistd.h>
}

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

#include "DwmPkg.hh"

using namespace std;

#if 0
namespace Crap {
  namespace Pkg {
    const char  *p = "raw pointer to char";
    std::string  junk("junk content");
  }
}
#endif

#define DWMWHAT_COPYRIGHT  "Daniel McRobb 2025 " DWM_PKG_SYM_JACKOLANTERN \
  DWM_PKG_SYM_GHOST " "

//  The lower level map maps the raw string to its JSON string.
//  The top level map just has two keys: "pkgs" and "others"
using PkgMap = map<string,map<string,string>>;

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static std::string MapToJson(std::map<std::string,std::string> & result)
{
  std::string  rc("{");
  std::string  comma;
  
  for (const auto & field : result) {
    rc += comma + " ";
    rc += "\"" + field.first + "\": \"" + field.second + "\"";
    comma = ",";
  }
  rc += " }";
  return rc;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static bool ParseAsDwmPkgInfo(const std::string & v,
                              std::map<std::string,std::string> & result)
{
  bool  rc = false;
  const static std::string  pkgTypes("(" DWM_PKG_TYPE_HDR
                                     "|" DWM_PKG_TYPE_LIB
                                     "|" DWM_PKG_TYPE_EXE
                                     "|" DWM_PKG_TYPE_DOC ")");
  static const std::string  pkgStatus("(" DWM_PKG_STATUS_DEV
                                      "|" DWM_PKG_STATUS_RC
                                      "|" DWM_PKG_STATUS_REL ")");
  static const std::string  pkgDate("((Jan|Feb|Mar|Apr|May|Jun"
                                    "|Jul|Aug|Sep|Oct|Nov|Dec)"
                                    " [ 123][0-9] [0-9][0-9][0-9][0-9])");

  static const std::string  rgxstr("\\@\\(#\\)[ ]+" + pkgTypes + " "
                                   + pkgStatus
                                   + " (.+)"                 // pkg name
                                   + " (.+)"                 // pkg version
                                   + " (" DWM_PKG_SYM_COPYRIGHT ")"
                                   + " (.+) "                // copyright
                                   + pkgDate + " "           // date
                                   + DWM_PKG_SYM_OTHER
                                   // + " (.*)\\0");            // other
                                   + " (.*)");               // other
  static const std::regex
    rgx(rgxstr,std::regex::ECMAScript|std::regex::optimize);
  std::smatch sm;
  if (std::regex_match(v, sm, rgx)) {
    if (sm.size() == 10) {
      result.clear();
      result["type"] = sm[1].str();
      result["status"] = sm[2].str();
      result["name"] = sm[3].str();
      result["version"] = sm[4].str();
      result["copyright"] = sm[6].str();
      result["date"] = sm[7].str();
      result["other"] = sm[9].str();
      rc = true;
    }
  }
  return rc;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static string OtherToJson(const string & other)
{
  return string("{ \"id\": \"" + other + "\" }");
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void GetPkgMap(const vector<string> & vs, PkgMap & pkgMap)
{
  pkgMap.clear();
  map<string,string>  infoMap;
  for (const auto & s : vs) {
    if (ParseAsDwmPkgInfo(s, infoMap)) {
      pkgMap["pkgs"][s] = MapToJson(infoMap);
    }
    else {
      pkgMap["others"][s] = OtherToJson(s);
    }
  }
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void PrintPackages(const PkgMap & pkgMap, bool showJson)
{
  if (! showJson) {
    auto it = pkgMap.find("pkgs");
    if (it != pkgMap.end()) {
      for (const auto & pkg : it->second) {
        std::cout << pkg.first << '\n';
      }
    }
    it = pkgMap.find("others");
    if (it != pkgMap.end()) {
      for (const auto & pkg : it->second) {
        std::cout << pkg.first << '\n';
      }
    }
    return;
  }

  if (! pkgMap.empty()) {
    cout << "{\n";
    
    auto it = pkgMap.find("pkgs");
    if (it != pkgMap.end()) {
      cout << "  \"pkgs\": [";
      string  comma;
      for (const auto pkg : it->second) {
        std::cout << comma << "\n    " << pkg.second;
        comma = ",";
      }
      cout << "\n  ]";
    }
    it = pkgMap.find("others");
    if (it != pkgMap.end()) {
      cout << ",\n  \"others\": [";
      string comma;
      for (const auto other : it->second) {
        cout << comma << "\n    " << other.second;
        comma = ",";
      }
      cout << "\n  ]";
    }
    cout << "\n}\n";
  }
  
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static string GetJson(const vector<string> & vs)
{
  vector<string>  pkgsJson, others;
  map<string,string>  infoMap;
  for (const auto & s : vs) {
    if (ParseAsDwmPkgInfo(s, infoMap)) {
      pkgsJson.push_back(MapToJson(infoMap));
    }
    else {
      others.push_back(s);
    }
  }

  string  rc;
  if (pkgsJson.empty() && others.empty()) {  return rc; }
  rc += "{\n";
  if (! pkgsJson.empty()) {
    rc += "  \"pkgs\": [";
    std::string  comma;
    for (const auto & pkg : pkgsJson) {
      rc += comma + "\n";
      rc += "      " + pkg;
      comma = ",";
    }
    rc += "\n  ]";
  }
  if (! others.empty()) {
    rc += ",  \n  \"others\": [";
    std::string  comma;
    for (const auto & pkg : others) {
      rc += comma + "\n";
      rc += "      { \"id\": \"" + pkg + "\" }";
      comma = ",";
    }
    rc += "  \n  ]";
  }
  rc += "\n}\n";
  return rc;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void LineMatch(const string & line)
{
  static regex lookrgx("^\\@\\(#\\).+", regex::ECMAScript|regex::optimize);
  static regex replregx("^\\@\\(#\\) *", regex::ECMAScript|regex::optimize);
  smatch  lookMatch;
  if (regex_match(line, lookMatch, lookrgx)) {
    string  s(regex_replace(lookMatch[0].str(), replregx, ""));
    if (! s.empty()) {
      cout << s << '\n';
    }
  }
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static pair<char *,size_t> MapFile(const string & filename)
{
  pair<char *,size_t> rc(nullptr,0);
  int  fd = open(filename.c_str(), O_RDONLY);
  if (fd >= 0) {
    struct stat  statbuf;
    if (stat(filename.c_str(), &statbuf) == 0) {
      rc.second = statbuf.st_size;
      rc.first = (char *)mmap(0, statbuf.st_size, PROT_READ, MAP_FILE|MAP_SHARED, fd, 0);
      if (rc.first == MAP_FAILED) {
        rc.first = 0;
        rc.second = 0;
      }
    }
    close(fd);
  }
  return rc;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static vector<string> FindSccsStrings(const char * map, size_t size)
{
  vector<string>     rc;
  size_t             i = 0;
  while (i < (size - 5)) {
    if ((map[i] == '@') && (map[i+1] == '(') && (map[i+2] == '#')
        && (map[i+3] == ')')) {
      string::size_type  startidx = i;
      i += 4;
      while ((map[i] != '\0') && (map[i] != '\n') && (i < size)) {
        ++i;
      }
      if (i < size) {
        rc.push_back(string(&map[startidx], &map[i]));
      }
    }
    else {
      ++i;
    }
  }
  return rc;
}

#if defined(DWM_PKG_CAN_USE_REFLECTION)

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static auto GetPackages()
{
  auto pkgs = Dwm::Pkg::get_packages<^^Dwm>();
  std::ranges::sort(pkgs);
  auto u = std::ranges::unique(pkgs);
  pkgs.erase(u.begin(),u.end());
  return pkgs;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void DumpPackagesJson()
{
  auto pkgs = GetPackages();
  
  if (! pkgs.empty()) {
    cout << "[\n";
    bool  first = true;
    for (auto & pkg : pkgs) {
      if (! first) {
        cout << ",\n  " << pkg.second.second;
      }
      else {
        cout << "  " << pkg.second.second;
        first = false;
      }
    }
    cout << "\n]\n";
  }
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void DumpPackagesPlain()
{
  auto pkgs = GetPackages();
  
  if (! pkgs.empty()) {
    for (auto & pkg : pkgs) {
      cout << pkg.second.first << '\n';
    }
  }
  return;
}

#endif

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void Usage(const char *argv0)
{
  std::cerr << "Usage: " << argv0 << " [-v|-V] [-j] files...\n";
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  bool  showVersion = false, showVerbose = false, showAsJson = false;
  int  optChar;
  while ((optChar = getopt(argc, argv, "jvV")) != -1) {
    switch (optChar) {
      case 'j':
        showAsJson = true;
        break;
      case 'v':
        showVersion = true;
        break;
      case 'V':
        showVersion = true;
        showVerbose = true;
        break;
      default:
        Usage(argv[0]);
        return 1;
        break;
    }
  }

  if (showVersion) {
#if defined(DWM_PKG_CAN_USE_REFLECTION)
    if (showVerbose) {
      DumpPackagesJson();
    }
    else {
      DumpPackagesPlain();
    }
#else
    if (showVerbose) {
      std::cout << "[\n"
                << "  " << Dwm::Pkg::info.as_json() << '\n'
                << "]\n";
    }
    else {
      std::cout << Dwm::Pkg::info.view() << '\n';
    }
#endif
    return 0;
  }

  int  rc = 0;
  for (int arg = optind; arg < argc; ++arg) {
    pair<char *,size_t>  mf = MapFile(argv[arg]);
    if (mf.first) {
      vector<string>  sccsStrings = FindSccsStrings(mf.first, mf.second);
      PkgMap  pkgMap;
      GetPkgMap(sccsStrings, pkgMap);
      PrintPackages(pkgMap, showAsJson);
#if 0
      if (showAsJson) {
        cout << GetJson(sccsStrings);
      }
      else {
        for (auto sit : sccsStrings) {
          LineMatch(sit);
        }
      }
#endif
      munmap(mf.first, mf.second);
    }
  }
  return rc;
}
