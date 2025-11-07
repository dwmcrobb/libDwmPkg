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
//!  \file DwmPkgStringLiteral.hh
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#ifndef _DWMPKGSTRINGLITERAL_HH_
#define _DWMPKGSTRINGLITERAL_HH_

#include <array>
#include <string_view>
#include <utility>  // for std::index_sequence

namespace Dwm {

  namespace Pkg {
    
    //------------------------------------------------------------------------
    //!  A structural class literal to hold a string literal so we can pass
    //!  a string literal as a non-type template parameter.  Many of us have
    //!  been using nearly this exact code to allow passing of string
    //!  literals as template parameters.
    //------------------------------------------------------------------------
    template <std::size_t N>
    struct StringLiteral {
      //----------------------------------------------------------------------
      //!  The encapsulated data.
      //----------------------------------------------------------------------
      std::array<char, N> data;
      
      //----------------------------------------------------------------------
      //!  Construct from a C-style string literal using a non-type template
      //!  parameter pack for characters.  A bit ugly but works and the way
      //!  many others have done this.
      //----------------------------------------------------------------------
      template <std::size_t... Is>
      constexpr StringLiteral(const char (&s)[N], std::index_sequence<Is...>)
          : data{{s[Is]...}}
      { }
      
      //----------------------------------------------------------------------
      //!  Construct from string literal input.  Template parameter N is
      //!  deduced.
      //----------------------------------------------------------------------
      constexpr StringLiteral(const char (&s)[N])
          : StringLiteral(s, std::make_index_sequence<N>{})
      {}
      
      //----------------------------------------------------------------------
      //!  Convert to std::string_view, excluding the null termination.
      //----------------------------------------------------------------------
      constexpr operator std::string_view() const
      { return std::string_view(data.data(), N - 1); }

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      constexpr size_t size() const  { return N - 1; }
    };

  }  // namespace Pkg

}  // namespace Dwm

#endif  // _DWMPKGSTRINGLITERAL_HH_
