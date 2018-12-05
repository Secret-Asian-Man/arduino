#include <NewPing.h> //https://playground.arduino.cc/Code/NewPing

/*
 * Program:
 *  MIT 101 robot project.
 *  
 * Purpose:
 *  To acquire and showcase new skills.
 *  
 * Programmer:
 *  David Wu
 *  
 * Date:
 *  11/10/2018 - David Wu - Initial creation
 *  
 */

// Stepper Pins
#define R_CLK_PIN 8
#define R_CW_PIN 9
#define L_CLK_PIN 10
#define L_CW_PIN 11

// Globals
#define STEPS_PER_REVOLUTION 1600
#define MAX_SPEED 200 // Delay in microseconds
#define COLLISION_DISTANCE 12
#define DEFAULT_AVOID_SPEED 0.4
#define DEFAULT_SPEED 0.5
#define WHEEL_RADIUS 1.565 // Must be greater than 0

// Sonar
#define TRIG_SONAR_PIN 12
#define ECHO_SONAR_PIN 13
NewPing sonar(TRIG_SONAR_PIN, ECHO_SONAR_PIN);

// RBG Pins
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

// Enumerations
enum Direction {forwards, left, right, backwards, no_direction}; // For highest level function
enum Color {red, green, blue, white, yellow, cyan, magenta, no_color};

// Function Declarations
inline void pilotTest();

inline void task1();
inline void task2();
inline void task3();
inline void task4();

void move(Direction direction=forwards, float speed=1.0, int inches=12);
void goForwards(float step_delay, unsigned int steps);
void goBackwards(float step_delay, unsigned int steps);
void rotateClockwise(float step_delay, unsigned int steps);
void rotateCounterClockwise(float step_delay, unsigned int steps);
void stop();

double getDistance(); //Depreciated
bool willCollide(double inches);

void setColor(Color color);
inline void policePattern(unsigned int delayTime=100);
inline void rainbowPattern(unsigned int delayTime=500);


// Program start
void setup(){
  
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  pinMode(R_CLK_PIN, OUTPUT);
  pinMode(R_CW_PIN, OUTPUT);
  pinMode(L_CLK_PIN, OUTPUT);
  pinMode(L_CW_PIN, OUTPUT);
  
  pinMode(TRIG_SONAR_PIN, OUTPUT);
  pinMode(ECHO_SONAR_PIN, INPUT);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  digitalWrite(L_CW_PIN, HIGH); // Sets left stepper to spin correctly. High is clockwise.
  digitalWrite(R_CW_PIN, LOW);
  
}


void loop(){

//  task1();
//  delay(3000);
  
//  task2();
//  delay(3000);

   task3();

//  task4();
  
}


/* @Description
 *  Move forward 3 feet.  Stop.  Wait 3 seconds.  Return to the starting position.
 *  
 * @Params 
 */
inline void task1(){
  
  move(forwards, DEFAULT_SPEED, 12*3); // Move forwards at 50% speed for 3 feet.
  delay(1000);
  move(backwards, DEFAULT_SPEED, 12*3); // Move backwards at 50% speed for 3 feet.
  
  return;
  
}


/* @Description
 *  Move in a 3 foot square, stopping at the original position.
 *  
 * @Params 
 */
inline void task2(){

  for(int i=0; i<4; ++i){
    move(forwards, DEFAULT_SPEED, 12*3); // Move forwards at 50% speed for 3 feet.
    move(left, DEFAULT_AVOID_SPEED); // turn counter clockwise at 50% speed.
  }

  return;
}


/* @Description
 *  Move randomly around the room avoiding obstacles. 
 *  
 * @Params 
 */
inline void task3(){

  while (true)
    move(random(3), DEFAULT_SPEED ,12); // Excluding backwards, move randomly at 50% speed in 1 foot increments.
  
  return;
}


/* @Description
 *  Move a distance of approximately six feet between two pre-determine points.
 *  There will be 2-3 objects approximately 8 inches tall and 4 inches wide between 
 *  the two points that the robot will have to avoid.
 *  
 * @Params 
 */
