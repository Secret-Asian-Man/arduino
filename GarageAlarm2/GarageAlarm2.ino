#include <SimpleTimer.h>

#define SPEAKER_PIN 2
#define TONE_DURATION 10
#define HERTZ 2500  // Average human hearing range is 20-20000Hz
#define SONAR_TRIG_PIN 3
#define SONAR_ECHO_PIN 4
#define CHECK_FREQUENCY 15 * 60000  // 900000 = 15 mins
#define RANGE_THRESHOLD 12  // In inches //Door measures an average of 11 inches from sensor to door

bool doorWasClosed = true;
SimpleTimer timer;

void setup() {
  Serial.begin(9600);
  pinMode(SONAR_TRIG_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT);
  timer.setInterval(CHECK_FREQUENCY, checkDoor);
}

void loop() {
  timer.run();
}

void checkDoor() { 
  if (getRange() > RANGE_THRESHOLD)  //  If door is open
  {
    if (doorWasClosed)
    {
      doorWasClosed = false;
    }
    else
    {
      alarm();
    }
  }
  else
  {
      doorWasClosed = true;
  }
}

void alarm() {
  while(getRange() > RANGE_THRESHOLD) {
    tone(SPEAKER_PIN, HERTZ, TONE_DURATION);
  }
}

int getRange() {  // In inches
  digitalWrite(SONAR_TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(SONAR_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SONAR_TRIG_PIN, LOW);

  return (pulseIn(SONAR_ECHO_PIN, HIGH)/2) * 0.0344;
}

