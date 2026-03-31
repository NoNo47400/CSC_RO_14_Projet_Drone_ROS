#include <ros/ros.h>
#include "tourelle_controller/MyTest.hpp"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "tourelle_test_circle_node");
  ros::NodeHandle nodeHandle("~");

  tourelle_test::MyTest myTest(nodeHandle);

  ros::Rate rate(50); // 50 Hz    

  while(true){
    myTest.loop();
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}
