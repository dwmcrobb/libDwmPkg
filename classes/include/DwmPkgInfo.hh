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

#define DWM_PKG_MK_LINE_ARG(x) DWM_PKG_MK_LINE_ARG2(x)
#define DWM_PKG_MK_LINE_ARG2(x) #x
#define DWM_PKG_MK_ARGS(n,v,c)  #n, #v, #c, __FILE_NAME__,                   \
                                "@(#) " #n "-" #v " \xC2\xA9 " #c  " "       \
                                __DATE__ " " __TIME__ " " __FILE_NAME__ ":"  \
                                DWM_PKG_MK_LINE_ARG(__LINE__),               \
                                DWM_PKG_MK_LINE_ARG(__LINE__)

//----------------------------------------------------------------------------
//!  
//----------------------------------------------------------------------------
namespace Dwm {
  
  namespace Pkg {

    //--------------------------------------------------------------------------
    //!  There is some slight trickery here.  I don't want to embed more
    //!  bytes in a binary than necessary.  I want a single string literal
    //!  embedded (_id), and just store offsets and lengths for each portion
    //!  of interest in _id: _name, _version, _copyright, _date, _time, _file
    //!  and _line.  Clang and gcc have __FILE_NAME__, but AFAIK MSVC does not.
    //!  This won't compile with MSVC, but theoretically you could change
    //!  the DWM_PKG_MK_ARGS macro to use __FILE__ instead of __FILE_NAME__,
    //!  you'll just get a much longer embedded _id.  I wish __FILE_NAME__
    //!  was standardized, since many of us need it.
    //!
    //!  The other trickery is that the constructor is a template.  This is
    //!  only so I can avoid using strlen() and instead automagically get
    //!  the size of the passed-in string literals, without being tripped up
    //!  by embedded nulls in the string literals.  Of course, ultimately
    //!  this means the constructor _must_ be called with string literals,
    //!  which is exactly what I want.  And if you intentionally embed nulls
    //!  in any of the constructor arguments... you'll get what you'd expect
    //!  (it's highly discouraged).
    //!
    //!  The easy way to construct an Info is to use the DWM_PKG_MK_ARGS
    //!  macro to build the constructor's arguments.  For a package named
    //!  'foobar' with version 1.2.3, you could put this in your main
    //!  header file (for example, 'foobar.hh'), inside an appropriate
    //!  namespace:
    //!
    //!  inline const Dwm::Pkg::Info info(DWM_PKG_MK_ARGS(foobar,1.2.3,rfdm.com));
    //!
    //!  This would embed a string of this form in the binary:
    //!
    //!  @(#) foobar-1.2.3 mcplex.net Nov  6 2025 17:33:57 foobar.hh:25
    //!
    //!  I'm using the @(#) prefix to make it possible to find this string
    //!  with the old 'what' utility as well as with 'dwmwhat'.
    //--------------------------------------------------------------------------
    class Info
    {
    public:
      constexpr Info() = delete;

#if 0
      constexpr Info(Info &&) = default;
      constexpr Info(const Info &) = default;
      constexpr Info & operator = (Info &) = default;
      constexpr Info & operator = (Info &&) = default;
#endif

      //----------------------------------------------------------------------
      //!  We're sort of abusing the type system here to preserve the
      //!  size of each argument.  Each argument is a string literal, and
      //!  we can capture the length of each string literal with template
      //!  parameter deduction at compile time.
      //----------------------------------------------------------------------
      template <size_t N, size_t V, size_t C, size_t F, size_t I, size_t L>
      constexpr Info(const char (&name)[N], const char (&version)[V],
                     const char (&copyright)[C], const char (&file)[F],
                     const char (&id)[I], const char (&line)[L])
          : _id(id), _idlen(I-1)
      {
        static_assert(I < 0x100);
        _name[0] = strlen("@(#) ");
        _name[1] = N-1;
        _version[0] = _name[0] + _name[1] + 1;
        _version[1] = V-1;
        _copyright[0] = _version[0] + _version[1] + 1;
        _copyright[1] = (C-1) + strlen("\xC2\xA9 ");
        _date[0] = _copyright[0] + _copyright[1] + 1;
        _date[1] = strlen(__DATE__);
        _time[0] = _date[0] + _date[1] + 1;
        _time[1] = strlen(__TIME__);
        _file[0] = _time[0] + _time[1] + 1;
        _file[1] = F - 1;
        _line[0] = _file[0] + _file[1] + 1;
        _line[1] = L - 1;
      }
      
      constexpr std::string_view name() const {
        return std::string_view(_id + _name[0], _name[1]);
      }
      constexpr std::string_view version() const {
        return std::string_view(_id + _version[0], _version[1]);
      }
      constexpr std::string_view copyright() const {
        return std::string_view(_id + _copyright[0], _copyright[1]);
      }
      constexpr std::string_view date() const {
        return std::string_view(_id + _date[0], _date[1]);
      }
      constexpr std::string_view time() const {
        return std::string_view(_id + _time[0], _time[1]);
      }
      constexpr std::string_view id() const {
        return std::string_view(_id, _idlen);
      }
      constexpr std::string_view file() const {
        return std::string_view(_id + _file[0], _file[1]);
      }
      constexpr uint32_t line() const {
        char  *endp = (char *)(_id + _line[0] + _line[1]);
        return std::strtoul(_id + _line[0], &endp, 10);
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      friend std::ostream & operator << (std::ostream & os, const Info & p)
      {
        if (p._id) {
          os << p.name() << ' ' << p.version() << ' ' << p.copyright() << ' '
             << p.date() << ' ' << p.time() << " {" << p.file() << ':'
             << p.line() << '}';
        }
        return os;
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      std::ostream & print_json(std::ostream & os) const
      {
        return os << as_json();
      }
      
      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      std::string as_json() const
      {
        if (_id) {
          return "{\"name\": \"" + std::string(name())
            + "\", \"version\": \"" + std::string(version())
            + "\", \"copyright\": \"" + std::string(copyright())
            + "\", \"date\": \"" + std::string(date())
            + "\", \"time\": \"" + std::string(time())
            + "\", \"id\": \"" + std::string(id())
            + "\", \"file\": \"" + std::string(file())
            + "\", \"line\": " + std::to_string(line()) + '}';
        }
        return std::string();
      }
      
    private:
      const char   *_id;
      uint8_t       _idlen;
      uint8_t       _name[2];
      uint8_t       _version[2];
      uint8_t       _copyright[2];
      uint8_t       _date[2];
      uint8_t       _time[2];
      uint8_t       _file[2];
      uint8_t       _line[2];
    };

    inline const Info     info(DWM_PKG_MK_ARGS(DwmPkg,0.0.42,Daniel McRobb 2025 \xF0\x9F\x8E\x83));
    
  }  // namespace Pkg

}  // namespace Dwm

#endif  // _DWMPKGINFO_HH_
