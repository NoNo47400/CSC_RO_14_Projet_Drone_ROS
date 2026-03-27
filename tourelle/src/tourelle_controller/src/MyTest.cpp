#include "tourelle_controller/MyTest.hpp"
#include <iostream>
#include <cmath>
namespace tourelle_test
{
  MyTest::MyTest(ros::NodeHandle &nodeHandle) :
      m_nodeHandle(&nodeHandle)
  {
    std::string droneTopicName;
    if (!m_nodeHandle->getParam("drone_topic_name", droneTopicName))
    {
      ROS_ERROR("Could not read drone_topic_name.");
    }

    m_currentAngle = 0;    

    m_tourelleTestPublisher = m_nodeHandle->advertise<geometry_msgs::Point>(droneTopicName, 10);
    ROS_INFO("Successfully launched node.");
  }

  void MyTest::loop()
  {
    double rayon = 1;
    geometry_msgs::Point point;
    point.x = rayon * cos(m_currentAngle / (180.0/3.141592653589793238463));
    point.y = rayon * sin(m_currentAngle / (180.0/3.141592653589793238463));
    point.z = 1;//3 * m_currentAngle/360 - 1;
    m_currentAngle -= 0.5;
    m_currentAngle = std::fmod(m_currentAngle, 360);
    m_tourelleTestPublisher.publish(point);
  }

} /* namespace */
