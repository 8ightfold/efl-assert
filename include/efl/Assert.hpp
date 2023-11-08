//===- efl/Assert.hpp -----------------------------------------------===//
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
//  ...
//
//===----------------------------------------------------------------===//

#pragma once

#ifndef EFL_ASSERT_ASSERT_HPP
#define EFL_ASSERT_ASSERT_HPP

#include <cassert>
#include <cstddef>
#include <efl/Config.hpp>

namespace efl {
namespace literals {
    inline bool operator""_Assert(const char*, std::size_t) NOEXCEPT {
        return false;
    }
} // namespace literals
} // namespace efl

#if defined(EFLI_ASSERT_USE_CODEGENNED_) && \
  __has_include(<efl/AssertGen.hpp>)
#  include "AssertGen.hpp"
#  define EFL_ASSERT_FALLBACK 0
#else
#  include "AssertFallback.hpp"
#  define EFL_ASSERT_FALLBACK 1
#endif

#ifndef EFLI_NDEBUG_
#  define EflDbgAssert(...) EflAssert(__VA_ARGS__)
#else
/// No assert for you!
#  define EflDbgAssert(...) (void)0
#endif

#endif // EFL_ASSERT_ASSERT_HPP