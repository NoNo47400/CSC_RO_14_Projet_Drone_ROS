/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#include <ros.h>
#include <tourelle_controller/TourelleAngles.h>
#include <std_msgs/String.h>
#include <Servo.h>

ros::NodeHandle nh;

Servo servoTop;
Servo servoMiddle;
Servo servoBottom;   // create Servo object to control a servo

float servoTopLowBound = 38;
float servoTopHighBound = 175;
float servoMiddleLowBound = 0;
float servoMiddleHighBound = 180;
float servoBottomLowBound = 0;
float servoBottomHighBound = 180;

std_msgs::String str_msg;
ros::Publisher debugger_publisher("debug_arduino", &str_msg);

void callBackFunction(const tourelle_controller::TourelleAngles &inputAngles)
{
  char text[20] = "[Recieved]:";
  str_msg.data = text;
  debugger_publisher.publish( &str_msg );

  dtostrf(inputAngles.angle_bottom, 6, 2, text);
  str_msg.data = text;
  debugger_publisher.publish( &str_msg );

  dtostrf(inputAngles.angle_middle, 6, 2, text);
  str_msg.data = text;
  debugger_publisher.publish( &str_msg );

  dtostrf(inputAngles.angle_top, 6, 2, text);
  str_msg.data = text;
  debugger_publisher.publish( &str_msg );

  if (inputAngles.angle_top >= servoTopLowBound && inputAngles.angle_top <= servoTopHighBound)
  {
    servoTop.write((int) inputAngles.angle_top);
  }

  if (inputAngles.angle_middle >= servoMiddleLowBound && inputAngles.angle_middle <= servoMiddleHighBound)
  {
    servoMiddle.write((int) inputAngles.angle_middle);
  }

  if (inputAngles.angle_bottom >= servoBottomLowBound && inputAngles.angle_bottom <= servoBottomHighBound )
  {
    servoBottom.write((int) inputAngles.angle_bottom);
  }
}

ros::Subscriber<tourelle_controller::TourelleAngles> angles_subscriber("/angles", &callBackFunction);

void setup()
{
  servoTop.attach(11);
  servoMiddle.attach(10);
  servoBottom.attach(9);  // attaches the servo on pin 9 to the Servo object

  nh.initNode();
  nh.advertise(debugger_publisher);
  nh.subscribe(angles_subscriber);
}

void loop()
{
  nh.spinOnce();
  delay(20);
}
