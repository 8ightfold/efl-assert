//===- efl/AssertGenFormatter.hpp -----------------------------------===//
//
// Copyright (C) 2023 Eightfold
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
//     limitations under the License.
//
//===----------------------------------------------------------------===//
//
//  This header uses CMake to generate definitions for `EflAssert`
//  and `EflDynAssert`. If no definition can be generated, 
//  `AssertFallback.hpp` will be used instead.
//
//===----------------------------------------------------------------===//

#pragma once

#ifndef EFL_ASSERT_ASSERTGENFORMATTER_HPP
#define EFL_ASSERT_ASSERTGENFORMATTER_HPP

#include <sstream>
#include <efl/Config.hpp>

namespace efl {
namespace assert {
  ALWAYS_INLINE void fmt_args(std::ostream&) { }

  template <typename Arg, typename...Args>
  void fmt_args(std::ostream& os, Arg&& arg, Args&&...args) {
    fmt_args(os << FWD(arg), FWD(args)...);
  }

  template <typename...Args>
  std::string parse_args(Args&&...args) {
    std::stringstream ss;
    fmt_args(ss << std::boolalpha, FWD(args)...);
    return ss.rdbuf()->str();
  }
} // namespace assert
} // namespace efl

#define EflDynAssertDefaultFormatter(...) (::efl::assert::parse_args(__VA_ARGS__).c_str())

#endif // EFL_ASSERT_ASSERTGENFORMATTER_HPP