inline void task4(){
  
  // Not enough info
  
  return;
}


/* @Description
 *  Highest level movement function, allowing user to manipulate the robot easily.
 *  
 *  Warning: Collision check will only happen before going forwards.
 *  
 * @Params 
 *  direction: consists of forwards, backwards, left, right, and no_direction
 *  speed: percentage of the max speed(MAX_SPEED). Must be be in the range 0.0 > speed >= 1.0
 *  inches: distance to drive in inches. Will be ignored when turning.
 */
void move(Direction direction, float speed, int inches) // Highest level function
{
  if(speed > 1.0 || speed <= 0.0 || inches < 0){
    stop();
    policePattern();
    return;
  }

  double revolutions = inches/(2*PI*WHEEL_RADIUS); // converting inches to wheel revolutions.
  revolutions = revolutions * STEPS_PER_REVOLUTION; // Converting revolutions into steps for low level function calls.
  speed = (1/speed) * MAX_SPEED; // Converting speed percentage into a delay time for low level function calls.
  
  switch(direction){
    case forwards:
    if(willCollide(inches+3)){
        move(backwards, DEFAULT_AVOID_SPEED);
        if(random(2)) //range from 0 to 1
          move(left, DEFAULT_AVOID_SPEED);
        else
          move(right, DEFAULT_AVOID_SPEED);
    } else{
      setColor(green);
      goForwards(speed, revolutions);
    }
    break;
    
    case backwards:
      setColor(red);
      goBackwards(speed, revolutions);
    break;
    
    case left:
      setColor(cyan);
      rotateCounterClockwise(speed, 1200);
    break;
    
    case right:
      setColor(blue);
      rotateClockwise(speed, 1200);
    break;
    
    case no_direction:
      stop();
    break;
  }

  return;
}


/* @Description
 *  Low level helper function for moving fowards. Used by move().
 *  
 * @Params 
 *  step_delay: The delay between each stepper step.
 *  steps: The amount of times to step a step.
 */
void goForwards(float step_delay, unsigned int steps){
  digitalWrite(L_CW_PIN, HIGH);
  digitalWrite(R_CW_PIN, LOW);
  
  for(unsigned int i = 0; i<steps; ++i){
    digitalWrite(R_CLK_PIN, HIGH);
    digitalWrite(L_CLK_PIN, HIGH);
    delayMicroseconds(step_delay);
    digitalWrite(R_CLK_PIN, LOW);
    digitalWrite(L_CLK_PIN, LOW);
  }

  return;
}


/* @Description
 *  Low level helper function for moving backwards. Used by move().
 *  
 * @Params 
 *  step_delay: The delay between each stepper step.
 *  steps: The amount of times to step a step.
 */
void goBackwards(float step_delay, unsigned int steps){
  digitalWrite(L_CW_PIN, LOW);
  digitalWrite(R_CW_PIN, HIGH);
  
  for(unsigned int i = 0; i<steps; ++i){
    digitalWrite(R_CLK_PIN, HIGH);
    digitalWrite(L_CLK_PIN, HIGH);
    delayMicroseconds(step_delay);
    digitalWrite(R_CLK_PIN, LOW);
    digitalWrite(L_CLK_PIN, LOW);
  }

  return;
}


/* @Description
 *  Low level helper function for rotating clockwise. Used by move().
 *  
 * @Params 
 *  step_delay: The delay between each stepper step.
 *  steps: The amount of times to step a step.
 */
void rotateClockwise(float step_delay, unsigned int steps)
{
  
  digitalWrite(L_CW_PIN, HIGH);
  digitalWrite(R_CW_PIN, HIGH);

  for(unsigned int i = 0; i<steps; ++i){
    digitalWrite(R_CLK_PIN, HIGH);
    digitalWrite(L_CLK_PIN, HIGH);
    delayMicroseconds(step_delay);
    digitalWrite(R_CLK_PIN, LOW);
    digitalWrite(L_CLK_PIN, LOW);
  }

  return;
}


