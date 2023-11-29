#include "Arduino.h"
#include "Wire.h"

#include "U8x8lib.h"
U8X8_SSD1306_128X64_NONAME_HW_I2C Oled(U8X8_PIN_NONE);

#include "DHT.h"
#define DHTTYPE DHT20 // DHT 20
DHT dht(DHTTYPE);

#define LIGHT_SENSOR A3
#define BUTTON 4
#define LED 6
#define SPEAKER 5
#define POTENCIOMETER A0

//-----------------
// Structures
enum State
{
  temperature,
  humidity,
  light
};
struct hdtData
{
  bool hasData;
  float humidity;
  float temperature;
};
//-----------------

//-----------------
// Global variables
State onState = temperature;
int buttonState = 0;
unsigned long timerCount = 0;
unsigned long oldTime = 0;
//-----------------

//-----------------
// Complementary functions
void printTemperature(hdtData data)
{
  Oled.setCursor(0, 0);
  Oled.print("Temp: ");
  Oled.setCursor(0, 4);
  Oled.print(data.temperature);
  Oled.print("\x00b0");
  Oled.print("C");
}

void printHumidity(hdtData data)
{
  Oled.setCursor(0, 0);
  Oled.print("Humid: ");
  Oled.setCursor(0, 4);
  Oled.print(data.humidity);
  Oled.print("%  ");
}

void printLight()
{
  digitalWrite(LED, HIGH);
  int lightSensorValue = analogRead(A3);
  int normalizedLight = map(lightSensorValue, 0, 1023, 0, 100);
  Oled.setCursor(0, 0);
  Oled.print("Light: ");
  Oled.setCursor(0, 4);
  Oled.print(normalizedLight);
  Oled.print("%     ");
}

hdtData getHdt()
{
  digitalWrite(LED, HIGH);
  struct hdtData data = hdtData();
  float temp_hum_val[2] = {0};
  if (!dht.readTempAndHumidity(temp_hum_val))
  {
    data.humidity = temp_hum_val[0];
    data.temperature = temp_hum_val[1];
    data.hasData = true;
  }
  else
  {
    data.hasData = false;
  }
  return data;
}

void checkButton()
{
  int prevState = buttonState;
  int newButtonState = digitalRead(BUTTON);

  // Prevent multiples taps at time
  if (prevState != newButtonState)
  {
    buttonState = newButtonState;
    digitalWrite(LED, HIGH);
  }
  else
  {
    return;
  }

  // Change state
  if (buttonState == HIGH)
  {
    switch (onState)
    {
    case temperature:
      onState = humidity;
      break;
    case humidity:
      onState = light;
      break;
    case light:
      onState = temperature;
      break;
    default:
      break;
    }
    timerCount = 0;
  }
}

unsigned long getDeltatime()
{
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - oldTime;
  oldTime = currentTime;
  return deltaTime;
}

void toneSpeaker() {
  int potenciometer = analogRead(POTENCIOMETER);
  if (potenciometer > 10)
  {
    tone(SPEAKER, analogRead(POTENCIOMETER));
  }
  else
  {
    noTone(SPEAKER);
  }
}
//-----------------

//-----------------
// Setup application
void setup()
{
  Oled.begin();
  Oled.setFlipMode(true);

  Wire.begin();
  dht.begin();

  pinMode(LIGHT_SENSOR, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(POTENCIOMETER, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
}
//-----------------

//-----------------
// Application loop
void loop()
{
  digitalWrite(LED, LOW);

  toneSpeaker();
  checkButton();

  delay(100);
  Oled.setFont(u8x8_font_inb21_2x4_f);

  unsigned long range = onState == light ? 900 : 2000;
  if (timerCount >= range)
  {
    switch (onState)
    {
    case temperature:
      printTemperature(getHdt());
      break;
    case humidity:
      printHumidity(getHdt());
      break;
    case light:
      printLight();
      break;
    default:
      break;
    }
    timerCount = 0;
  }

  timerCount += getDeltatime();
}
//-----------------