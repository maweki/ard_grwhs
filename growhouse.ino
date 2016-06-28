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

// Declarative Rules
#define RULECOUNT 2
Rule rules[] = {
//{ {minHum, maxHum}, {minTemp, maxTemp}, { led1 } }
  { {0,   70}, {0,100}, { Logic::False() } },
  { {75, 100}, {0,100}, { Logic::True() } }
};

// Functional Rules
Action functionalRuleStub(Environment env) {
  return EmptyAction();
}

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

byte cnt = 0;

void loop()
{
    //Read data and store it to variables hum and temp
    Environment env = readEnvironment();
    Action act = EmptyAction();
    
    for (byte i = 0; i < RULECOUNT; i++) {
       act = AddActions(act, ruleApplies(env, rules[i]));
    }
    act = AddActions(act, functionalRuleStub(env));

    if (act.led1.isTrue()) { digitalWrite(13, HIGH); }
    if (act.led1.isFalse()) { digitalWrite(13, LOW); }


    //Print temp and humidity values to serial monitor
    if (cnt++ >= 10) {
      cnt = 0;
      Serial.print(jsonEnv(env));
    }

    delay(1000); //Delay 1 sec.
}
