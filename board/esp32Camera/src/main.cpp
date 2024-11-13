#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <dht11.h>

/*initialize devices instance*/
dht11 DHT;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/***pin setup***/
const int pinButton = 2;
const int pinLed = 4;
const int pinTemp = 12;
const int pinLight = 13;
const int pinMoistDigital = 11;
const int pinMoistAnalog = A0;
const int pinLightAnalog = A3;

/*setp variable*/
char array1[]=" Start  ";  //the string to print on the LCD
char array2[]=" ";  //the string to print on the LCD
int tim = 250;  //the value of delay time
int buttonState = 0;
int lastButtonState = 0;
int pinState = 0;
float humidity = 0;
float temperature = 0;
int moistDO = 0;
int moistAO = 0;
int lightAO = 0;


/*********************************************************/
void setup()
{
  pinMode(pinButton, INPUT);
  pinMode(pinLed, OUTPUT);
  pinMode(pinLight, INPUT);
  pinMode(pinMoistAnalog, INPUT);
  pinMode(pinMoistDigital, INPUT);

  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  Serial.begin(9600);
}


/******************************************************** */
void loop() 
{
    lastButtonState = buttonState;
    buttonState = digitalRead(pinButton);
    if (buttonState == HIGH && buttonState != lastButtonState){
      if (pinState == 0){
        digitalWrite(pinLed, HIGH);
        Serial.println("Button Toggle ON");
        pinState = 1;
      } else if (pinState == 1){
        digitalWrite(pinLed, LOW);
        Serial.println("Button Toggle OFF");
        pinState = 0;
      }
      lastButtonState = buttonState;
    }

  if (lastButtonState == HIGH){
    // DHT11 Read, Temperature, Humidity
    int dhtRead = DHT.read(pinTemp);
    Serial.println((String)"Temperature (C): " + DHT.temperature + " // Humidity (%): " + DHT.humidity);

    //LM393 w/ Photodiode, High/Low
    int lmRead = digitalRead(pinTemp);
    Serial.println((String)"Light Reading: " + lmRead);

    //Moisture Sensor
    int moistDO = digitalRead(pinMoistDigital);
    moistDO = map(moistDO, 0, 1, 1, 0);
    int moistAO = analogRead(pinMoistAnalog);
    moistAO = map(moistAO, 0, 1024, 100, 0);
    Serial.println((String)"Moisture Digital: " + moistDO + " // Moisture Analog: " + moistAO);

    //TEMT6000 Analog Light Sensor
    int lightAO = analogRead(pinLightAnalog);
    Serial.println((String)"Light Level (%): " + lightAO);
    lightAO = map(lightAO, 0, 2014, 0, 100);
    
  }

    // lcd.setCursor(15,0);  // set the cursor to column 15, line 0
    // for (int positionCounter1 = 0; positionCounter1 < 26; positionCounter1++)
    // {
    //   lcd.scrollDisplayLeft();  //Scrolls the contents of the display one space to the left.
    //   lcd.print(array1[positionCounter1]);  // Print a message to the LCD.
    //   delay(tim);  //wait for 250 microseconds
    // }
    // lcd.clear();  //Clears the LCD screen and positions the cursor in the upper-left corner.
    // lcd.setCursor(15,1);  // set the cursor to column 15, line 1
    // for (int positionCounter = 0; positionCounter < 26; positionCounter++)
    // {
    //   lcd.scrollDisplayLeft();  //Scrolls the contents of the display one space to the left.
    //   lcd.print(array2[positionCounter]);  // Print a message to the LCD.
    //   delay(tim);  //wait for 250 microseconds
    // }
    // lcd.clear();  //Clears the LCD screen and positions the cursor in the upper-left corner.
}
/************************************************************/

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}// include the library code