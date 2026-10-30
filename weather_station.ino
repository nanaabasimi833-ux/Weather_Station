// libs used: Grove Barometer Sensor BMP280 | Grove Temperature and Humidity Sensor | U8g2
// OLED Screen (#include allows the use of external libs)
#include <Arduino.h>
#include <U8x8lib.h>

// Temperature and Humidity Sensor
#include "DHT.h"
#include "Wire.h"
#include "Seeed_BMP280.h"

#define DHTTYPE DHT20 // DHT 20 (Digital Humidity and Temperature Sensor Type)

BMP280 bmp280;
DHT dht(DHTTYPE);

// allows different boards to use their correct ports
#if defined(ARDUINO_ARCH_AVR)
  #define debug Serial
#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM)
  #define debug SerialUSB
#else
  #define debug Serial
#endif

// connector size, model - allows lib to know information about the screen
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

// Pins used
const int BuzzerPin = 5;
const int PotPin = A0;

void setup(void) {
  // initialised all dependencies
  debug.begin(115200);
  debug.println("DHTxx test!");
  Wire.begin();
  dht.begin();

  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);

  // check bmp280 is active
  if (!bmp280.init()) {
    debug.println("Device not connected or broken!");
  }

  pinMode(BuzzerPin, OUTPUT);   // sets buzzer to have an output (sound)
  digitalWrite(BuzzerPin, LOW); // buzzer starts low (no sound)
  pinMode(PotPin, INPUT);       // intake of user input
}

void loop(void) {
  // set vars for ease of use
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  float pressure = bmp280.getPressure() / 100.0;
  int potValue = analogRead(PotPin);

  // basic interpolation to map the pot to match temps from 15 to 40
  // potentiometer has a max analog read of 1023
  // (range worked out by 15 + 25 = 40 as max setting)
  float tempThreshold = 15.0 + ((potValue / 1023.0) * 25.0);

  // OLED (Content)
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  u8x8.setCursor(0, 0);
  u8x8.print("Temp:");
  u8x8.print(temp, 1);
  u8x8.print("C ");

  u8x8.setCursor(0, 2);
  u8x8.print("Humi:");
  u8x8.print(humidity, 1);
  u8x8.print("% ");

  // due to display issue decided to display (wrapping digits) pressure as Hectopascal
  u8x8.setCursor(0, 4);
  u8x8.print("Pres:");
  u8x8.print(pressure, 1);
  u8x8.print("hPa ");

  u8x8.setCursor(0, 6);
  u8x8.print("Alm:");
  u8x8.print(tempThreshold, 1);
  u8x8.print("C ");

  // checks current temp against threshold; if temp is higher, pulses buzzer HIGH/LOW to beep
  if (temp >= tempThreshold) {
    digitalWrite(BuzzerPin, HIGH);
    delay(200);
    digitalWrite(BuzzerPin, LOW);
    delay(200);
  } else {
    digitalWrite(BuzzerPin, LOW);
    delay(200);
  }

  u8x8.refreshDisplay();
  delay(200);
}
