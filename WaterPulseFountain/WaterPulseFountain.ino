//#define LED_PIN 2
#define MOSFET_PIN 4 //4
#define POT_SIG_PIN 1 //P2

void setup() {                
  Serial.begin(9600);
  pinMode(MOSFET_PIN, OUTPUT);
//  pinMode(LED_PIN, OUTPUT);
}


void loop() { 
//  analogWrite(LED_PIN, 255/3);
  analogWrite(MOSFET_PIN,map(analogRead(POT_SIG_PIN), 0, 1023, 0, 255));
}
