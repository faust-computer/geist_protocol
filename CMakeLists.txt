cmake_minimum_required(VERSION 3.8)
project(geist_protocol)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# Set the path to the nanopb root folder
# Read more here:
# - https://github.com/nanopb/nanopb/issues/481
# - https://github.com/nanopb/nanopb/issues/785
set(NANOPB_SRC_ROOT_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanopb")
message(STATUS "NANOPB_SRC_ROOT_FOLDER: ${NANOPB_SRC_ROOT_FOLDER}")

# Modify the find_path command to be more specific
find_path(NANOPB_GENERATOR_SOURCE_DIR
    NAMES nanopb_generator.py
    PATHS ${NANOPB_SRC_ROOT_FOLDER}/generator
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH
)

# Use the set path in CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${NANOPB_SRC_ROOT_FOLDER}/extra)
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

find_package(Nanopb MODULE REQUIRED)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

# generate the nanopb files from the .proto files
NANOPB_GENERATE_CPP(PROTO_SRCS PROTO_HDRS RELPATH protos
  protos/simple.proto
  protos/request.proto
  protos/response.proto
)

# Set the include directories
include_directories(
  include
  ${CMAKE_CURRENT_BINARY_DIR}
  ${NANOPB_INCLUDE_DIRS}
)

# Set the library target
add_library(${PROJECT_NAME} SHARED
  ${PROTO_HDRS}
  ${PROTO_SRCS}
  src/uart.cpp
  src/nanopb.cpp
)

ament_export_targets(${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_target_dependencies(${PROJECT_NAME} rclcpp std_msgs)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

# Install the library header files
install(
  DIRECTORY include/
  DESTINATION include
)

# Install the Nanopb-generated header files
install(
  FILES ${PROTO_HDRS}
  DESTINATION include
)

# Install the CMake configuration files
ament_package()