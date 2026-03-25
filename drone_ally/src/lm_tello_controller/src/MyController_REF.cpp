#include "tourelle_controller/MyController.hpp"
#include <iostream>
#include <cmath>
namespace tourelle_controller
{
  MyController::MyController(ros::NodeHandle &nodeHandle) :
      m_nodeHandle(&nodeHandle)
  {
    //TODO : Rename topics
    m_dronePointSubscriber = m_nodeHandle->subscribe("/front/scan", 10, &MyController::dronePointCallback, this);
    m_selfPoseSubscriber = m_nodeHandle->subscribe("/front/scan", 10, &MyController::selfPoseCallback, this);
    m_tourelleCommandPublisher = m_nodeHandle->advertise<tourelle_controller::TourelleAngles>("/angles", 10);
    ROS_INFO("Successfully launched node.");
  }

  void MyController::dronePointCallback(const geometry_msgs::Point &msg)
  {
    tourelle_controller::TourelleAngles commande;

    //Calcul

    m_tourelleCommandPublisher.publish(commande);

    ROS_INFO_STREAM("Command sent"<< std::endl);
  }

  void MyController::selfPoseCallback(const geometry_msgs::PoseStamped &msg)
  {
    m_pose = msg.pose; // On récupère juste la pose
  }


} /* namespace */
