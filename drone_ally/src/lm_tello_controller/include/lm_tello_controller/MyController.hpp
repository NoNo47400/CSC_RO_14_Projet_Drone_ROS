#pragma once

#include <ros/ros.h>
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"

namespace lm_tello_controller
{
class MyController
 {
public:
    /*!
     * Constructor.
     */
    MyController(ros::NodeHandle &nodeHandle);

private:
    ros::NodeHandle* m_nodeHandle;
    // ros::Publisher m_tourelleCommandPublisher;
    ros::Subscriber m_droneOptiTrackSubcriber;
    // ros::Subscriber m_selfPoseSubscriber;
    // geometry_msgs::Pose m_pose;
    // void dronePointCallback(const geometry_msgs::Point &msg);
    // void selfPoseCallback(const geometry_msgs::PoseStamped &msg);


    ros::Publisher m_takeoff_pub;
    ros::Publisher m_land_pub;
    ros::Publisher m_vel_pub;

    void droneOptiTrackCallback(const geometry_msgs::PoseStamped &msg);

    void main();

    volatile bool m_inBounds = false;
};

} /* namespace */
