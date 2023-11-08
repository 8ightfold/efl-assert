cmake_minimum_required(VERSION 3.12)
include_guard(DIRECTORY)

project(
  efl-assert
  VERSION 0.1.1
  LANGUAGES CXX
)

include(cmake/LoopUtils.cmake)
include(cmake/LocateAssert.cmake)
include(cmake/LocateLang.cmake)

set(__EFL_ASSERT_TLINC ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT DEFINED EFL_ASSERT_HEADER)
  __efl_assert_locate(__EFL_ASSERT_HEADER)
else(NOT DEFINED __EFL_ASSERT_HEADER)
  message("[efl::assert] Using explicit path to `assert.h`...")
  if(NOT EXISTS EFL_ASSERT_HEADER)
    message(FATAL_ERROR "[efl::assert] Could not locate `assert.h`. "
      "Ensure your path is not relative.")
  endif()
  set(__EFL_ASSERT_HEADER ${EFL_ASSERT_HEADER} 
    CACHE PATH "Explicit path to `assert.h`" FORCE)
endif()

message("[efl::assert] assert-path: ${__EFL_ASSERT_HEADER}")

add_subdirectory(conf)
add_subdirectory(submodules/efl-config)

add_library(__efl_assert INTERFACE)
target_link_libraries(__efl_assert INTERFACE efl::config)
target_include_directories(__efl_assert INTERFACE include)
target_compile_definitions(__efl_assert INTERFACE EFL_ASSERT_VERSION="${PROJECT_VERSION}")

if(__EFL_ASSERT_USE_CODEGENNED)
  target_compile_definitions(__efl_assert INTERFACE
    EFLI_ASSERT_USE_CODEGENNED_=1)
  message("[efl::assert] enable-codegen: ON")
else()
  message("[efl::assert] enable-codegen: OFF")
endif()

add_library(efl::assert ALIAS __efl_assert)
