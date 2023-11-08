function(__efl_assert_search_retrace __in __out)
  set(__EFL_ASSERT_FSTUB ${${__in}})
  cmake_path(GET __EFL_ASSERT_FSTUB ROOT_PATH __EFL_ASSERT_ROOT)
  set(__EFL_COUNT 0)

  while(NOT (__EFL_ASSERT_FSTUB STREQUAL __EFL_ASSERT_ROOT))
    # message("stub: ${__EFL_ASSERT_FSTUB}")
    cmake_path(GET __EFL_ASSERT_FSTUB PARENT_PATH __EFL_ASSERT_FSTUB)
    if(IS_DIRECTORY ${__EFL_ASSERT_FSTUB}/include)
      set(__EFL_ASSERT_INC_DIR ${__EFL_ASSERT_FSTUB}/include)
      break()
    endif()
    __efl_bound_loop(__EFL_COUNT 25)
  endwhile()

  if(DEFINED __EFL_ASSERT_INC_DIR)
    find_file(__EFL_ASSERT_HEADER "assert.h"
      PATHS ${__EFL_ASSERT_INC_DIR})
  endif()

  if(DEFINED __EFL_ASSERT_HEADER AND 
   NOT (${__EFL_ASSERT_HEADER} STREQUAL "__EFL_ASSERT_HEADER-NOTFOUND"))
    set(${__out} ${__EFL_ASSERT_HEADER} PARENT_SCOPE)
    unset(__EFL_ASSERT_INC_DIR CACHE)
  endif()
endfunction()

function(__efl_assert_search_dirs __in __out)
  foreach(__INC_DIR ${${__in}})
    if(EXISTS ${__INC_DIR}/assert.h)
      set(${__out} ${__INC_DIR}/assert.h PARENT_SCOPE)
      break()
    endif()
  endforeach()
endfunction()

function(__efl_assert_locate_header __out)
  if(DEFINED CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)
    set(__EFL_ASSERT_FPATH "${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES}")
    __efl_assert_search_dirs(__EFL_ASSERT_FPATH ${__out})
  elseif(DEFINED CMAKE_CXX_COMPILER)
    set(__EFL_ASSERT_FPATH "${CMAKE_CXX_COMPILER}")
    __efl_assert_search_retrace(__EFL_ASSERT_FPATH ${__out})
  else()
    message(FATAL_ERROR "[efl::assert] Could not locate `assert.h`. "
      "Try setting CMAKE_CXX_COMPILER or using CMAKE_TOOLCHAIN_FILE.")
  endif()

  if(DEFINED __EFL_ASSERT_FPATH)
    set(${__out} ${${__out}} PARENT_SCOPE)
  endif()
endfunction()

macro(__efl_assert_locate __out)
  if(NOT DEFINED ${__out})
    message("[efl::assert] Searching for `assert.h`...")
    __efl_assert_locate_header(${__out})
    set(${__out} ${${__out}} CACHE PATH "Path to `assert.h`" FORCE)
  endif()
endmacro()

macro(__efl_assert_parse __dir)
  set(__EFL_PARSER_SCRIPT
    "${__dir}/ParseAssert.${__EFL_ASSERT_SCRIPT_EXT}")
  execute_process(COMMAND 
    ${__EFL_ASSERT_SCRIPT_EXE} 
      ${__EFL_PARSER_SCRIPT} ${__EFL_ASSERT_HEADER}
    WORKING_DIRECTORY ${__dir}
    OUTPUT_VARIABLE __EFL_ASSERT_DEFINITION
    ERROR_VARIABLE __EFL_ASSERT_DEFINITION_ERR
  )
  string(STRIP "${__EFL_ASSERT_DEFINITION}" __EFL_ASSERT_DEFINITION)
endmacro()