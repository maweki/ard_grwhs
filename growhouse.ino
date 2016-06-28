/* How to use the DHT-22 sensor with Arduino uno
   Temperature and humidity sensor
   More info: http://www.ardumotive.com/how-to-use-dht-22-sensor-en.html
   Dev: Michalis Vasilakis // Date: 1/7/2015 // www.ardumotive.com */

//Libraries
#include <DHT.h>;
#include "env.h";

//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// Functions
Environment readEnvironment();


Environment readEnvironment() {
  return {
    dht.readHumidity(),
    dht.readTemperature()
  };
}

#define RULECOUNT 2
Rule rules[] = {
//{ {minHum, maxHum}, {minTemp, maxTemp}, { led1 } }
  { {0,   50}, {0,100}, { Logic::False() } },
  { {60, 100}, {0,100}, { Logic::True() } }
};

void setup()
{
    // Initialize Serial Output
    Serial.begin(9600);

    // Initialize Sensor Libs
    dht.begin();

    // Initialize Pins
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
}

void loop()
{
    //Read data and store it to variables hum and temp
    Environment env = readEnvironment();
    Action act = EmptyAction();
    
    for (byte i = 0; i < RULECOUNT; i++) {
       act = AddActions(act, ruleApplies(env, rules[i]));
    }

    if (act.led1.isTrue()) { digitalWrite(13, HIGH); }
    if (act.led1.isFalse()) { digitalWrite(13, LOW); }


    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(env.humidity);
    Serial.print(" %, Temp: ");
    Serial.print(env.temperature);
    Serial.println(" Celsius"); 
    
    delay(2000); //Delay 2 sec.
}
