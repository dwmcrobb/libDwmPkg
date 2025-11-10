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
//!  \brief NOT YET DOCUMENTED
//---------------------------------------------------------------------------

#include <cassert>

#include "DwmPkgInfo.hh"

inline constexpr const Dwm::Pkg::Info __attribute__((used))
g_info1(DWM_PKG_TYPE_HDR, "g_info1", DWM_PKG_STATUS_RC, "0.0.1",
         "Daniel McRobb 2025 " DWM_PKG_SYM_JACKOLANTERN DWM_PKG_SYM_GHOST " ",
         "\xE2\x96\xB6 mcplex.net");

int main(int argc, char *argv[])
{
  assert(g_info1.type() == DWM_PKG_TYPE_HDR);
  assert(g_info1.name() == "g_info1");
  assert(g_info1.status() == DWM_PKG_STATUS_RC);
  assert(g_info1.version() == "0.0.1");
  assert(g_info1.copyright() == "Daniel McRobb 2025 " DWM_PKG_SYM_JACKOLANTERN DWM_PKG_SYM_GHOST " ");
  assert(g_info1.other() == "\xE2\x96\xB6 mcplex.net");
  return 0;
}
