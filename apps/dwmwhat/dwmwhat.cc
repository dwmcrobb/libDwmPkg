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
#include <regex>
#include <vector>
#include <utility>  // for std::pair

#include "DwmPkg.hh"

using namespace std;

static bool g_shortOption = false;

namespace Crap {
  namespace Pkg {
    const char  *p = "raw pointer to char";
    std::string  junk("junk content");
  }
}

#define JACKOLANTERN "\xF0\x9F\x8E\x83"
#define COPYRTSYMBOL "\xC2\xA9"
#define DWMWHAT_COPYRIGHT "Copyright " COPYRTSYMBOL " Daniel McRobb " \
  JACKOLANTERN " 2025"

namespace Dwm {
  namespace dwmwhat {
    inline constexpr Dwm::Pkg::Info
    info("dwmwhat","1.0.0",DWMWHAT_COPYRIGHT,
         __DATE__,__TIME__,__FILE_NAME__);
  }
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

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
static void Usage(const char *argv0)
{
  std::cerr << "Usage: " << argv0 << " [-v] files...\n";
  return;
}

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  bool  showVersion = false;
  int  optChar;
  while ((optChar = getopt(argc, argv, "v")) != -1) {
    switch (optChar) {
      case 'v':
        showVersion = true;
        break;
      default:
        Usage(argv[0]);
        return 1;
        break;
    }
  }

  if (showVersion) {
#if defined(DWM_PKG_CAN_USE_REFLECTION)
    auto pkgs = Dwm::Pkg::get_packages<^^Dwm>();
    for (const auto & pkg : pkgs) {
      std::cout << pkg.second << '\n'; // << pkg.second.as_json() << '\n';
    }
#else
    std::cout << Dwm::Pkg::info << '\n'; // << Dwm::Pkg::info.as_json() << '\n';
#endif
    return 0;
  }

#if 0
  Dwm::Pkg::StringLiteral l("left_literal");
  Dwm::Pkg::StringLiteral r("right_literal");
  std::cout << std::string_view(l+r);
#endif

#if 0
  auto  vars = Dwm::Pkg::get_vars_in_ns<"Pkg",^^::, Dwm::Pkg::Info,
                                        std::string, const char *>();
  for (const auto & var : vars) {
    std::cout << var.first;
    std::visit([] (auto && arg)
    {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T,Dwm::Pkg::Info>
                    || std::is_same_v<T,std::string>) {
        std::cout << ' ' << arg;
      }
      else if constexpr (std::is_same_v<T,const char *>) {
        if (arg != nullptr) {
          std::cout << ' ' << arg;
        }
      }
    }, var.second);
    std::cout << '\n';
  }
#endif
  
  int  rc = 0;
  for (int arg = optind; arg < argc; ++arg) {
    pair<char *,size_t>  mf = MapFile(argv[arg]);
    if (mf.first) {
      vector<string>  sccsStrings = FindSccsStrings(mf.first, mf.second);
      for (auto sit : sccsStrings) {
        LineMatch(sit);
      }
      munmap(mf.first, mf.second);
    }
  }
  return rc;
}
