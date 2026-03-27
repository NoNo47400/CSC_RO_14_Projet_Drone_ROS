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

    ~MyController();

    ros::Publisher m_takeoff_pub;
    ros::Publisher m_land_pub;
    ros::Publisher m_vel_pub;


    // void main();
    // void angleLoop();

    bool m_inBounds = false;
    geometry_msgs::Point m_pos;

private:
    ros::NodeHandle* m_nodeHandle;
    void droneOptiTrackCallback(const geometry_msgs::PoseStamped &msg);
    ros::Subscriber m_droneOptiTrackSubcriber;
};

} /* namespace */
