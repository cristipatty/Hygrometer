/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SHT31.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

Adafruit_SHT31 sht31 = Adafruit_SHT31();

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    while (1) delay(1); // Don't proceed, loop forever
  }

  if (!sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  
  drawInit();
  
}

void loop() {

  byte deg10, deg1, deg01, rh10, rh1 ;

  readTempRh(&deg10, &deg1, &deg01, &rh10, &rh1);

  drawTH(deg10, deg1, deg01, rh10, rh1);

  delay(2000);
}

float temp, rh;
void readTempRh(byte * deg10, byte * deg1, byte * deg01, byte * rh10, byte * rh1) {
  temp = sht31.readTemperature();
  rh = sht31.readHumidity();

  if (isnan(temp)) {
    temp = 0;
    Serial.println(F("Error reading Temp!"));
  }
  if (isnan(rh)) {
    rh = 0;
    Serial.println(F("Error reading RH!"));
  }

  *deg10 = (round(temp * 10.0) / 100.0);
  *deg1 = (int)(round(temp * 10.0) / 10.0) % 10;
  *deg01 = (int)(round(temp * 10.0)) % 10;
  *rh10 = (round(rh)) / 10;
  *rh1 = (int)(round(rh)) % 10;

  Serial.print(F("read: Temp "));
  Serial.print(*deg10);Serial.print(*deg1);Serial.print(F("."));Serial.print(*deg01);
  Serial.print(F(" ("));Serial.print(temp);Serial.print(F(") "));
  Serial.print(F(" RH "));Serial.print(*rh10);Serial.print(*rh1);
  Serial.print(F(" ("));Serial.print(rh);Serial.print(F(") "));
  Serial.println();
}

byte oldDeg10, oldDeg1, oldDeg01, oldRh10, oldRh1;

void drawInit() {
  display.setRotation(3);
  display.clearDisplay();
  display.setTextSize(1);
  
  int pixelCursorY = 43;
  int font18PtPxWidth = 18 + 1;
  display.setTextColor(WHITE);
  display.setFont(&FreeSansBold12pt7b);

  display.setCursor(font18PtPxWidth * 2, pixelCursorY);
  display.print(F("."));

  pixelCursorY = 107;

  display.setCursor(font18PtPxWidth * 2,pixelCursorY);
  display.print(F("%"));
}

void drawTH(byte deg10, byte deg1, byte deg01, byte rh10, byte rh1) {
  int pixelCursorX = 0;
  int pixelCursorY = 43;
  int font18PtPxWidth = 18 + 1;
  int font12PtPxWidth = 6;

  display.setFont(&FreeSansBold18pt7b); // 18pt = 24 pixels height, 18 width
  
  if (deg10 != oldDeg10) {
    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(BLACK);
    display.print(oldDeg10);
    
    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(WHITE);
    display.print(deg10);
    oldDeg10 = deg10;
  }

  pixelCursorX += font18PtPxWidth;

  if (deg1 != oldDeg1) {
    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(BLACK);
    display.print(oldDeg1);

    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(WHITE);
    display.print(deg1);
    oldDeg1 = deg1;
  }

  pixelCursorX += font18PtPxWidth;

  pixelCursorX += font12PtPxWidth;

  if (deg01 != oldDeg01) {
    display.setFont(&FreeSansBold12pt7b);
    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(BLACK);
    display.print(oldDeg01);

    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(WHITE);
    display.print(deg01);
    oldDeg01 = deg01;
  }
  
  // ===== Humidity =====

  pixelCursorY = 107;
  pixelCursorX = 0;
  
  display.setFont(&FreeSansBold18pt7b);

  if (rh10 != oldRh10) {
    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(BLACK);
    display.print(oldRh10);

    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(WHITE);
    display.print(rh10);
    oldRh10 = rh10;
  }
  
  pixelCursorX += font18PtPxWidth;

  if (rh1 != oldRh1) {
    display.setCursor(pixelCursorX,pixelCursorY);
    display.setTextColor(BLACK);
    display.print(oldRh1);

    display.setTextColor(WHITE);
    display.setCursor(pixelCursorX,pixelCursorY);
    display.print(rh1);
    oldRh1 = rh1;
  }
  
  display.display();
}

void scanI2C()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    // The i2c scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println(" !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("Done.\n");
  }

  delay(2000);
}
