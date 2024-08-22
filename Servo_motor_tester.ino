/* Servo_motor_tester.ino
 *
 * bav 20240822 Added sweep capability with a toggle switch.
 * bav 20240821 Want to combine sweep capabiltiy with pot input. 
  * Code copied here for reference.
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position
void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}
void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
} // end loop
 *
 * Also might want to extend 1000-2000 usec pwm. Check documentation.
 * Want to run this on an ATTiny85 useing ATTinyCore (Spence Konde) board manager.
 * bav 20240821 initial version
 * https://srituhobby.com/how-to-make-a-diy-servo-motor-tester-with-the-arduino/
 *
 *-----------------------------------
 *Servo motor tester.
 * https://srituhobby.com
 */

#include <Servo.h>

Servo servo;

#define testerPotInputPin A1  // attiny85 pin 7; ADC1
#define servoPWMOutputPin 0   // D0, attiny85 pin 5; PWM0
#define modePotOrSweepPin 1   // D1, attiny85 pin 6;
// attiny85 pins 2,3; D3,D4; reserved for USB+ and USB-
//          pin 1 reset / pin 8 VCC / pin 4 Gnd / pin 6 D1 unused.
//          Could use pin 6 for toggle switch to use pot or sweep servo.


void setup() {
  Serial.begin(9600);
  servo.attach(servoPWMOutputPin);
}
void loop() {
  static int value = 0;

  // Mode potentiometer input
  if (modePotOrSweepPin) {

    value = analogRead(testerPotInputPin);  // Pot values ccw 0; cw 1023
    value = map(value, 0, 1024, 0, 180);    // Servo class can accept input degrees. Check documentation.
  
  // Mode auto sweep input
  } else {                                  
    static bool cntUp = true;
    if (cntUp && value < 180) {
      value++;
    } else if (cntUp) {
      cntUp = false;
      value--;
    } else if (!cntUp && 0 < value) {
      value--;
    } else if (!cntUp) {
      cntUp = true;
      value++;
    }
  }

  Serial.println(value);
  servo.write(value);
  delay(18);  // RC frame rate is 50Hz or 20 millisecs approximately.
              // Max pulse duration is 2 msec. So 20 - 2 = ~18 msec.
}