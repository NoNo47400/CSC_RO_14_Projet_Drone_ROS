#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <tf/tf.h>
#include "lm_tello_controller/MyController.hpp"

// void copyPoint(geometry_msgs::Point fromPoint, geometry_msgs::Point toPoint)
// {
//   toPoint.x = fromPoint.x;
//   toPoint.y = fromPoint.y;
//   toPoint.z = fromPoint.z;
// }

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

  if (false && posAfterTakeoff.z - posBeforeTakeoff.z < 0.3)
  {
    ROS_INFO("No takeoff detected ! (dif is %f)", posAfterTakeoff.z - posBeforeTakeoff.z);

    return EXIT_FAILURE;
  }

  
  ROS_INFO("Waiting for in bounds OptiTrack callback...");
  while (!myController.m_inBounds) { ros::spinOnce(); };
  ROS_INFO("Done. Drone is in bounds.");

  {
    const ros::Time startTime = ros::Time::now();
    while (ros::Time::now() - startTime < ros::Duration(10.0))
    {
      geometry_msgs::Twist myVelMsg;
      myVelMsg.angular.z = 0.05;
      myController.m_vel_pub.publish(myVelMsg);
      ros::spinOnce();
    }
  }

  {
    const ros::Time startTime = ros::Time::now();
    while (ros::Time::now() - startTime < ros::Duration(10.0))
    {
      geometry_msgs::Twist myVelMsg;
      myVelMsg.angular.z = -0.05;
      myController.m_vel_pub.publish(myVelMsg);
      ros::spinOnce();
    }
  }

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
  //       myVelMsg.linear.x = flightDir*0.7;
  //       myController.m_vel_pub.publish(myVelMsg);
  //       ros::spinOnce();
  //     }

  //     ROS_INFO("Going straight for 1 more second...");
  //     const ros::Time startTime = ros::Time::now();
  //     while (ros::Time::now() - startTime < ros::Duration(1.0))
  //     {
  //       geometry_msgs::Twist myVelMsg;
  //       myVelMsg.linear.x = flightDir*0.7;
  //       myController.m_vel_pub.publish(myVelMsg);
  //       ros::spinOnce();
  //     }
  //   }

  //   ROS_INFO("Going straight 'til boundary...");
  //   while (myController.m_inBounds)
  //   {
  //     geometry_msgs::Twist myVelMsg;
  //     myVelMsg.linear.x = flightDir*0.7;
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
  //   myVelMsg.linear.x = flightDir*0.7;
  //   myController.m_vel_pub.publish(myVelMsg);
  //   ros::spinOnce();
  // }

  // // Stop the drone
  // geometry_msgs::Twist myVelMsg;
  // myController.m_vel_pub.publish(myVelMsg);
  // ros::spinOnce();

  // ros::Duration(1.0).sleep();


  // ROS_INFO("Landing...");
  // myController.m_land_pub.publish(emptyMsg);
  // ros::spinOnce();
  // ros::Duration(1.0).sleep();


  // ROS_INFO("Done.");

  // return EXIT_SUCCESS;
}

