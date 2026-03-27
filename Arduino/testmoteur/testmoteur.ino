/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo myservo;  // create Servo object to control a servo
Servo myservo2;  // create Servo object to control a servo
// twelve Servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  //myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  myservo2.attach(10);
  //myservo.attach(9, 450, 2450);  // attaches the servo on pin 9 to the Servo object
  //myservo.write(100);              // tell servo to go to position in variable 'pos'
  //delay(2000);
}

void loop() {
  // int speed = 10;
  // for (pos = 35; pos <= 178; pos += speed) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo2.write(pos);   
  //   //myservo2.write(180 - pos);           // tell servo to go to position in variable 'pos'
  //   delay(30);                       // waits 15 ms for the servo to reach the position
  // }
  // for (pos = 178; pos >= 35; pos -= speed) { // goes from 180 degrees to 0 degrees
  //   myservo2.write(pos);
  //   //myservo2.write(180 - pos);       // tell servo to go to position in variable 'pos'
  //   delay(30);                       // waits 15 ms for the servo to reach the position
  // }
  // myservo2.write(38); // En bas au MAX
  // delay(1000);
  myservo2.write(90); // Horizontal
  delay(30);
  // myservo2.write(175); // En haut au MAX
  // delay(1000);
}
