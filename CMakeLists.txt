cmake_minimum_required(VERSION 3.11)
project(lycam-c)

set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  add_definitions(-DDEBUG)
  add_definitions(-D_DEBUG)
endif()

if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#
# add_gmock_test(<target> <sources>...)
#
#  Adds a Google Mock based test executable, <target>, built from <sources> and
#  adds the test so that CTest will run it. Both the executable and the test
#  will be named <target>.
#
function(add_lym_test target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} lycam)
  add_test(${target} ${target})

  if(UNIX)
    add_custom_command(TARGET ${target}
      POST_BUILD
      COMMAND ${target}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Running ${target}" VERBATIM)
  endif()
endfunction()

add_subdirectory(src)
