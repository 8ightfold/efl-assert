if(NOT EFL_ASSERT_FALLBACK STREQUAL __EFL_ASSERT_FALLBACK)
  set(__EFL_ASSERT_FALLBACK ${EFL_ASSERT_FALLBACK} 
    CACHE BOOL "Force use of fallback header." FORCE)
  unset(__EFL_ASSERT_USE_CODEGENNED CACHE)
  unset(USE_CODEGENNED CACHE)
endif()

if(EFL_ASSERT_FALLBACK)
  set(__EFL_ASSERT_USE_CODEGENNED OFF 
    CACHE BOOL "Force use of fallback header." FORCE)
endif()

if(DEFINED __EFL_ASSERT_USE_CODEGENNED)
  __efl_assert_propagate(__EFL_ASSERT_USE_CODEGENNED)
  return()
endif()

##=== Builtin Config ========================================##

include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}")
check_cxx_source_compiles("#include \"HasBuiltinExpect.cpp\"" HAS_BUILTIN_EXPECT)

if(HAS_BUILTIN_EXPECT)
  set(__EFL_ASSERT_EXPECT "__builtin_expect(!!(chk), 1)")
  message("[efl::assert] builtin-expect: ON")
else()
  set(__EFL_ASSERT_EXPECT "(!!(chk))")
  message("[efl::assert] builtin-expect: OFF")
endif()

set(__EFL_ASSERT_EXPECT ${__EFL_ASSERT_EXPECT} 
  CACHE STRING "Body for __builtin_expect wrapper.")

##=== Assert Parsing ========================================##
__efl_find_perl()
__efl_assert_parse(${CMAKE_CURRENT_SOURCE_DIR})

message(DEBUG "[efl::assert] script-path: ${__EFL_PARSER_SCRIPT}")
message(DEBUG "[efl::assert] scripting-ver: ${PERL_VERSION}")
message(DEBUG "[efl::assert] scripting-exe: ${PERL_EXECUTABLE}")

if(__EFL_ASSERT_DEFINITION_ERR STREQUAL "")
  set(__EFL_ASSERT_DEFINITION ${__EFL_ASSERT_DEFINITION} 
    CACHE STRING "The generated assertion definition.")
  message(DEBUG "[efl::assert] Perl-output: ${__EFL_ASSERT_DEFINITION}")
  configure_file(
    ${PROJECT_SOURCE_DIR}/include/efl/AssertGen.hpp.in
    ${PROJECT_SOURCE_DIR}/include/efl/AssertGen.hpp @ONLY
  )
else()
  string(STRIP "${__EFL_ASSERT_DEFINITION_ERR}" __EFL_ASSERT_DEFINITION_ERR)
  message(SEND_ERROR "[efl::assert] Perl-error: `${__EFL_ASSERT_DEFINITION_ERR}`")
endif()

get_target_property(__EFL_CONFIG_INCLUDES efl::config INTERFACE_INCLUDE_DIRECTORIES)
list(APPEND CMAKE_REQUIRED_INCLUDES "${__EFL_CONFIG_INCLUDES}")
list(APPEND CMAKE_REQUIRED_INCLUDES "${PROJECT_SOURCE_DIR}/include")
check_cxx_source_compiles("#include \"CompileTest.cpp\"" USE_CODEGENNED)

set(__EFL_ASSERT_USE_CODEGENNED ${USE_CODEGENNED} 
  CACHE BOOL "Whether or not to use codegenned header." FORCE)
__efl_assert_propagate(__EFL_ASSERT_USE_CODEGENNED)
