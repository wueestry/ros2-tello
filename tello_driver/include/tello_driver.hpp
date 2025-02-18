#include <asio.hpp>
#include <asio/io_service.hpp>
#include <cstdint>
#include <memory>

#include "cv_bridge/cv_bridge.h"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include "tello_msgs/msg/flight_data.hpp"
#include "tello_msgs/msg/tello_response_state.hpp"
#include "tello_msgs/srv/tello_communication_state.hpp"

namespace tello_driver {
class CommandSocket;

class StateSocket;

class VideoSocket;

class TelloDriverNode : public rclcpp::Node {
public:
  explicit TelloDriverNode(const rclcpp::NodeOpptions &options); // Constructor

  ~TelloDriverNode();

  // ROS publishers
  rclcpp::Publisher<sensor_msgs::msgs::Image>::SharedPtr image_pub_;
  rclcpp::Publisher<sensor_msgs::msgs::CameraInfo>::SharedPtr camera_info_pub_;
  rclcpp::Publisher<tello_msgs::msgs::FlightData>::SharedPtr flight_data_pub_;
  rclcpp::Publisher<tello_msgs::msgs::TelloResponseState>::SharedPtr
      tello_response_state_pub_;

private:
  void timer_callback(); // ROS timer callback

  void command_callback(
      const std::shared_ptr<rmw_request_id> request_header,
      const std::shared_ptr<tello_msgs::srv::TelloCommunicationState::Request>
          request,
      std::shared_ptr<tello_msgs::srv::TelloCommunicationState::Response>
          response); // ROS command callback

  void cmd_vel_callback(
      const geometry_msgs::msg::Twist::SharedPtr msg); // ROS velocity callback

  // Tello sockets
  std::unique_ptr<CommandSocket> cmd_socket_;
  std::unique_ptr<StateSocket> state_socket_;
  std::unique_ptr<VideoSocket> video_socket_;

  rclcpp::Service<tello_msgs::srv::TelloCommunicationState>::Shared_Ptr
      command_srv_; // ROS command service

  rclcpp::Subscription<geometry_msgs::msg::Twist>::Shared_Ptr
      cmd_vel_sub_; // ROS velocity subscriber

  rclcpp::TimerBase::SharedPtr timer_; // ROS timer
};

class TelloSocket {
public:
  TelloSocket(TelloDriverNode *driver, unsigned short port)
      : driver_(driver),
        socket_(io_service_, asio::ip::udp::endpoint(asio::ip::udp::v4, port)) {
  } // Constructor

  bool receiving();

  rclcpp::Time receive_time();

  virtual void timout();

protected:
  void listen();

  virtual void process_packet(size_t r) = 0;

  TelloDriverNode *driver_; // Pointer to driver node

  asio::io_service io_service_; // IO service manager

  asio::ip::udp::socket socket_; // Socket

  std::thread thread_; // Thread of socket

  std::mutex mtx_; // Mutex to guard public calls

  bool receiving_ = false; // Receiving status

  rclcpp::Time receive_time_; // Time of latest receiving packet

  std::vector<unsigned char> buf_; // Buffer to store packets
};

class CommandSocket : public TelloSocket { // Inherits TelloSocket class
public:
  CommandSocket(TelloDriverNode *driver, std::string ip, unsigned short port,
                unsigned short command_port); // Constructor

  void timeout() override;

  bool waiting();

  rclcpp::Time send_time();

  void initiate_command(std::string cmd, bool responds);

private:
  void process_packet(size_t r) override;

  void complete_command(uint8_t rc, std::string str);

  asio::ip::udp::endpoint remote_endpoint_;

  rclcpp::Time send_time_; // Time of latest packet sent

  bool responding_; // Responding to tello_response_pub_

  bool waiting_ = false; // Waiting for response
};

class StateSocket : public TelloSocket {
public:
  StateSocket(TelloDriverNode *driver, unsigned short data_port); // Constructor

private:
  void process_packet(size_t r) override; // Process packets

  uint8_t sdk_ = tello_msgs::msgs::FlightData::SDK_UNKNOWN; // Tello SDK version
};

class VideoSocket : public TelloSocket {
public:
  VideoSocket(TelloDriverNode *driver, unsigned short video_port,
              const std::string &camera_info_path); // Constructor

private:
  void process_packet(size_t r) override; // Process packets

  void decode_frames(); // Frame decoding

  std::vector<unsigned char> seq_buffer_; // Buffer for video packets

  size_t seq_buffer_next_ = 0; // Next free location in sequence buffer

  int seq_buffer_num_packets_ = 0; // Number of packets collected

  H264Decoder decoder_; // H264 decoder

  ConverterRGB24 converter_; // RGB converter

  sensor_msgs::msg::CameraInfo camera_info_msg_; // Camera info message
};
} // namespace tello_driver
