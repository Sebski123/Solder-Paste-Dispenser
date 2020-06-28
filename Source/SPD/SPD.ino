/* Project       : Solder Paste dispenser
 * Code by       : LKBrilliant
 * Date          : 26.08.2018
 * Components    : 24BYJ-48 5V unipolar stepper motor
 *                 ATtiny85 microcontroller
 *                 ULN2003A Stepper driver
 *                
 * Description : > By pressing the external button once, the motor will turn 
 *                 preset number of steps. 
 *               > The screw mechanism will convert the rotational motion into linear 
 *                 motion thus compressing the plunge of the Syringe. 
 *               > changing the "forwardStp" and "idleBkStp", the amount of solder coming 
 *                 out of the container can be controlled.
 *               > Forward and backward direction may vary due to different wiring 
 *                 configurations of the stepper motor
 */

#include <Stepper.h>

const int stepsPerRevolution = 2048;

// Connections between Attiny85 and ULN2003A
#define in1 0
#define in2 1
#define in3 2
#define in4 3

#define forwardStp   2       // Number of steps per button press
#define idleBkStp   200      

Stepper myStepper = Stepper(stepsPerRevolution, in2, in4, in1, in3);

long t = 0;
bool backed = false;

void setup() {
  pinMode(4, INPUT);
  myStepper.setSpeed(1); 
  myStepper.step(5);
  motor_stop();           // Stepper off
  
}

void loop() {
  if(digitalRead(4) == HIGH){
    myStepper.step(forwardStp);
    t = millis();
    backed = false;
  }
  if((millis() - t) > 10000 && backed == false){     // release the inside pressure after 10s of idle
    myStepper.step(-idleBkStp);
    motor_stop();
    backed = true;
  }
}

void motor_stop() {
  // stepper off
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
