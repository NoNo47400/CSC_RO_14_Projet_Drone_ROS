#!/usr/bin/env python3

import rospy
import cv2
import cv2.aruco as aruco
import numpy as np
from sensor_msgs.msg import Image, CameraInfo
from geometry_msgs.msg import Twist, Point
from std_msgs.msg import Empty
from cv_bridge import CvBridge, CvBridgeError
from std_msgs.msg import Empty

class ArucoFollower:
    def __init__(self):
        rospy.init_node('aruco_follower_node')
        self.bridge = CvBridge()
        
        self.cmd_vel_pub = rospy.Publisher('/tello/cmd_vel', Twist, queue_size=10)
        self.image_sub = rospy.Subscriber('/tello/camera/image_raw', Image, self.image_callback)
        self.camera_info_pub = rospy.Publisher('/tello/camera_info', CameraInfo, queue_size=1)
        self.takeoff_pub = rospy.Publisher('/tello/takeoff', Empty, queue_size=10)
        self.ennemy_position_pub = rospy.Publisher('/ennemy_position', Point, queue_size=10) # Publish the position of the tag for the agent to use
        
        rospy.sleep(1.0)

        # For the tag
        ## Need to generate the tag from DICT_4X4_50 and with the id 42
        # OpenCV ArUco API differs across versions; support both old and new names.
        if hasattr(aruco, "getPredefinedDictionary"):
            self.aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_4X4_50)
        else:
            self.aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_50)

        if hasattr(aruco, "DetectorParameters"):
            self.aruco_params = aruco.DetectorParameters()
        else:
            self.aruco_params = aruco.DetectorParameters_create()

        self.aruco_detector = None
        if hasattr(aruco, "ArucoDetector"):
            self.aruco_detector = aruco.ArucoDetector(self.aruco_dict, self.aruco_params)
            print("ArucoDetector is not None")
        
        self.target_id = 42           
        self.marker_size = 0.10       # Real world size of the tag to measure in meters (here 10cm)
        self.target_distance = 1.0    # The drone should stay exactly 1.0 meter away from the tag
        
        # Camera parameters
        height = 720
        width = 960

        focal_length = width
        center_x = width/2
        center_y = height/2
        self.camera_matrix = np.array([[focal_length, 0, center_x],
                                       [0, focal_length, center_y],
                                       [0, 0, 1]], dtype=float)
        self.dist_coeffs = np.zeros((4,1)) # Assume no lens distortion for simplicity

        self.camera_info_pub.publish(CameraInfo(height=height, width=width, K=self.camera_matrix.flatten(), D=self.dist_coeffs.flatten()))

        # PID gains for control (need to be tuned after)
        self.kp_x = 0.5   # Forward/Backward gain
        self.kp_y = 10   # Up/Down gain
        self.kp_yaw = 10 # Rotation gain
        
        # Maximum speed limit
        self.max_speed = 0.4 
        
        self.velocity_msg = Twist()
        rospy.loginfo("ArUco Follower Node Started. Looking for Tag ID: {}".format(self.target_id))

    def limit_speed(self, value):
        return max(min(value, self.max_speed), -self.max_speed)

    def image_callback(self, data):
        print("In Image Callback")
        try:
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
            rospy.logerr(e)
            return

        gray = cv2.cvtColor(cv_image, cv2.COLOR_BGR2GRAY)
        
        # Detect the marker
        if self.aruco_detector is not None:
            corners, ids, rejected = self.aruco_detector.detectMarkers(gray)
        else:
            corners, ids, rejected = aruco.detectMarkers(gray, self.aruco_dict, parameters=self.aruco_params)
        
        marker_found = False
        
        print(f"ids : {ids}")

        if ids is not None:
            for i in range(len(ids)): # Normally there will be only one marker, but in case
                if ids[i][0] == self.target_id:
                    marker_found = True
                    
                    # Estimate 3D Pose of the target marker with rotation vector and translation vector 
                    rvec, tvec, _ = aruco.estimatePoseSingleMarkers(corners[i], self.marker_size, 
                                                                    self.camera_matrix, self.dist_coeffs)
                    
                    # tvec format: [[ [x, y, z] ]] in meters
                    x_cam = tvec[0][0][0] # Left/Right (Negative is left, Positive is right)
                    y_cam = tvec[0][0][1] # Up/Down (Negative is up, Positive is down)
                    z_cam = tvec[0][0][2] # Forward/Backward (Distance from camera)

                    # Publish marker position for tourelle.
                    marker_position = Point()
                    marker_position.x = x_cam
                    marker_position.y = y_cam
                    marker_position.z = z_cam
                    self.ennemy_position_pub.publish(marker_position)
                    
                    # Draw axes on the image for debugging
                    try:
                        # Try new OpenCV API first (4.7+)
                        cv2.drawFrameAxes(cv_image, self.camera_matrix, self.dist_coeffs, rvec[0], tvec[0], 0.1)
                    except AttributeError:
                        # Fallback to older API if available
                        try:
                            aruco.drawAxis(cv_image, self.camera_matrix, self.dist_coeffs, rvec[0], tvec[0], 0.1)
                        except AttributeError:
                            pass  # Skip drawing if function not available
                    
                    try:
                        aruco.drawDetectedMarkers(cv_image, corners)
                    except AttributeError:
                        pass  # Skip if function not available
                    
                    # Calculate Errors
                    error_forward = z_cam - self.target_distance
                    error_updown = y_cam       # Target is center of screen (y = 0)
                    error_leftright = x_cam    # Target is center of screen (x = 0)
                    
                    # Preparing cmd vel
                    self.velocity_msg.linear.y = self.limit_speed(error_forward * self.kp_x)
                    self.velocity_msg.linear.z = self.limit_speed(-error_updown * self.kp_y)
                    self.velocity_msg.angular.z = self.limit_speed(error_leftright * self.kp_yaw)
                    print(f"lin_x:{self.velocity_msg.linear.x}, lin_z:{self.velocity_msg.linear.z}, ang_z:{self.velocity_msg.angular.x}")
                    # No side movement
                    self.velocity_msg.linear.x = 0.0 
                    
                    # Publish the movement command
                    self.cmd_vel_pub.publish(self.velocity_msg)
                    
                    # Print debug info
                    rospy.loginfo("Tag found! Z(dist): {:.2f}m | X_err: {:.2f}m | Y_err: {:.2f}m".format(z_cam, error_leftright, error_updown))
                    break # Stop looking at other tags once we found the target
        
        # If the target tag was not found in this frame, hover in place
        if not marker_found:
            self.velocity_msg.linear.x = 0.0
            self.velocity_msg.linear.y = 0.0
            self.velocity_msg.linear.z = 0.0
            self.velocity_msg.angular.z = 0.0
            self.cmd_vel_pub.publish(self.velocity_msg)
            
        # Display the video feed
        cv2.imshow("ArUco Tracker", cv_image)
        cv2.waitKey(1)

if __name__ == '__main__':
    try:
        follower = ArucoFollower()
        print("Taking off...")
        follower.takeoff_pub.publish()
        rospy.spin()
    except rospy.ROSInterruptException:
        cv2.destroyAllWindows()