/* LED cube test code
 *
 * Reads (x,y,z) co-ordinates from the Serial Monitor and toggles the state of
 * the LED at that co-ordinate. The co-ordinates are specified as "x y z", e.g.
 * "1 2 2", followed by a newline. Invalid co-ordinates are rejected.
 *
 * You need to fill in all the places marked TODO.
 *
 * == Setting up the Serial Monitor ==
 * The Serial Monitor must be configured (bottom-right corner of the screen) as:
 *   - Newline (for the line ending)
 *   - Baud rate 115200
 *
 * ENGR 40M
 * July 2017
 */

const byte CATHODE_PINS[8] = {4,A1,2,A2,5,A0,3,A3};
const byte ANODE_PINS[8] = {8,7,11,12,13,6,10,9};

void setup()
{
  // Make all of the negative and positive pins outputs and the audio pin input

    for (byte i = 0; i < 8; i++) {
      pinMode(ANODE_PINS[i], OUTPUT);
      pinMode(CATHODE_PINS[i], OUTPUT);
      digitalWrite(ANODE_PINS[i], HIGH);
      digitalWrite(CATHODE_PINS[i], HIGH);
    }

  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  Serial.begin(115200);
}

void clear() {
  for (byte i = 0; i < 8; i++) {
      digitalWrite(ANODE_PINS[i], HIGH);
      digitalWrite(CATHODE_PINS[i], HIGH);
    }
}

/* Function: getLEDState
 * ---------------------
 * Returns the state of the LED in a 4x4x4 pattern array, each dimension
 * representing an axis of the LED cube, that corresponds to the given anode (+)
 * wire and cathode (-) wire number.
 *
 * This function is called by display(), in order to find whether an LED for a
 * particular anode (+) wire and cathode (-) wire should be switched on.
 */
inline byte getLEDState(byte pattern[4][4][4], byte aNum, byte cNum)
{
  if(aNum < 4 && cNum < 4) {
    return pattern[cNum][aNum][0];
  }
  else if(aNum < 4 && cNum > 3) {
    return pattern[cNum - 4][aNum][1];
  }
  else if(aNum > 3 && cNum < 4) {
    return pattern[cNum][aNum - 4][3];
  }
  else if(aNum > 3 && cNum > 3) {
    return pattern[cNum - 4][aNum - 4][2];
  }
  return 0;
}

/* Function: display
 * -----------------
 * Runs through one multiplexing cycle of the LEDs, controlling which LEDs are
 * on.
 *
 * During this function, LEDs that should be on will be turned on momentarily,
 * one row at a time. When this function returns, all the LEDs will be off
 * again, so it needs to be called continuously for LEDs to be on.
 */
void display(byte pattern[4][4][4]) {
  for(byte cath = 0; cath < 4; cath++) {
    for(byte anod = 0; anod < 4; anod++) {
      for(byte layr = 0; layr < 4; layr++) {
        if(pattern[cath][anod][layr] == 1) {
          if(layr == 2) {
            digitalWrite(CATHODE_PINS[cath],LOW);
            digitalWrite(ANODE_PINS[anod],LOW);   
            delay(1);
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);        
          }
          if(layr == 1) {
            digitalWrite(CATHODE_PINS[cath],LOW);
            digitalWrite(ANODE_PINS[anod + 4],LOW);
            delay(1);
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH);  
          }
          if(layr == 3) {
            digitalWrite(CATHODE_PINS[cath + 4],LOW);
            digitalWrite(ANODE_PINS[anod],LOW);
            delay(1);
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);
          }
          else {
            digitalWrite(CATHODE_PINS[cath + 4],LOW);
            digitalWrite(ANODE_PINS[anod + 4],LOW);
            delay(1);
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH);
          }
        }
        else {
          if(layr == 0) {
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);           
          }
          if(layr == 1) {
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH); 
          }
          if(layr == 2) {
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);
          }
          else {
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH);
          }          
        }
      }
    }
  }
}

void loop()
{
    clear();
  static byte ledPattern[4][4][4]; // 1 for on, 0 for off

  byte x = 0;
  byte y = 0;
  byte z = 0;
  static char message[60];

  if (Serial.available()) {
    // Parse the values from the serial string
    x = Serial.parseInt();
    y = Serial.parseInt();
    z = Serial.parseInt();

    // Check for input validity
    if (Serial.read() != '\n') {
      Serial.println("invalid input - check that line ending is set to \"Newline\"; input must be three numbers");
      return;
    }
    if (x < 0 || x > 3 || y < 0 || y > 3 || z < 0 || z > 3) {
      sprintf(message, "x = %d, y = %d, z = %d -- indices out of bounds", x, y, z);
      Serial.println(message);
      return;
    }

    // Print to Serial Monitor to give feedback about input
    sprintf(message, "x = %d, y = %d, z = %d", x, y, z);
    Serial.println(message);

    // Toggle the LED state
    ledPattern[x][y][z] = !ledPattern[x][y][z];
  }

  // This function gets called every loop
  display(ledPattern);

}

