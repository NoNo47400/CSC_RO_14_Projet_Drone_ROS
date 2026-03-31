#pragma once

#include <ros/ros.h>
#include "tourelle_controller/TourelleAngles.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"

namespace tourelle_controller
{

/*!
 * Class containing the tourelle Controller
 */
class MyController
 {
public:
    /*!
     * Constructor.
     */
    MyController(ros::NodeHandle &nodeHandle);

private:
    ros::NodeHandle* m_nodeHandle;
    ros::Publisher m_tourelleCommandPublisher;
    ros::Subscriber m_dronePointSubscriber;
    ros::Subscriber m_selfPoseSubscriber;
    geometry_msgs::Pose m_pose;
    void dronePointCallback(const geometry_msgs::Point &msg);
    void selfPoseCallback(const geometry_msgs::PoseStamped &msg);
    const double m_height = 0.15;
    double m_currentCommandBot;
    double m_currentCommandMiddle;
};

} /* namespace */
