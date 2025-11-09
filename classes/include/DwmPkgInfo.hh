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
//!  \file DwmPkgInfo.hh
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#ifndef _DWMPKGINFO_HH_
#define _DWMPKGINFO_HH_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <source_location>

#include "DwmPkgSegmentedLiteral.hh"

#define DWM_PKG_COPYRIGHT     "\xC2\xA9\xEF\xB8\x8F"
#define DWM_PKG_GHOST         "\xF0\x9F\x91\xBB"
#define DWM_PKG_FILE_FOLDER   "\xF0\x9F\x93\x81"
#define DWM_PKG_OPEN_FOLDER   "\xF0\x9F\x93\x82"
#define DWM_PKG_JACKOLANTERN  "\xF0\x9F\x8E\x83"    // jack-o-lantern

// #define DWM_PKG_DELIM " \xF0\x9F\x91\xBD "    // alien
// #define DWM_PKG_DELIM " \xF0\x9F\xA4\x96 "    // robot emoji
// #define DWM_PKG_DELIM " \xE2\x9A\x99 "        // gear
// #define DWM_PKG_DELIM " \xF0\x9F\x8C\x80 "    // cyclone
// #define DWM_PKG_DELIM " \xF0\x9F\x91\xB7 "    // construction worker
// #define DWM_PKG_DELIM " \xF0\x9F\x9A\x80 "    // rocket
// #define DWM_PKG_DELIM " \xE2\x9E\xA1\xEF\xB8\x8F  "  // right arrow
#define DWM_PKG_DELIM " "

namespace Dwm {

  namespace Pkg {

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    template <std::size_t N, std::size_t V, std::size_t C, std::size_t D,
              std::size_t T, std::size_t F>
    class Info
    {
    public:
      consteval Info(const char (&name)[N], const char (&version)[V],
                     const char (&cpyright)[C], const char (&date)[D],
                     const char (&time)[T], const char (&file)[F])
          : _sl(DWM_PKG_DELIM,"@(#)",name,version,cpyright,date,time,file)
      {}
      
      constexpr std::string_view name() const noexcept
      { return _sl.nth(1); }
      
      constexpr std::string_view version() const noexcept
      { return _sl.nth(2); }
      
      constexpr std::string_view copyright() const noexcept
      { return _sl.nth(3); }
      
      constexpr std::string_view date() const noexcept
      { return _sl.nth(4); }
      
      constexpr std::string_view time() const noexcept
      { return _sl.nth(5); }
      
      constexpr std::string_view file() const noexcept
      { return _sl.nth(6); }
      
      constexpr std::string as_json() const noexcept
      {
        return "{\"name\": \"" + std::string(name())
          + "\", \"version\": \"" + std::string(version())
          + "\", \"copyright\": \"" + std::string(copyright())
          + "\", \"date\": \"" + std::string(date())
          + "\", \"time\": \"" + std::string(time())
          + "\", \"id\": \"" + std::string(_sl)
          + "\", \"file\": \"" + std::string(file()) + "\"}";
      }

      consteval operator std::string_view () const noexcept
      { return _sl; }

      consteval std::string_view view() const noexcept
      {
        return std::string_view(name().data(),
                                _sl.view().size()
                                - (name().data() - _sl.view().data()));
      }
        
      constexpr size_t size_of_seg_lens() const noexcept
      { return _sl.size_of_seg_lengths(); }

  constexpr size_t num_segments() const noexcept
      { return _sl.num_segments(); }
      
    private:
      consteval static auto calc_num_chars() {
        //  The total chars we need is the sum of the chars for
        //  name (N), version (V), copyright (C), data (D), time (T)
        // and file (F), plus 6 delimiters and the '@(#}' prefix..
        return (sizeof("@(#)") + N + V + C + D + T + F
                + (6 * sizeof(DWM_PKG_DELIM)));
      };
      
      SegmentedLiteral<sizeof(DWM_PKG_DELIM)-1,7,calc_num_chars()>  _sl;
    };

  }  // namespace Pkg

}  // namespace Dwm

#endif  // _DWMPKGINFO_HH_
