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

char buffer[200];
int pos1 = 90;    // variable to store the servo position
//int pos2 = 0;
int testpos;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the Servo object
  myservo2.attach(10);

  //COMMUNICATION
  // on démarre la liaison
  // en la réglant à une vitesse de 9600 bits par seconde.
  Serial.begin(9600);
}

void serialEvent() // déclaration de la fonction d'interruption sur la voie série
{
    // lit toutes les données (vide le buffer de réception)
    int last = Serial.readBytesUntil('\n', buffer, 4);
    while(last != 0){
      //READ
      buffer[last] = '\0';  // Null-terminate the string
      Serial.print("[Recieved]:"); //ARDUINO VERS PC
      Serial.println(buffer); //ARDUINO VERS PC

      //PARSE
      if (sscanf(buffer, "%d", &testpos)==1){
        Serial.print("[Parsed as int]:"); //ARDUINO VERS PC
        Serial.println(testpos); //ARDUINO VERS PC
      }
      else{
        Serial.println("Parsing error occured"); //ARDUINO VERS PC
        testpos = 0;
      }

      //TEST
      if (testpos < 35 || testpos > 178){
        Serial.print("[Warning]: int "); //ARDUINO VERS PC
        Serial.print(testpos); //ARDUINO VERS PC
        Serial.println(" is not in bounds, skipping..."); //ARDUINO VERS PC
      }
      else{
        //APPLY CHANGES
        pos1 = testpos;
      }

      // Fin de la boucle
      last = Serial.readBytesUntil('\n', buffer, 4);
    }
}

void loop() {
  //delay(1000);
  // Serial.print("Trying pos1 :");
  // Serial.println(pos1);
  myservo2.write(pos1); // En bas au MAX
}
