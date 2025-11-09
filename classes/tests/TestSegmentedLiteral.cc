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
//!  \file TestSegmentedLiteral.cc
//!  \author Daniel W. McRobb
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#include <cassert>
#include <iostream>

#include "DwmPkgSegmentedLiteral.hh"

inline constexpr const Dwm::Pkg::SegmentedLiteral
TestSegmentedLiteral(" ","@(#)","TestSegmentedLiteral","",
                     "Copyright Daniel McRobb 2025",__DATE__,__TIME__);

int main(int argc, char *argv[])
{
  constexpr Dwm::Pkg::SegmentedLiteral  allEmpty("","","","");
  std::string_view  sv = allEmpty.view();
  assert(sv.size() == 0);
  
  constexpr Dwm::Pkg::SegmentedLiteral  justEnoughArgs("","");
  assert(justEnoughArgs.view().size() == 0);

  constexpr Dwm::Pkg::SegmentedLiteral  delimsOnly(" ","","","");
  assert(delimsOnly.view().size() == 2);
  assert(delimsOnly.nth(0) == "");
  assert(delimsOnly.nth(1) == "");
  assert(delimsOnly.nth(2) == "");

  constexpr Dwm::Pkg::SegmentedLiteral  emptyDelim("","abc","defg","hijkl");
  assert(emptyDelim.view().size()
         == strlen("abc") + strlen("defg") + strlen("hijkl"));
  assert(emptyDelim.nth(0) == "abc");
  assert(emptyDelim.nth(1) == "defg");
  assert(emptyDelim.nth(2) == "hijkl");

  assert(TestSegmentedLiteral.nth(0) == "@(#)");
  assert(TestSegmentedLiteral.nth(1) == "TestSegmentedLiteral");

  std::cout << TestSegmentedLiteral.view() << '\n';
  
  return 0;
}
