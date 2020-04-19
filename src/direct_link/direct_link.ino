/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

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
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  
#define GPIO_PIN 12
//#define GPIO_LED_D16 16

void setup() {
  Serial.begin(115200);
  //Serial.println("Booting...");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.invertDisplay(true);
  // the library initializes this with an Adafruit splash screen.
  display.display();
  //delay(2000); // Pause for 2 seconds
    // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setCursor(0,0);             // Start at top-left corner
  
  //Serial.println("TempObj TempObjAvg TempAmb TempAmbAvg");
  Serial.println("TempObj TempAmb");
  pinMode(GPIO_PIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
//  pinMode(GPIO_LED_D16, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

void set_output(void) {
  pinMode(GPIO_PIN, OUTPUT); 
}

void set_input(void) {
  pinMode(GPIO_PIN, INPUT); 
}

void set_pin(int val) {
  pinMode(GPIO_PIN, val);
}

// Read the current state of the pin and return 0 or 1
int read_pin(void) {
  int val;
  val = digitalRead(GPIO_PIN);
  //Serial.println(val);
  return val;
}

void toggle_low_high(int delay) {
  digitalWrite(GPIO_PIN, LOW);
  delayMicroseconds(delay);
  digitalWrite(GPIO_PIN, HIGH);
  delayMicroseconds(delay);
}

 int myCounter= 0 ;
 float tempAmbAvg = 0;
 float tempObjAvg = 0;
 float tempAmbSum = 0;
 float tempObjSum = 0;

// the loop function runs over and over again forever
void loop() {
  int tempObj=0;
  int tempAmb=0;
  int tempbit = 0;
  float tempK = 0;
  //  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
  //delayMicroseconds(180);
  set_output();
  set_pin(LOW);
  delay(200);
  set_pin(HIGH);
  delayMicroseconds(90);
  
  for (int i=0; i <17; i++){
    set_output();
    toggle_low_high(10);
    set_input();
    //delayMicroseconds(20);
    tempObj <<= 1;
    if (read_pin() == 0x01) tempObj++;
    //delayMicroseconds(10);
    }

  //set_output();
  tempAmb = 0;
  tempbit = 0;
  for (int i=0; i < 14; i++) {
     set_output();
     toggle_low_high(10);
     set_input();
     delayMicroseconds(10);
     tempbit = 1 << (13-i);
     if (read_pin() == 0x01){      
      //Set the bit to high and take or with the current value 
      tempAmb = tempAmb | tempbit;
     }else {
      
     }
     /*delayMicroseconds(10);
     tempAmb <<= 1;
     if (read_pin() == 0x01) tempAmb++;
     //delayMicroseconds(10);
     */
  }
  set_output(); // Configure DL Pin as output
  set_pin(LOW); // Set DL Port low for min. 200ns
  delayMicroseconds(30);
  set_input();  // Configure DL Port as input
  // Comversion: 44 uV per Kelven uV/K
  // 1 count = 0.8 uV
  // Volts = counts * 0.8 uV 
  // K = 44 uV / K >> 1/44 K/uV * volts
  //tempK = (((tempObj*0.8)/140)-((tempAmb*0.8)/90));
  //tempK = tempK/290;
  if (myCounter >=25) {
    myCounter = 0;
    tempAmbAvg = (tempAmbSum/25);
    tempObjAvg = (tempObjSum/25);
    tempAmbSum = 0;
    tempObjSum = 0;
    
    //Serial.print((float)(tempObjAvg/290)-273.15);
    //Serial.print("\t");
    //Serial.println((float)(tempAmbAvg/90) - 273.15);
    
    //Serial.print((float)(tempObjAvg));
    //Serial.print("\t");
    //Serial.println((float)(tempAmbAvg));
    //Serial.print("\t");
    //Serial.println((((float)tempObjAvg/290)+(float)tempAmbAvg/90)-273.15);

  } else {
    tempAmbSum +=tempAmb;
    tempObjSum +=tempObj;
    myCounter++; 
  }
  /*
  for (int b = 16; b >= 0; b--)
  {
    Serial.print(bitRead(tempObj, b));
  }
  Serial.print(" ");
  for (int b = 13; b >= 0; b--)
  {
    Serial.print(bitRead(tempAmb, b));
  }
  Serial.print("\t"); */
  //Serial.print("Temp Obj: ");
  Serial.print((float)tempObj);
  Serial.print(" ");
  //Serial.println((float)tempObjAvg/100);
  //Serial.print("\t");
    
  //Serial.print("Temp Obj C: ");
  //Serial.print(tempObj/126);
  //Serial.print(" Temp Amb: " );
  Serial.println((float)tempAmb);
  //Serial.print("\t");
  //Serial.println((float)tempAmbAvg/90);
  //Serial.print(" Temp Amb c: " );
  //Serial.print(tempAmb/90);
  //Serial.print(" Temp K: " );
  //Serial.print(tempK);
  //Serial.print(" Temp C: " );
  //Serial.println(tempK-273.15);
  //delay(500);
  display.setCursor(0,0);             // Start at top-left corner
  display.print(F("TO: "));
  display.println(tempObj);
  display.print(F("TA: "));
  display.println(tempAmb);
// - Configure DL Port as output
// - Set DL Port low for min. 200ns
// - Set DL Port high for min. 200ns
// - Configure DL Port as input
// - Tamb << 1; shift bits left by 1 bit
// - if ( DL Port == high) Tamb++; // read 1

}
