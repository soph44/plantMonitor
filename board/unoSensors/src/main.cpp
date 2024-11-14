#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <dht11.h>
#include <ArduinoJson.h>

/*initialize devices instance*/
dht11 DHT;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/***pin setup***/
const int pinButton = 2;
const int pinLed = 4;
const int pinTemp = 12;
// const int pinLight = 13;
const int pinMoistDigital = 11;
const int pinMoistAnalog = A0;
const int pinLightAnalog = A3;

/*setp variable*/
int buttonState = LOW;
int lastButtonState = LOW;
// int pinState = 0;
unsigned long lastDebounce = 0;
unsigned long debounceDelay = 20;

int humidity = 0;
int temperature = 0;
int moistDO = 0;
int moistAO = 0;
int lightAO = 0;


/*********************************************************/
void setup()
{
  pinMode(pinButton, INPUT);
  pinMode(pinLed, OUTPUT);
  // pinMode(pinLight, INPUT);
  pinMode(pinMoistAnalog, INPUT);
  pinMode(pinMoistDigital, INPUT);

  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight

  Serial.begin(9600);
}


/******************************************************** */
void loop() 
{
    buttonState = digitalRead(pinButton);
    if (buttonState != lastButtonState){
      lastDebounce = millis();
    }

    if ((millis() - lastDebounce) > debounceDelay){
      buttonState = digitalRead(pinButton);
      if (buttonState == HIGH && lastButtonState == LOW){
        digitalWrite(pinLed, HIGH);

        // DHT11 Read, Temperature, Humidity
        int dhtRead = DHT.read(pinTemp);
        temperature =  DHT.temperature;
        humidity = DHT.humidity;

        // //LM393 w/ Photodiode, High/Low
        // int lmRead = digitalRead(pinLight);
        // Serial.println((String)"Light Reading: " + lmRead);

        //Moisture Sensor
        moistDO = digitalRead(pinMoistDigital);
        moistDO = map(moistDO, 0, 1, 1, 0);
        moistAO = analogRead(pinMoistAnalog);
        moistAO = map(moistAO, 0, 1024, 100, 0);

        //TEMT6000 Analog Light Sensor
        lightAO = analogRead(pinLightAnalog);
        lightAO = map(lightAO, 0, 2014, 0, 100);

        //Package into JSON format and serial to serial port to be read
        StaticJsonDocument<200> pkg;
        pkg["temperature"] = temperature;
        pkg["humidity"] = humidity;
        pkg["moisture"] = moistAO;
        pkg["light"] = lightAO;
        serializeJson(pkg, Serial);
        Serial.println();

        //Display on LCD1602 last reading during last button toggle
        //Nore that no I2C scanning was needed as the LCD I2C chip is tied directly to the dedicated SDA/SLC Uno pins
        lcd.setCursor(0, 0);
        lcd.print("T: " + String(DHT.temperature) + " / RH: " + String(DHT.humidity));
        lcd.setCursor(0, 1);
        lcd.print("L: " + String(lightAO) + " / M: " + String(moistAO)); 

        
        digitalWrite(pinLed, LOW);
      }
    }
    lastButtonState = buttonState;
}
/************************************************************/

// put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }
// include the library code 