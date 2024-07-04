# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\reading_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\reading_autogen.dir\\ParseCache.txt"
  "reading_autogen"
  )
endif()
