//===- efl/AssertFallback.hpp ---------------------------------------===//
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
//  The header to be included when AssertGen.hpp is not created.
//
//===----------------------------------------------------------------===//

#pragma once

#ifndef EFL_ASSERT_ASSERTFALLBACK_HPP
#define EFL_ASSERT_ASSERTFALLBACK_HPP

#include <efl/Config.hpp>

//=== Exposed API ===//
#define EflAssert(chk, str) EFLI_ASSERT_BOX_(EFLI_ASSERT_EXPECT_(!(chk)), str)
#define EflDynAssert(chk, ...) assert(chk)

#ifndef EflDynAssertFormatter
#  define EflDynAssertFormatter(...) "Dynamic Assertion"
#endif

//=== Implementation ===//
#ifdef __has_builtin
#  define EFLI_ASSERT_HBUILTIN_(n) __has_builtin(n)
#else
#  define EFLI_ASSERT_HBUILTIN_(...) 0
#endif

#if EFLI_ASSERT_HBUILTIN_(__builtin_expect)
#  define EFLI_ASSERT_EXPECT_
#elif defined(COMPILER_GNU)
#  if __GNUC__ >= 13
#    define EFLI_ASSERT_EXPECT_
#  endif
#elif defined(COMPILER_ICC)
#  if (__INTEL_COMPILER / 100) >= 13
#    define EFLI_ASSERT_EXPECT_
#  endif
#endif

#ifdef EFLI_ASSERT_EXPECT_
#  undef  EFLI_ASSERT_EXPECT_
#  define EFLI_ASSERT_EXPECT_(ichk) __builtin_expect(!!(ichk), 1)
#else
#  define EFLI_ASSERT_EXPECT_(ichk) (!!(ichk))
#endif

/// Assumes `ichk` is inverted
#define EFLI_ASSERT_BOX_(ichk, str) \
  do { if(ichk) UNLIKELY { using namespace ::efl::literals; assert(str ## _f); } } while(0)

#endif // EFL_ASSERT_ASSERTFALLBACK_HPP
