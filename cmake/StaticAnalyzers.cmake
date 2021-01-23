option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" OFF)
if(ENABLE_CPPCHECK)
find_program(CMAKE_CXX_CPPCHECK cppcheck)
  if(CMAKE_CXX_CPPCHECK)
    list(
      APPEND CMAKE_CXX_CPPCHECK 
      "--enable=all"
      "--inconclusive"
      "--force" 
      "--inline-suppr"
      )
  endif(CMAKE_CXX_CPPCHECK)
endif()

option(ENABLE_IWYU "Enable static analysis with include-what-you-use" OFF)
if(ENABLE_IWYU)
  find_program(INCLUDE_WHAT_YOU_USE include-what-you-use)
  if(INCLUDE_WHAT_YOU_USE)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
  else()
    message(SEND_ERROR "include-what-you-use requested but executable not found")
  endif()
endif()