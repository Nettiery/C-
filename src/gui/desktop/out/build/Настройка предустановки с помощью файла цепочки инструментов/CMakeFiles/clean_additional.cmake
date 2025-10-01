# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/desktop_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/desktop_autogen.dir/ParseCache.txt"
  "desktop_autogen"
  )
endif()
