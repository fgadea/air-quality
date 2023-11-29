#include "Arduino.h"
#include "Wire.h"
#include "U8x8lib.h"  //name=U8g2 version=2.33.15 author=oliver <olikraus@gmail.com> url=https://github.com/olikraus/u8g2
#include "DHT.h"      //name=Grove Temperature And Humidity Sensor version=2.0.1 author=Seeed Studio url=https://github.com/Seeed-Studio/Grove_Temperature_And_Humidity_Sensor
#define DHTTYPE DHT20 // DHT 20 *Notice: The DHT10 and DHT20 is different from other DHT* sensor ,it uses i2c interface rather than one wire So it doesn't require a pin
DHT dht(DHTTYPE);     // DHT10 DHT20 don't need to define Pin`
// U8X8_SSD1306_128X64_NONAME_SW_I2C Oled(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE); // POUR OLED SEUL
U8X8_SSD1306_128X64_NONAME_HW_I2C Oled(/* reset=*/U8X8_PIN_NONE); // POUR OLED ET AUTRE MODULE I2C (https://forum.seeedstudio.com/t/i2c-module-problems-with-grove-beginner-kit-for-arduino/252416/22)
void setup()
{
  Oled.setBusClock(100000); // https://forum.seeedstudio.com/t/i2c-module-problems-with-grove-beginner-kit-for-arduino/252416/28
  Oled.begin();
  Oled.setFlipMode(true);
  // Oled.setFont(u8x8_font_amstrad_cpc_extended_r); Oled.drawString(0, 0, "Setup");
  // Oled.setFont(u8x8_font_chroma48medium8_r); Oled.setCursor(0, 0); Oled.print("Setup");
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.println("DHT20 test!");
  Wire.begin();
  dht.begin();
}
void loop()
{
  Oled.setFont(u8x8_font_amstrad_cpc_extended_r); // Oled.drawString(0, 1, "Loop");
  // Oled.setFont(u8x8_font_chroma48medium8_r); Oled.setCursor(0, 1); Oled.print("Loop");
  float temp_hum_val[2] = {0}; // Reading temperature or humidity takes about 250 milliseconds! Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  if (!dht.readTempAndHumidity(temp_hum_val))
  {
    float myHumidity = temp_hum_val[0];
    float myTemperature = temp_hum_val[1];
    Serial.print("Humidity: ");
    Serial.print(myHumidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(myTemperature);
    Serial.println(" *C");
    Oled.setCursor(0, 0);
    Oled.print("Temperature");
    Oled.setCursor(0, 1);
    Oled.print(myTemperature);
    Oled.print(" °C");
    Oled.setCursor(0, 3);
    Oled.print("Humidite");
    Oled.setCursor(0, 4);
    Oled.print(myHumidity);
    Oled.print(" %");
  }
  else
  {
    Serial.println("Failed to get temprature and humidity value.");
  }
  delay(1500);
}