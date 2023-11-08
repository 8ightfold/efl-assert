if(DEFINED __EFL_ASSERT_USE_CODEGENNED)
  __efl_assert_propagate(__EFL_ASSERT_USE_CODEGENNED)
  return()
endif()

include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}")
check_cxx_source_compiles("#include \"HasBuiltinExpect.cpp\"" HAS_BUILTIN_EXPECT)

if(HAS_BUILTIN_EXPECT)
  set(__EFL_ASSERT_EXPECT "__builtin_expect((chk), 1)")
  message("[efl::assert] builtin-expect: ON")
else()
  set(__EFL_ASSERT_EXPECT "(chk)")
  message("[efl::assert] builtin-expect: OFF")
endif()

set(__EFL_ASSERT_EXPECT ${__EFL_ASSERT_EXPECT} 
  CACHE STRING "Body for __builtin_expect wrapper.")

##=== Assert Parsing ===##
__efl_assert_find_any(
  Ruby 
  Perl 
  Python
)

if(DEFINED __EFL_ASSERT_SCRIPT_LANG)
  set(__EFL_ASSERT_SCRIPT_EXE ${${__EFL_ASSERT_SL}_EXECUTABLE})
  set(__EFL_ASSERT_SCRIPT_VER ${${__EFL_ASSERT_SL}_VERSION})
  if("${__EFL_ASSERT_SCRIPT_LANG}" STREQUAL "Ruby")
    set(__EFL_ASSERT_SCRIPT_EXT "rb")
  elseif("${__EFL_ASSERT_SCRIPT_LANG}" STREQUAL "Perl")
    set(__EFL_ASSERT_SCRIPT_EXT "pl")
  elseif("${__EFL_ASSERT_SCRIPT_LANG}" STREQUAL "Python")
    set(__EFL_ASSERT_SCRIPT_EXT "py")
  endif()
else()
  message(FATAL_ERROR 
    "[efl::assert] Could not locate a scripting language (Perl, Ruby, Python).")
endif()

if(NOT "${__EFL_ASSERT_SCRIPT_LANG}" STREQUAL "Ruby")
  message(SEND_ERROR 
    "${__EFL_ASSERT_SCRIPT_LANG} is currently unsupported.")
endif()

__efl_assert_parse(${CMAKE_CURRENT_SOURCE_DIR})
message("[efl::assert] scripting-lang: ${__EFL_ASSERT_SCRIPT_LANG}")
message(DEBUG "[efl::assert] script-path: ${__EFL_PARSER_SCRIPT}")
message(DEBUG "[efl::assert] scripting-ver: ${__EFL_ASSERT_SCRIPT_VER}")
message(DEBUG "[efl::assert] scripting-exe: ${__EFL_ASSERT_SCRIPT_EXE}")

if(__EFL_ASSERT_DEFINITION_ERR STREQUAL "")
  set(__EFL_ASSERT_DEFINITION ${__EFL_ASSERT_DEFINITION} 
    CACHE STRING "The generated assertion definition.")
  message(DEBUG "[efl::assert] ${__EFL_ASSERT_SCRIPT_LANG}-output: ${__EFL_ASSERT_DEFINITION}")
  configure_file(
    ${__EFL_ASSERT_TLINC}/include/efl/AssertGen.hpp.in
    ${__EFL_ASSERT_TLINC}/include/efl/AssertGen.hpp @ONLY
  )
else()
  string(STRIP "${__EFL_ASSERT_DEFINITION_ERR}" __EFL_ASSERT_DEFINITION_ERR)
  message(SEND_ERROR "[efl::assert] ${__EFL_ASSERT_SCRIPT_LANG}-error: `${__EFL_ASSERT_DEFINITION_ERR}`")
endif()

list(APPEND CMAKE_REQUIRED_INCLUDES "${__EFL_ASSERT_TLINC}/include")
list(APPEND CMAKE_REQUIRED_INCLUDES "${__EFL_ASSERT_TLINC}/submodules/efl-config/include")
list(APPEND CMAKE_REQUIRED_DEFINITIONS "-DEFLI_ASSERT_USE_CODEGENNED_=1")
check_cxx_source_compiles("#include \"CompileTest.cpp\"" USE_CODEGENNED)
set(__EFL_ASSERT_USE_CODEGENNED ${USE_CODEGENNED} 
  CACHE BOOL "Whether or not to use codegenned header.")
__efl_assert_propagate(__EFL_ASSERT_USE_CODEGENNED)