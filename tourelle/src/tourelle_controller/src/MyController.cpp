#include "tourelle_controller/MyController.hpp"
#include <iostream>
#include <cmath>
namespace tourelle_controller
{
  MyController::MyController(ros::NodeHandle &nodeHandle) :
      m_nodeHandle(&nodeHandle)
  {
    std::string droneTopicName;
    if (!m_nodeHandle->getParam("drone_topic_name", droneTopicName))
    {
      ROS_ERROR("Could not read drone_topic_name.");
    }
    std::string selfTopicName;
    if (!m_nodeHandle->getParam("self_topic_name", selfTopicName))
    {
      ROS_ERROR("Could not read self_topic_name.");
    }

    m_currentCommandBot = 90;
    m_currentCommandMiddle = 90;

    //TODO : Rename topics
    m_dronePointSubscriber = m_nodeHandle->subscribe(droneTopicName, 10, &MyController::dronePointCallback, this);
    m_selfPoseSubscriber = m_nodeHandle->subscribe(selfTopicName, 10, &MyController::selfPoseCallback, this);
    m_tourelleCommandPublisher = m_nodeHandle->advertise<tourelle_controller::TourelleAngles>("/angles", 10);
    ROS_INFO("Successfully launched node.");

    tourelle_controller::TourelleAngles commande;
    commande.angle_bottom = 90;
    commande.angle_middle = 90;
    commande.angle_top = 90;      
    m_tourelleCommandPublisher.publish(commande);
  }

  void MyController::dronePointCallback(const geometry_msgs::Point &msg)
  {
    /*
    On suppose que le moteur du bas et du milieu vont de 0° à 180°
    Le moteur du haut est limité entre 40° et 175°    
    */
    tourelle_controller::TourelleAngles commande;

    double deltax = msg.x - m_pose.position.x;
    double deltay = msg.y - m_pose.position.y;
    double deltaz = msg.z - m_pose.position.z - m_height;

    //Calcul de la commande en yaw
      double goal_yaw = atan2(deltay,deltax) * (180.0/3.141592653589793238463); // Angle selon z pour les deux moteurs du bas

      //Ajouter la connaissance du yaw actuel
      double current_yaw = - m_currentCommandBot - m_currentCommandMiddle + 180; // Dans [-180°, 180°]
      double delta_command_yaw = current_yaw - goal_yaw;

      // Normalisation dans [-180, 180]
      delta_command_yaw = std::fmod(delta_command_yaw + 180.0, 360.0);
      if (delta_command_yaw < 0)
          delta_command_yaw += 360.0;
      delta_command_yaw -= 180.0;
      
      double command_yaw_bottom;
      double command_yaw_middle;

      //On maximise d'abord le moteur du bas
      if (delta_command_yaw > 0) // Sens horaire
      {
        if (m_currentCommandBot + delta_command_yaw > 180) 
        //Si on NE PEUT PAS résoudre le problème avec un seulement le moteur bas
        {
          delta_command_yaw = delta_command_yaw - m_currentCommandBot; // On calcule ce qui dépasse
          command_yaw_bottom = 180; // On met ce qu'on peut dans le bas
          command_yaw_middle = m_currentCommandMiddle + delta_command_yaw; // Et le reste dans l'autre
        }
        else
        //Si on PEUT résoudre le problème avec seulement le moteur bas
        {
          command_yaw_bottom = m_currentCommandBot + delta_command_yaw; // On met tout dans le bas
          command_yaw_middle = m_currentCommandMiddle; // On ne change pas le middle
        }
      }
      else // Sens anti-horaire
      {
        if (m_currentCommandBot + delta_command_yaw < 0) 
        //Si on NE PEUT PAS résoudre le problème avec un seulement le moteur bas
        {
          delta_command_yaw = delta_command_yaw + m_currentCommandBot; // On calcule ce qui dépasse
          command_yaw_bottom = 0; // On met ce qu'on peut dans le bas
          command_yaw_middle = m_currentCommandMiddle + delta_command_yaw; // Et le reste dans l'autre
        }
        else
        //Si on PEUT résoudre le problème avec seulement le moteur bas
        {
          command_yaw_bottom = m_currentCommandBot + delta_command_yaw; // On met tout dans le bas
          command_yaw_middle = m_currentCommandMiddle; // On ne change pas le middle
        }
      }
    
    //Calcul de la commande en pitch
      double dist = sqrt(deltax*deltax + deltay*deltay);
      double goal_pitch = atan2(deltaz, dist) * (180.0/3.141592653589793238463); // Angle pour le moteur du haut

      double command_pitch = goal_pitch + 90;
    
    //Ajout dans la commande //TODO implementer la commande
      commande.angle_bottom = command_yaw_bottom; //Augmentation = sens horaire
      commande.angle_middle = command_yaw_middle; //Augmentation = sens horaire
      commande.angle_top = command_pitch; //Augmentation = vers le haut

    m_tourelleCommandPublisher.publish(commande);

    ROS_INFO_STREAM("Command sent"<< std::endl);
  }

  void MyController::selfPoseCallback(const geometry_msgs::PoseStamped &msg)
  {
    m_pose = msg.pose; // On récupère juste la pose
  }

} /* namespace */
