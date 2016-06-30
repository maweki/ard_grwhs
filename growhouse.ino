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

// internal (non-sensor) state
bool led1;

Environment readEnvironment() {
  return (Environment){
    dht.readHumidity(),
    dht.readTemperature(),
    led1
  };
}

// Declarative Rules
#define RULECOUNT 2
Rule rules[] = {
// ======== PRECONDITIONS ================== / POSTCONDITIONS
//{ {minHum, maxHum}, {minTemp, maxTemp}, led1, { led1 } }
  { {0,   70}, {0,100}, Logic::DontCare(), { Logic::False() } },
  { {75, 100}, {0,100}, Logic::DontCare(), { Logic::True() } }
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

    // Declarative rules
    for (byte i = 0; i < RULECOUNT; i++) {
       act = AddActions(act, ruleApplies(env, rules[i]));
    }
    // Functional rules
    act = AddActions(act, functionalRuleStub(env));

    // synchronize/execute Actions
    // Should be at least True/False for every Action
    if (act.led1.isTrue()) {
      digitalWrite(13, HIGH);
      led1 = true;
    }
    if (act.led1.isFalse()) {
      digitalWrite(13, LOW);
      led1 = false;
    }


    //Print Environment to serial monitor twice a minute
    if (cnt++ >= 30) {
      cnt = 0;
      Serial.print(jsonEnv(env));
    }

    delay(1000); //Delay 1 sec.
}
