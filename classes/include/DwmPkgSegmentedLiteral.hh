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
//!  \file DwmPkgSegmentedLiteral.hh
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#ifndef _DWMPKGSEGMENTEDLITERAL_HH_
#define _DWMPKGSEGMENTEDLITERAL_HH_


#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <numeric>
#include <ranges>
#include <string_view>
#include <type_traits>

namespace Dwm {

  namespace Pkg {

    //------------------------------------------------------------------------
    //!  The idea of this template: hold a concatenated string literal that
    //!  is constructed from a variadic list of string literals, with the
    //!  length of each string literal stored in an array.  This allows one
    //!  string literal to be stored in a binary, but each 'segement' is
    //!  retrievable via the @c nth() member.
    //!  This is primarily here to support the Dwm::Pkg::Info class, but
    //!  I split it out simply because it's useful for other things.
    //!  You can do this:
    //!
    //!  SegmentedLiteral foo("foo","1.0.1","My Name 2025",__DATE__,__FILE__);
    //!
    //!  and be able to pull out "foo" with nth(0), "1.0.1" with nth(1),
    //!  etc.
    //!
    //!  Probably worth noting some of the goals of this class template:
    //!   - Keep a contiguous single string literal so it can be found as
    //!     one string in a binary.
    //!   - Try to minimize the space utilized for the array of segment
    //!     lengths.
    //!   - Almost everything needs to be done at compile time so that
    //!     the compiler is aware of a single string literal ("constexpr
    //!     all the things", Jason Turner).
    //------------------------------------------------------------------------
    template <std::size_t DelimLen, std::size_t NumSegs, std::size_t NumChars>
    class SegmentedLiteral
    {
    public:
      using SegLenType =
        std::conditional<(NumChars <= 256),
                         uint8_t,
                         typename std::conditional<(NumChars <= 65536),
                                                   uint16_t,
                                                   uint32_t>::type>::type;
      using BufType = const char(&)[NumChars];

      //----------------------------------------------------------------------
      //!  
      //----------------------------------------------------------------------
      template <std::size_t D, std::size_t F, std::size_t ...Ns>
      consteval SegmentedLiteral(const char (&delim)[D], const char (&f)[F],
                                 const char (&...s)[Ns])
      {
        static_assert((NumChars) <= std::numeric_limits<SegLenType>::max());

        //  'f' is just 'first'
        auto  it = std::ranges::copy_n(f,F-1,_buffer).out;
        std::size_t  si = 0;
        seglengths[si++] = F-1;
        ((seglengths[si++] = Ns-1,
          it = std::ranges::copy_n(delim,D-1,it).out,
          it = std::ranges::copy_n(s,Ns-1,it).out), ...);
        *it = '\0';
      }
      
      //----------------------------------------------------------------------
      //!  Returns a view of the whole buffer, minus the terminating null.
      //----------------------------------------------------------------------
      consteval operator std::string_view () const noexcept
      { return std::string_view(_buffer,size-1); }

      //----------------------------------------------------------------------
      //!  Returns the buffer.
      //----------------------------------------------------------------------
      constexpr operator BufType () const noexcept
      { return _buffer; }

      //----------------------------------------------------------------------
      //!  Returns a view of the whole buffer, minus the terminating null.
      //----------------------------------------------------------------------
      constexpr std::string_view view() const noexcept
      { return std::string_view(_buffer,size-1); }
        
      //----------------------------------------------------------------------
      //!  Returns the number of segments in the buffer (1 or more).
      //----------------------------------------------------------------------
      constexpr std::size_t num_segments() const noexcept
      { return NumSegs; }
      
      //----------------------------------------------------------------------
      //!  Returns a view of the nth segment.
      //----------------------------------------------------------------------
      constexpr std::string_view nth(std::size_t n) const noexcept
      {
        assert(n < NumSegs);
        std::size_t  off = std::accumulate(seglengths, &seglengths[n], 0);
        off += (n - 0) * delimLen;
        return std::string_view(_buffer + off, seglengths[n]);
      }
      
      //----------------------------------------------------------------------
      //!  Returns the size of the type used to hold segment lengths.
      //----------------------------------------------------------------------
      constexpr std::size_t size_of_seg_lengths() const noexcept
      { return sizeof(SegLenType); }

    private:
      char         _buffer[NumChars] {};
      std::size_t  size = NumChars;
      SegLenType   seglengths[NumSegs] {};
      std::size_t  delimLen = DelimLen;
    };

    //------------------------------------------------------------------------
    //!  Just a helper to hold the number of characters we need for the
    //!  buffer based on the template arguments.  We need space for N
    //!  segments and N-1 delimiters.
    //------------------------------------------------------------------------
    template <std::size_t D, std::size_t ...Ns>
    struct SegmentedLiteralChars {
      static const size_t sz =
        ((D - 1) * (sizeof...(Ns) - 1)) + ((Ns-1) + ...) + 1;
    };

    template <std::size_t D, std::size_t ...Ns>
    constexpr size_t SegmentedLiteralChars_v =
      SegmentedLiteralChars<D,Ns...>::sz;
    
    //------------------------------------------------------------------------
    //!  Deduction guide.  This is critical, since we need to deduce DelimLen,
    //!  NumSegs and NumChars to instantiate the template.  We deduce
    //!  DelimLen from the length of delim, minus 1 (remove null).  We
    //!  deduce the number of segments from the number of variadic
    //!  arguments (sizeof...(Ns)).  To deduce NumChars, we add up the
    //!  length of all of the variadic arguments (minus 1 for each to ignore
    //!  null termination).  We then add the number of characters we need to
    //!  store delimiter strings.  Here we need D-1 characters for each,
    //!  and we need (sizeof...(Ns) - 1) of them (they only go between
    //!  segments, not before the first segment or after the last segment).
    //------------------------------------------------------------------------
    template <std::size_t D, std::size_t ...Ns>
    SegmentedLiteral(const char (&delim)[D], const char (&...s)[Ns])
      -> SegmentedLiteral<D-1,sizeof...(Ns),SegmentedLiteralChars_v<D,Ns...>>;
    
  }  // namespace Pkg

}  // namespace Dwm

#endif  // _DWMPKGSEGMENTEDLITERAL_HH_
