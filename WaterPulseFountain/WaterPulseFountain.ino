#define PUMP_PIN 3 // For pump
#define LED_PIN 9
#define POT_SIGIN_PIN A0

#define PUMP_PWM_VALUE 95 // Default 95
int pot_map = 0;

void setup() {                
  Serial.begin(9600);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(POT_SIGIN_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  analogWrite(PUMP_PIN, PUMP_PWM_VALUE);
}

void loop() {
  pot_map = map(analogRead(POT_SIGIN_PIN), 0, 1023, 0, 255);
  Serial.println(pot_map);
  pulse(LED_PIN,pot_map);
}

// Need to make snapshot light a static delay. Delay between snapshots should be variable
void pulse(char pin, int interval){
  digitalWrite(pin,1);
  delay(interval/2);
  digitalWrite(pin,0);
  delay(interval/2);
  return;
}

//Base Signal from Arduino
//Collector LED Ground
//Emitter Power Supply and Arduino Ground
//Connect the Arduino ground and Power supply ground
