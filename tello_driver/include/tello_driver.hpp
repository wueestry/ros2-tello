#include <asio.hpp>

#include "rclcpp/rclcpp.h"
#include "cv_bridge/cv_bridge.h"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include "tello_msgs/msg/flight_data.hpp"
#include "tello_msgs/msg/tello_response_state.hpp"
#include "tello_msgs/srv/tello_communication_state.hpp"

namespace tello_driver {
  class CommandSocket;

  class StateSocket;

  class VideoSocket;
}
