#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <tf/tf.h>
#include "lm_tello_controller/MyController.hpp"
#define M_PI 3.14159265358979323846

int main(int argc, char **argv)
{
  ros::init(argc, argv, "lm_tello_controller_node");
  ros::NodeHandle nodeHandle("~");

  lm_tello_controller::MyController myController(nodeHandle);

  const std_msgs::Empty emptyMsg;

  ROS_INFO("Sending takeoff command...");
  ros::spinOnce();
  geometry_msgs::Point posBeforeTakeoff = myController.m_pos;
  // copyPoint(myController.m_pos, posBeforeTakeoff);
  myController.m_takeoff_pub.publish(emptyMsg);
  ros::spinOnce();

  // Wait while flying
  ROS_INFO("Waiting 5 seconds for takeoff...");
  ros::Duration(5.0).sleep();
  ros::spinOnce();
  geometry_msgs::Point posAfterTakeoff = myController.m_pos;;
  // copyPoint(myController.m_pos, posAfterTakeoff);

  ROS_INFO("\nposAfterTakeoff.z  = %f\nposBeforeTakeoff.z = %f", posAfterTakeoff.z, posBeforeTakeoff.z);

  if (posAfterTakeoff.z - posBeforeTakeoff.z < 0.3)
  {
    ROS_INFO("No takeoff detected ! (dif is %f)", posAfterTakeoff.z - posBeforeTakeoff.z);

    return EXIT_FAILURE;
  }

  
  ROS_INFO("Waiting for in bounds OptiTrack callback...");
  const ros::Time startTime = ros::Time::now();
  while (!myController.m_inBounds) {
    if (ros::Time::now() - startTime > ros::Duration(5.0))
    {
      ROS_INFO("Drone still not in bounds after 5 seconds !");
      return EXIT_FAILURE;
    }
    ros::spinOnce();
  };
  ROS_INFO("Done. Drone is in bounds.");

  {
    double turnDir = 1.0;
    for (size_t i = 0; i < 3; i++)
    {
      ROS_INFO("Turning to pi * %f ...", turnDir);
      const ros::Time startTime = ros::Time::now();
      while (ros::Time::now() - startTime < ros::Duration(6.0))
      {
        geometry_msgs::Twist myVelMsg;
        myVelMsg.angular.z = -(myController.m_yaw-M_PI*((turnDir+1)/2));
        myController.m_vel_pub.publish(myVelMsg);
        ros::spinOnce();
      }
      turnDir*=-1;
    }
  }

  // {
  //   ROS_INFO("Turning z negative...");
  //   const ros::Time startTime = ros::Time::now();
  //   while (ros::Time::now() - startTime < ros::Duration(10.0))
  //   {
  //     geometry_msgs::Twist myVelMsg;
  //     myVelMsg.angular.z = -1.;
  //     myController.m_vel_pub.publish(myVelMsg);
  //     ros::spinOnce();
  //   }
  // }

  return EXIT_SUCCESS;

  // double flightDir = 1.0;

  // for (size_t i = 0; i < 4; i++)
  // {
  //   if (i > 0)
  //   {
  //     ROS_INFO("Going straight 'til back in bounds...");
  //     while (!myController.m_inBounds)
  //     {
  //       geometry_msgs::Twist myVelMsg;
  //       myVelMsg.linear.x = flightDir*0.6;
  //       myController.m_vel_pub.publish(myVelMsg);
  //       ros::spinOnce();
  //     }

  //     ROS_INFO("Going straight for 1 more second...");
  //     const ros::Time startTime = ros::Time::now();
  //     while (ros::Time::now() - startTime < ros::Duration(1.0))
  //     {
  //       geometry_msgs::Twist myVelMsg;
  //       myVelMsg.linear.x = flightDir*0.6;
  //       myController.m_vel_pub.publish(myVelMsg);
  //       ros::spinOnce();
  //     }
  //   }

  //   ROS_INFO("Going straight 'til boundary...");
  //   while (myController.m_inBounds)
  //   {
  //     geometry_msgs::Twist myVelMsg;
  //     myVelMsg.linear.x = flightDir*0.6;
  //     myController.m_vel_pub.publish(myVelMsg);
  //     ros::spinOnce();
  //   }
  //   flightDir *= -1;
  //   ROS_INFO("Reached boundary, going the other way");
  // }

  // ROS_INFO("Going back for 2 second...");
  // const ros::Time startTime = ros::Time::now();
  // while (ros::Time::now() - startTime < ros::Duration(1.0))
  // {
  //   geometry_msgs::Twist myVelMsg;
  //   myVelMsg.linear.x = flightDir*0.6;
  //   myController.m_vel_pub.publish(myVelMsg);
  //   ros::spinOnce();
  // }

  // ROS_INFO("Done.");

  // return EXIT_SUCCESS;
}

