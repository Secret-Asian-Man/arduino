#define OUTPUT_PIN                      D4
#define INPUT_PIN                       D6        
#define AUTO_CLOSE_TIME                 20 * 60000

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = ".....";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = ".....";
char pass[] = ".....";

bool doorClosed = true;

WidgetLCD lcd(V1);

BLYNK_WRITE(V0) // Button
{
  toggleDoor();
}

void toggleDoor() {
  digitalWrite(OUTPUT_PIN, HIGH);
  delay(1000);
  digitalWrite(OUTPUT_PIN, LOW);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT_PULLUP);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  doorClosed = digitalRead(INPUT_PIN);

  if (doorClosed)
  {
    lcd.print(0,0,"Door Closed");
  }
  else
  {
    lcd.print(0,0,"Door is OPEN!!!");
  }

  static unsigned long openTimer = millis();
  if (!doorClosed) {
    lcd.print(0,1, (String("Closing in ") + String((AUTO_CLOSE_TIME - (millis() - openTimer))/1000) + String("s")));
    if ((millis() - openTimer > AUTO_CLOSE_TIME)) {
      openTimer = millis();
      toggleDoor();
    }
  }
  else
  {
    openTimer = millis();
  }

  lcd.clear();
}
