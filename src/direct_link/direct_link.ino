/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#define GPIO_PIN 12
#define GPIO_LED_D16 16

void setup() {
  Serial.begin(115200);
  //Serial.println("Booting...");
  delay(100);
  //Serial.println("TempObj TempObjAvg TempAmb TempAmbAvg");
  Serial.println("TempObj TempAmb");
  pinMode(GPIO_PIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(GPIO_LED_D16, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
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

  float tempK = 0;
  //  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
  //delayMicroseconds(180);
  set_output();
  set_pin(LOW);
  delay(50);
  set_pin(HIGH);
  delayMicroseconds(90);
  
  for (int i=0; i <17; i++){
    set_output();
    toggle_low_high(30);
    set_input();
    //delayMicroseconds(20);
    tempObj <<= 1;
    if (read_pin() == 0x01) tempObj++;
    //delayMicroseconds(10);
    }

  //set_output();
  
  for (int i=0; i < 14; i++) {
     set_output();
     toggle_low_high(30);
     set_input();
     //delayMicroseconds(10);
     tempAmb <<= 1;
     if (read_pin() == 0x01) tempAmb++;
     //delayMicroseconds(10);
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
    
    Serial.print((float)(tempObjAvg/290)-273.15);
    Serial.print("\t");
    Serial.print((float)(tempAmbAvg/90) - 273.15);
    Serial.print("\t");
    Serial.println((((float)tempObjAvg/290)+(float)tempAmbAvg/90)-273.15);

  } else {
    tempAmbSum +=tempAmb;
    tempObjSum +=tempObj;
    myCounter++; 
  }
  //Serial.print("Temp Obj: ");
  //Serial.print((float)tempObj);
  //Serial.print("\t");
  //Serial.println((float)tempObjAvg/100);
  //Serial.print("\t");
    
  //Serial.print("Temp Obj C: ");
  //Serial.print(tempObj/126);
  //Serial.print(" Temp Amb: " );
  //Serial.println((float)tempAmb);
  //Serial.print("\t");
  //Serial.println((float)tempAmbAvg/90);
  //Serial.print(" Temp Amb c: " );
  //Serial.print(tempAmb/90);
  //Serial.print(" Temp K: " );
  //Serial.print(tempK);
  //Serial.print(" Temp C: " );
  //Serial.println(tempK-273.15);
  //delay(500);
  
// - Configure DL Port as output
// - Set DL Port low for min. 200ns
// - Set DL Port high for min. 200ns
// - Configure DL Port as input
// - Tamb << 1; shift bits left by 1 bit
// - if ( DL Port == high) Tamb++; // read 1

}
