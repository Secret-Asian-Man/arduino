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
#define MAX_SPEED 200

// Sonar Pins
#define TRIG_SONAR_PIN 12
#define ECHO_SONAR_PIN 13

// Sonar Globals
double distance = 0.0;

// RBG Pins
#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

// Enumerations
enum Direction {no_direction, forwards, backwards, left, right}; // For highest level function
enum Color {red, green, blue, white, yellow, cyan, magenta, no_color};

// Function Declarations
void move(Direction direction=forwards, float speed=1.0, double revolutions=1.0);
void goForwards(float step_delay, unsigned int steps);
void goBackwards(float step_delay, unsigned int steps);
void rotateClockwise(float step_delay, unsigned int steps);
void rotateCounterClockwise(float step_delay, unsigned int steps);
void stop();

double getDistance();

void setColor(Color color);
void policePattern(unsigned int delayTime=100);
void rainbowPattern(unsigned int delayTime=500);


// Program start
void setup(){
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

  move(forwards, 0.1);
  move(left,0.1);
  move(forwards, 0.1);
  move(left,0.1);
  move(forwards, 0.1);
  move(left,0.1);
  move(forwards, 0.1);
  move(left,0.1);
  
  //policePattern();
  //rainbowPattern();
}


/* @Description
 *  Highest level movement function, allowing user to manipulate the robot easily.
 *  
 * @Params 
 *  direction: consists of forwards, backwards, left, right, and no_direction
 *  speed: percentage of the max speed(MAX_SPEED). Must be be in the range 0.0 > speed >= 1.0
 *  revolutions: amount of full revolutions of the wheel consisting of 1600 steps for one revolution.
 */
void move(Direction direction, float speed, double revolutions) // Highest level function
{
  if(speed > 1.0 || speed <= 0.0 || revolutions < 0){
    stop();
    policePattern();
    return;
  }
  
  revolutions = revolutions * STEPS_PER_REVOLUTION; // Converting revolutions into steps for low level function calls.
  speed = (1/speed) * MAX_SPEED; // Converting speed percentage into a delay time for low level function calls.
  
  switch(direction){
    case forwards:
      setColor(green);
      goForwards(speed, revolutions);
    break;
    
    case backwards:
      setColor(red);
      goBackwards(speed, revolutions);
    break;
    
    case left:
      setColor(cyan);
      rotateCounterClockwise(speed, revolutions*0.75);
    break;
    
    case right:
      setColor(blue);
      rotateClockwise(speed, revolutions*0.75);
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
 *  Reads from sonar and returns distance in inches.
 *  
 * @Params 
 *  
 */
double getDistance(){
  // Reset trig pin
  digitalWrite(TRIG_SONAR_PIN, LOW);
  delayMicroseconds(2);

  // Send out pulse
  digitalWrite(TRIG_SONAR_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_SONAR_PIN, LOW);

  // Read returning pulse and format in inches
  return (pulseIn(ECHO_SONAR_PIN, HIGH)*0.00665);
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
void policePattern(unsigned int delayTime){
  
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
void rainbowPattern(unsigned int delayTime){

  delay(delayTime);
  setColor(red);
  delay(delayTime);
  setColor(green);
  delay(delayTime);
  setColor(blue);
  delay(delayTime);
  setColor(white);
  delay(delayTime);
  setColor(yellow);
  delay(delayTime);
  setColor(cyan);
  delay(delayTime);
  setColor(magenta);

  return;
}

