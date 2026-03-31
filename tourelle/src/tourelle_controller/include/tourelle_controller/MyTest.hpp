#pragma once

#include <ros/ros.h>
#include "geometry_msgs/Point.h"

namespace tourelle_test
{

/*!
 * Class containing the tourelle Test
 */
class MyTest
 {
public:
    /*!
     * Constructor.
     */
    MyTest(ros::NodeHandle &nodeHandle);
    void loop();

private:
    ros::NodeHandle* m_nodeHandle;
    ros::Publisher m_tourelleTestPublisher;
    double m_currentAngle;
};

} /* namespace */
