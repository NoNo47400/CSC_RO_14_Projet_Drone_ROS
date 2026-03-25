#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include "lm_tello_controller/MyController.hpp"

// Position limits for OptiTrack
#define MIN_POS_X -1.6
#define MAX_POS_X 1.7
#define MIN_POS_Y -1.3
#define MAX_POS_Y 2.1
#define MIN_POS_Z 0.5
#define MAX_POS_Z 2.3
#define POS_MARGIN 0.3

// int main(int argc, char** argv)
// {
//     ros::init(argc, argv, "lm_tello_controller_node");
//     ros::NodeHandle nh;

//     // Subscribe
//     nh.subscribe("/natnet_ros/lmro14_flat_marker_pose", 10, dronePointCallback);

//     // Publishers to tello topics
//     ros::Publisher takeoff_pub = nh.advertise<std_msgs::Empty>("/tello/takeoff", 1);
//     ros::Publisher land_pub    = nh.advertise<std_msgs::Empty>("/tello/land", 1);

//     // Let publishers connect
//     ros::Duration(3.0).sleep();

//     std_msgs::Empty msg;

//     ROS_INFO("Taking off and waiting for 5 seconds...");
//     takeoff_pub.publish(msg);

//     // Wait while flying
//     ros::Duration(5.0).sleep();

//     ROS_INFO("Landing...");
//     land_pub.publish(msg);

//     ROS_INFO("Done.");

//     return 0;
// }


namespace lm_tello_controller
{
  MyController::MyController(ros::NodeHandle &nodeHandle) :
      m_nodeHandle(&nodeHandle)
  {

    // Subscribe
    m_droneOptiTrackSubcriber =  m_nodeHandle->subscribe("/natnet_ros/lmro14_flat_marker/pose", 10, &MyController::droneOptiTrackCallback, this);

    // // Publishers to tello topics
    // ros::Publisher takeoff_pub = m_nodeHandle->advertise<std_msgs::Empty>("/tello/takeoff", 1);
    // ros::Publisher land_pub    = m_nodeHandle->advertise<std_msgs::Empty>("/tello/land", 1);

    ROS_INFO("Init done.");

    // // Let publishers connect
    // ros::Duration(3.0).sleep();

    // std_msgs::Empty msg;

    // ROS_INFO("Taking off and waiting for 5 seconds...");
    // takeoff_pub.publish(msg);

    // // Wait while flying
    // ros::Duration(5.0).sleep();

    // ROS_INFO("Landing...");
    // land_pub.publish(msg);

    // ROS_INFO("Done.");
  }

  void MyController::droneOptiTrackCallback(const geometry_msgs::PoseStamped &msg)
  {
    geometry_msgs::Point pos = msg.pose.position;
    // ROS_INFO("X = ", pos.x);
    // ROS_INFO("Y = ", pos.y);
    // ROS_INFO("Z = ", pos.z);
    if (!(pos.x < MAX_POS_X-POS_MARGIN && pos.x > MIN_POS_X+POS_MARGIN 
     && pos.y < MAX_POS_Y-POS_MARGIN && pos.y > MIN_POS_Y+POS_MARGIN
     && pos.z < MAX_POS_Z-POS_MARGIN && pos.z > MIN_POS_Z+POS_MARGIN))
     {
      ROS_INFO("OUTSIDE BOUNDARIES !!");
     }
     else
      ROS_INFO("INSIDE ...");

  }

} /* namespace */
