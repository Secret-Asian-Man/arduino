#define PUMP_PIN 4 // For pump
#define POT_SIGOUT_PIN 9
#define POT_SIGIN_PIN A0

#define BLINK_DELAY 50
#define PUMP_DELAY 50
#define PUMP_INTERVAL 100 // TODO: Needs testing
int pot_map = 0;

void setup() {                
  Serial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(POT_SIGIN_PIN, INPUT);
  pinMode(POT_SIGOUT_PIN, OUTPUT);
}

void loop() {
  pot_map = map(analogRead(POT_SIGIN_PIN), 0, 1023, 0, 255);
  Serial.println(pot_map);
  pulse(POT_SIGOUT_PIN, BLINK_DELAY,pot_map);
  pulse(PUMP_PIN, PUMP_DELAY, PUMP_INTERVAL);
}

void pulse(char pin, int pulseDelay int interval){
  digitalWrite(pin,1);
  delay(pulseDelay);
  digitalWrite(pin,0);
  delay(interval);
  return;
}

