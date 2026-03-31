#include <ros/ros.h>
#include "tourelle_controller/MyController.hpp"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "tourelle_controller_node");
  ros::NodeHandle nodeHandle("~");

  tourelle_controller::MyController myController(nodeHandle);

  ros::spin();
  return 0;
}
