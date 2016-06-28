struct Environment {
  float humidity;
  float temperature;
};

String jsonEnv(Environment env) {
  return String("{ \"hum\": ") + String(env.humidity) +
         String(", \"tmp\": ") + String(env.temperature) +
         String(" }");
};

struct RangeFloat {
  float min;
  float max;
};

struct RangeInt {
  int min;
  int max;
};

class Logic {
private:
    byte state;
public:
    static Logic True() { Logic a; a = Logic(); a.state = 1; return a; };
    bool isTrue() { return state == 1; };
    
    static Logic False() { Logic a; a = Logic(); a.state = 0; return a; };
    bool isFalse() { return state == 0; };
    
    static Logic DontCare() { Logic a; a = Logic(); a.state = 2; return a; };
    bool isDontCare() { return state == 2; };
    
    Logic And(Logic other) {
      if (other.isFalse() or isDontCare()) { return other; }
      return *this;
    };
    
    bool Get(bool def) {
      if (isDontCare())  { return def; }
      return isTrue();
    }
};

struct Action {
  Logic led1;
//  Logic water_soil;
//  Logic water_air;
};

Action AddActions(Action a, Action b) {
  return {
    a.led1.And(b.led1)//,
//    a.water_soil.And(b.water_soil),
//    a.water_air.And(b.water_air)
  };
};

Action EmptyAction() {
  return { Logic::DontCare()/*, Logic::DontCare(), Logic::DontCare()*/ }; 
}

struct Rule {
  RangeFloat rangeHumidity;
  RangeFloat rangeTemperature;
  Action action;
};

Action ruleApplies(Environment env, Rule rule) {
  if (env.humidity >= rule.rangeHumidity.min and
      env.humidity <= rule.rangeHumidity.max and
      env.temperature >= rule.rangeTemperature.min and
      env.temperature <= rule.rangeTemperature.max)
  { return rule.action; }
  return EmptyAction();
}
