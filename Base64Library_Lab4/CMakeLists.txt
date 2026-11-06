# CMakeList.txt : CMake project for Base64Library_Lab4, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Base64Library_Lab4 "Base64Library_Lab4.cpp" "Base64Library_Lab4.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Base64Library_Lab4 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
