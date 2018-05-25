// Starter code to blink every LED using the simplest possible iteration
// through anode and cathode pins.
// ENGR 40M

const byte CATHODE_PINS[8] = {5,4,A0,A1,3,2,A3,A2};
const byte ANODE_PINS[8] = {9,10,6,13,12,11,7,8};

void setup()
{
    for (byte i = 0; i < 8; i++) {
      pinMode(ANODE_PINS[i], OUTPUT);
      pinMode(CATHODE_PINS[i], OUTPUT);
      digitalWrite(ANODE_PINS[i], HIGH);
      digitalWrite(CATHODE_PINS[i], HIGH);
    }
}
void loop()
{
    for(byte cNum = 0; cNum < 8; cNum++) {
      digitalWrite(CATHODE_PINS[cNum],LOW);
      for(byte aNum = 0; aNum < 8; aNum++) {
        digitalWrite(ANODE_PINS[aNum],LOW);
        delay(500);
        digitalWrite(ANODE_PINS[aNum],HIGH);
      }
      digitalWrite(CATHODE_PINS[cNum],HIGH);
    } 
}