/* @Description
 *  Low level helper function for rotating counterclockwise. Used by move().
 *  
 * @Params 
 *  step_delay: The delay between each stepper step.
 *  steps: The amount of times to step a step.
 */
void rotateCounterClockwise(float step_delay, unsigned int steps)
{

  digitalWrite(L_CW_PIN, LOW);
  digitalWrite(R_CW_PIN, LOW);

  for(unsigned int i = 0; i<steps; ++i){
    digitalWrite(R_CLK_PIN, HIGH);
    digitalWrite(L_CLK_PIN, HIGH);
    delayMicroseconds(step_delay);
    digitalWrite(R_CLK_PIN, LOW);
    digitalWrite(L_CLK_PIN, LOW);
  }
  
  return;
}


/* @Description
 *  Low level helper function for stopping.
 *  
 * @Params 
 * 
 */
void stop(){
  digitalWrite(R_CLK_PIN, LOW);
  digitalWrite(L_CLK_PIN, LOW);
  
  return;
}


/* @Description
 *  Tests LEDS and motors. Drives in a square figure 8 pattern.
 *  
 * @Params 
 * 
 */
inline void pilotTest(){
  
  rainbowPattern();
  policePattern();
  
  for(unsigned int i=0; i<4; ++i){
    move(forwards, 0.2, 1);
    move(left,0.2, 1); 
  }
  
  for(unsigned int i=0; i<4; ++i){
    move(backwards, 0.2, 1);
    move(right,0.2, 1);
  }

  return;
}


//Depreciated. Use NewPing library.
/* @Description
 *  Reads from sonar and returns distance in inches.
 *  
 * @Params 
 *  
 */
double getDistance(){
  static double distance = 0.0;

  // Reset trig pin
  digitalWrite(TRIG_SONAR_PIN, LOW);
  delayMicroseconds(2);

  // Send out pulse
  digitalWrite(TRIG_SONAR_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_SONAR_PIN, LOW);

  distance = pulseIn(ECHO_SONAR_PIN, HIGH)*0.00665;
  
  //Serial.println(distance);
  
  // Read returning pulse and format in inches
  return (distance);
}


/* @Description
 *  Checks if there is any obstacle X inches away.
 *  
 * @Params 
 * inches: number of inches of safety you want.
 */
bool willCollide(double inches){
  return sonar.ping_in() < inches;
}


/* @Description
 *  Set LED colors at maximum brightness to 1 of 7 distinct colors.
 *  
 * @Params 
 *  color: can be set to red, green, blue, white, yellow, cyan, magenta, or no_color
 */
void setColor(Color color){
  /*
   * White = RGB
   * Yellow = RG
   * Cyan = GB
   * Magenta = RB
   */
    //nreturn; // disables lights
   switch(color){
    
    case red:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
    break;
      
    case green:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
    break;

    case blue:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
    break;
      
    case white:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
    break;
      
    case yellow:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, LOW);
    break;
      
    case cyan:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
    break;
      
    case magenta:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, HIGH);
    break;
      
    case no_color:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
    break;
    
    default:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
   }
   
   return;
}


/* @Description
 *  Set LEDs to display a police light pattern. Good for debugging.
 *  
 * @Params 
 *  delayTime: delay between light flickers. Delay time in-between color changes is doubled.
 */
inline void policePattern(unsigned int delayTime){
  
  delay(delayTime*2);
  setColor(red);
  delay(delayTime);
  setColor(no_color);
  delay(delayTime);
  setColor(red);
  delay(delayTime);
  setColor(no_color);
  
  delay(delayTime*2);
  setColor(blue);
  delay(delayTime);
  setColor(no_color);
  delay(delayTime);
  setColor(blue);
  delay(delayTime);
  setColor(no_color);

  return;
}


/* @Description
 *  Set LEDs to display all the possible colors. Good for pilot light testing.
 *  
 * @Params 
 *  delayTime: delay between light changes.
 */
inline void rainbowPattern(unsigned int delayTime){

  for(unsigned char i = 0; i<7; ++i){
    delay(delayTime);
    setColor(i);
  }

  return;
}
