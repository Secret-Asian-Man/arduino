#define OUTPUT_PIN                      D4
#define INPUT_PIN                       D6                    
#define DOOR_TIMER_PIN                  V2
#define DOOR_TOGGLE_PIN                 V0

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = ".....";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = ".....";
char pass[] = ".....";

bool DOOR_CLOSED = true;
unsigned long AUTO_CLOSE_TIME = 30 * 60000;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

WidgetLCD lcd(V1);

BLYNK_WRITE(DOOR_TOGGLE_PIN) // Button
{
  toggleDoor();
}

BLYNK_WRITE(DOOR_TIMER_PIN) // Slider
{
  AUTO_CLOSE_TIME = param.asInt() * 60000;
}

void toggleDoor() {
  digitalWrite(OUTPUT_PIN, HIGH);
  delay(1000);
  digitalWrite(OUTPUT_PIN, LOW);
}

String millisecondsToMinSec(unsigned long milliseconds) {
  return String(milliseconds/60000) + "m " + String((milliseconds/1000) % 60) +"s";
}

void initialize() {
  Blynk.setProperty(DOOR_TIMER_PIN, "min", 30);
  Blynk.setProperty(DOOR_TIMER_PIN, "max", 180);
  Blynk.virtualWrite(DOOR_TIMER_PIN, AUTO_CLOSE_TIME/60000);
  Blynk.syncVirtual(DOOR_TIMER_PIN);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT_PULLUP);
  Blynk.begin(auth, ssid, pass);
  initialize();
}

void loop()
{
  Blynk.run();
  DOOR_CLOSED = !digitalRead(INPUT_PIN);

  if (DOOR_CLOSED)
  {
    lcd.print(0,0,"Door Closed");
  }
  else
  {
    lcd.print(0,0,"Door is OPEN!!!");
  }

  currentMillis = millis();
  if (!DOOR_CLOSED) {
    lcd.print(0,1, (millisecondsToMinSec(AUTO_CLOSE_TIME - (unsigned long)(currentMillis - previousMillis))));
    if (((unsigned long)(currentMillis - previousMillis) >= AUTO_CLOSE_TIME)) {
      previousMillis = currentMillis;
      toggleDoor();
    }
  }
  else {
    previousMillis = currentMillis;
  }

  lcd.clear();
}
