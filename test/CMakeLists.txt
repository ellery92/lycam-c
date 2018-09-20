#
# add_gmock_test(<target> <sources>...)
#
#  Adds a Google Mock based test executable, <target>, built from <sources> and
#  adds the test so that CTest will run it. Both the executable and the test
#  will be named <target>.
#
function(add_lym_test target)
  add_executable(${target} ${ARGN})
  target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../src)
  target_link_libraries(${target} lycam-c)
  add_test(${target} ${target})

  if(UNIX)
    add_custom_command(TARGET ${target}
      POST_BUILD
      COMMAND ${target}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Running ${target}" VERBATIM)
  endif()
endfunction()

# add_lym_test(arvtool arvtool.c)
