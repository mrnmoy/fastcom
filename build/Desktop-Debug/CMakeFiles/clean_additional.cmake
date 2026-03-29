# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appfastcom_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appfastcom_autogen.dir/ParseCache.txt"
  "appfastcom_autogen"
  )
endif()
