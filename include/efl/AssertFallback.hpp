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

#if defined(COMPILER_GCC) || defined(COMPILER_ICC)
#  define EflAssert(chk, str) EFLI_ASSERT_BEXPECT_((chk), str)
#elif defined(COMPILER_CLANG)
#  if __has_builtin(__builtin_expect)
#    define EflAssert(chk, str) EFLI_ASSERT_BEXPECT_((chk), str)
#  else
#    define EflAssert(chk, str) EFLI_ASSERT_DEFAULT_((chk), str)
#  endif
#else
#  define EflAssert(chk, str) EFLI_ASSERT_DEFAULT_((chk), str)
#endif

/// For compilers with `__builtin_expect(v, probability)`
#define EFLI_ASSERT_BEXPECT_(chk, str) \
  EFLI_ASSERT_BOX_(__builtin_expect((!(chk)), 0), str)

/// For compilers with with no special features
#define EFLI_ASSERT_DEFAULT_(chk, str) \
  EFLI_ASSERT_BOX_((!(chk)), str)

/// Assumes `ichk` is inverted
#define EFLI_ASSERT_BOX_(ichk, str) \
  do { if(ichk) UNLIKELY { using namespace efl::literals; assert(str ## _Assert); } } while(0)

#define EflDynAssert(...) \
  static_assert(false, "Cannot use dynamic assertions in fallback!")

#endif // EFL_ASSERT_ASSERTFALLBACK_HPP
