#include <ros/ros.h>
#include "lm_tello_controller/MyController.hpp"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "lm_tello_controller_node");
  ros::NodeHandle nodeHandle("~");

  lm_tello_controller::MyController myController(nodeHandle);

  ros::spin();
  return 0;
}

