#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <tf/tf.h>
#include "lm_tello_controller/MyController.hpp"
#include "geometry_msgs/Twist.h"

// Position limits for OptiTrack
#define MIN_POS_X -1.6
#define MAX_POS_X 1.7
#define MIN_POS_Y -1.3
#define MAX_POS_Y 2.1
#define MIN_POS_Z 0.4
#define MAX_POS_Z 2.3
#define POS_MARGIN 0.4

namespace lm_tello_controller
{
  MyController::MyController(ros::NodeHandle &nodeHandle) :
      m_nodeHandle(&nodeHandle)
  {

    // Subscribe
    m_droneOptiTrackSubcriber =  m_nodeHandle->subscribe("/natnet_ros/lmro14_tello2/pose", 10, &MyController::droneOptiTrackCallback, this);

    // // Publishers to tello topics
    m_takeoff_pub = m_nodeHandle->advertise<std_msgs::Empty>("/tello/takeoff", 1);
    m_land_pub    = m_nodeHandle->advertise<std_msgs::Empty>("/tello/land", 1);
    m_vel_pub     = m_nodeHandle->advertise<geometry_msgs::Twist>("/tello/cmd_vel", 1);

    ROS_INFO("Init done.");

    ROS_INFO("Waiting 3 sec for init...");
    // Let publishers connect
    ros::Duration(3.0).sleep();
    ROS_INFO("Waiting done.");
  }

  MyController::~MyController()
  {
    const std_msgs::Empty emptyMsg;

    // Stop the drone
    ROS_INFO("Stopping for 1 seconds...");
    ros::Rate rate1(50); // 50 Hz
    for (int i = 0; i < 1*50; ++i)
    {
      geometry_msgs::Twist myVelMsg;
      m_vel_pub.publish(myVelMsg);
      ros::spinOnce();
      rate1.sleep();
    }

    ROS_INFO("Landing for 5 seconds...");
    ros::Rate rate2(10);
    for (int i = 0; i < 5*10; ++i)
    {
      m_land_pub.publish(emptyMsg);
      ros::spinOnce();
      rate2.sleep();
    }
  }

  // void MyController::angleLoop()
  // {
    
  // }

  

  void MyController::droneOptiTrackCallback(const geometry_msgs::PoseStamped &msg)
  {
    const geometry_msgs::Point pos = msg.pose.position;
    m_pos.x = pos.x; m_pos.y = pos.y; m_pos.z = pos.z;
    // ROS_INFO("X = ", pos.x);
    // ROS_INFO("Y = ", pos.y);
    // ROS_INFO("Z = ", pos.z);
    if (!(pos.x < MAX_POS_X-POS_MARGIN && pos.x > MIN_POS_X+POS_MARGIN 
     && pos.y < MAX_POS_Y-POS_MARGIN && pos.y > MIN_POS_Y+POS_MARGIN
     && pos.z < MAX_POS_Z-POS_MARGIN && pos.z > MIN_POS_Z+POS_MARGIN))
     {
      // ROS_INFO("OUTSIDE BOUNDARIES !!");
      m_inBounds = false;
     }
     else
     {
      //  ROS_INFO("INSIDE ...");
      m_inBounds = true;
     }


     const geometry_msgs::Quaternion quat = msg.pose.orientation;
    //  quat.

     tf::Quaternion q(
        quat.x,
        quat.y,
        quat.z,
        quat.w);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    m_yaw = -yaw;
    // ROS_INFO("\nroll  = %f\npitch = %f\nyaw   = %f", roll, pitch, yaw);
  }

} /* namespace */
