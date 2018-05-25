// Starter code to blink every LED using the simplest possible iteration
// through anode and cathode pins.
// ENGR 40M

const byte CATHODE_PINS[8] = {2, 3, 4, 5, A0, A1, A2, A3};
const byte ANODE_PINS[8] = {6, 7, 8, 9, 10, 11, 12 ,13};

void setup()
{
    for (byte i = 0; i < 8; i++) {
      pinMode(ANODE_PINS[i], OUTPUT);
      pinMode(CATHODE_PINS[i], OUTPUT);
      digitalWrite(ANODE_PINS[i], LOW);
      digitalWrite(CATHODE_PINS[i], HIGH);
    }
}
void loop()
{
  for (byte aNum = 0; aNum < 8; aNum++) {
    digitalWrite(ANODE_PINS[aNum],LOW);
    for (byte cNum = 0; cNum < 8; cNum++) {
          digitalWrite(CATHODE_PINS[cNum],LOW);
//          delay(250);
          digitalWrite(CATHODE_PINS[cNum],HIGH);
    }
    digitalWrite(ANODE_PINS[aNum],HIGH);
  }


}

