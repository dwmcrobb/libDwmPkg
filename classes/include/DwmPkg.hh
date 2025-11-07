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
//!  \file DwmPkg.hh
//!  \author Daniel W. McRobb
//!  \brief Utility function templates, using C++26 reflection
//---------------------------------------------------------------------------

#ifndef _DWMPKG_HH_
#define _DWMPKG_HH_

#if defined(__cpp_impl_reflection)
#  if defined(__cpp_expansion_statements)
#    if __has_include(<meta>)
#      include <meta>
#      if defined(__cpp_lib_define_static)
#        define DWM_PKG_CAN_USE_REFLECTION 1
#      endif
#    endif
#  endif
#endif

#include <cassert>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "DwmPkgStringLiteral.hh"
#include "DwmPkgInfo.hh"

#if defined(DWM_PKG_CAN_USE_REFLECTION)

namespace Dwm {

  namespace Pkg {

    
    //------------------------------------------------------------------------
    //!  Returns a fully qualified name for the given reflection @c info.
    //!  This won't work for everything, in part because the bloomberg clang
    //!  fork doesn't implement has_parent() or has_c_language_linkage().
    //!  But it's still useful for the set of cases where it works.
    //------------------------------------------------------------------------
    template <std::meta::info info>
    static constexpr auto FQN()
    {
      std::string  s;
      if constexpr (info == ^^::) { // ! std::meta::has_parent(info)
        return s;
      }
      else {
        s = FQN<std::meta::parent_of(info)>();
        if constexpr (^^:: != std::meta::parent_of(info)) {
          s += "::";
        }
        s += std::meta::display_string_of(info);
        return s;
      }
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    auto  append_to_vec = [](auto & l, auto && r) 
    { l.insert(l.end(), r.begin(), r.end()); };
    
    //------------------------------------------------------------------------
    //!  Returns a vector<pair<string,T>> of all instances of type @c T
    //!  found in the given namespace @c NS.  The first member of each pair
    //!  is a "fully-qualified name" and the second is a copy of the instance
    //!  of @c T.  Note that the visibility of the namespace is that of the
    //!  translation unit from which this is called.
    //------------------------------------------------------------------------
    template <typename T, std::meta::info NS>
    requires std::is_copy_constructible_v<T>
    constexpr auto get_vars_in_ns()
    {
      using namespace std::meta;
      
      std::vector<std::pair<std::string,T>>  vars;
      constexpr auto ctx = access_context::unchecked();
      template for (constexpr auto mem :
                      define_static_array(members_of(NS, ctx))) {
        if constexpr (is_namespace(mem)) {
          append_to_vec(vars,get_vars_in_ns<T,mem>());
        }
        else {
          if constexpr (is_variable(mem)) {
            using  memType = typename[:remove_cvref(type_of(mem)):];
            if constexpr (std::same_as<T,memType>) {
              vars.push_back({FQN<mem>(),[:mem:]});
            }
          }
        }
      }
      return vars;
    }

    //------------------------------------------------------------------------
    //!  
    //------------------------------------------------------------------------
    template <typename T, std::meta::info ...NSes>
    constexpr auto get_vars_in_nses()
    {
      std::vector<std::pair<std::string,T>>  vars;
      (append_to_vec(vars, get_vars_in_ns<T,NSes>()), ...);
      return vars;
    }
    
    //------------------------------------------------------------------------
    //!  Returns a vector of pair<string,Dwm::Pkg::Info> holding all
    //!  instances of Dwm::Pkg::Info found within the given namespace
    //!  reflections @c NSes.  Note that the visibility of the namespaces
    //!  is that of the translation unit from which this is called.
    //------------------------------------------------------------------------
    template <std::meta::info ...NSes>
    constexpr auto get_packages()
    { return get_vars_in_nses<Dwm::Pkg::Info,NSes...>(); }

#if 0
    //------------------------------------------------------------------------
    //!  A structural class literal to hold a string literal so we can pass
    //!  a string literal as a non-type template parameter.  Many of us have
    //!  been using nearly this exact code to allow passing of string
    //!  literals as template parameters.
    //------------------------------------------------------------------------
    template <std::size_t N>
    struct string_literal {
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
      constexpr string_literal(const char (&s)[N], std::index_sequence<Is...>)
          : data{{s[Is]...}}
      { }
      
      //----------------------------------------------------------------------
      //!  Construct from string literal input.  Template parameter N is
      //!  deduced.
      //----------------------------------------------------------------------
      constexpr string_literal(const char (&s)[N])
          : string_literal(s, std::make_index_sequence<N>{})
      {}
      
      //----------------------------------------------------------------------
      //!  Convert to std::string_view, excluding the null termination.
      //----------------------------------------------------------------------
      constexpr operator std::string_view() const
      { return std::string_view(data.data(), N - 1); }
    };
#endif
    
    //------------------------------------------------------------------------
    //!  Returns variables that are of the types in @c Ts and inside a
    //!  namespace named @C NSName under the given namespace reflection
    //!  @c NS.  For example, you might want to find all integers strings
    //!  under any namespace named 'package' somewhere under the global
    //!  namespace:
    //!
    //!  auto  vars = get_vars_in_ns<"package",^^::,int,std::string>();
    //!
    //!  We return a vector<pair<string,variant>>, where the first member
    //!  in each pair is a 'fully qualified' name of the variable and
    //!  the second member is a variant holding its value.
    //------------------------------------------------------------------------
    template <StringLiteral NSName, std::meta::info NS, typename ...Ts>
    constexpr auto get_vars_in_ns()
    {
      std::vector<std::pair<std::string,std::variant<Ts...>>>  vars;

      using std::meta::access_context, std::meta::remove_cvref,
        std::meta::is_namespace, std::meta::members_of,
        std::meta::display_string_of, std::meta::is_variable,
        std::meta::type_of;
      
      constexpr auto ctx = access_context::unchecked();
      template for (constexpr auto mem :
                      define_static_array(members_of(NS, ctx))) {
        if constexpr (is_namespace(mem)) {
          if constexpr (display_string_of(mem) == NSName) {
            //  found a matching namespace
            template for (constexpr auto nsmem :
                            define_static_array(members_of(mem, ctx))) {
              if constexpr (is_variable(nsmem)) {
                //  found a variable.  Gets its type, sans cvref
                using  memType = typename[:remove_cvref(type_of(nsmem)):];
                if constexpr (((std::same_as<Ts,memType>) || ...)) {
                  //  variable is of a type listed in the Ts parameter pack.
                  //  Add it to our return, with its FQN.  Note this is a
                  //  copy.
                  vars.push_back({FQN<nsmem>(),[:nsmem:]});
                }
              }
            }
          }
          else {
            //  keep searching (recursive)
            append_to_vec(vars,get_vars_in_ns<NSName,mem,Ts...>());
          }
        }
      }
      return vars;
    }
    
  }  // namespace Pkg

}  // namespace Dwm


#endif

#endif  // _DWMPKG_HH_
