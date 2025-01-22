cmake_minimum_required(VERSION 3.6)
project(tello_msgs)

if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif ()

# Find packages
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

# Generate ROS interfaces
rosidl_generate_interfaces(
  ${PROJECT_NAME}
  "msg/FlightData.msg"
  "msg/TelloResponseState.msg"
  "srv/TelloCommunicationState.srv"
  DEPENDENCIES std_msgs
)

ament_package()
