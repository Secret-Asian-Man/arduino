//#define MOSFET_PIN 4
#define POT_SIGOUT_PIN 9
#define POT_SIGIN_PIN A0

#define BLINK_DELAY 50
int pot_map = 0;

void setup() {                
  Serial.begin(9600);
  //pinMode(MOSFET_PIN, OUTPUT);
  pinMode(POT_SIGIN_PIN, INPUT);
  pinMode(POT_SIGOUT_PIN, OUTPUT);
}

void loop() {
  pot_map = map(analogRead(POT_SIGIN_PIN), 0, 1023, 0, 255);
  Serial.println(pot_map);
  strobeLight(POT_SIGOUT_PIN, pot_map);
}

void strobeLight(char pin, int interval){
  digitalWrite(pin,1);
  delay(BLINK_DELAY);
  digitalWrite(pin,0);
  delay(interval);
  return;
}

