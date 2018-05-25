const byte CATHODE_PINS[8] = {5,A0,3,A3,4,A1,2,A2};
const byte ANODE_PINS[8] = {13,6,10,9,8,7,11,12};

void setup()
{
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

int getSample() {
 
  int valueL = abs(analogRead(A4) - 450);
  Serial.println(abs(valueL)); // to serial monitor
  return abs(valueL);
}

void display(byte pattern[4][4][4]) {
  for(byte layr = 0; layr < 4; layr++) {
    for(byte cath = 0; cath < 4; cath++) {
      for(byte anod = 0; anod < 4; anod++) {
        if(pattern[cath][anod][layr] == 1) {
          if(layr == 0) {
            digitalWrite(CATHODE_PINS[cath],LOW);
            digitalWrite(ANODE_PINS[anod],LOW);   
            delay(.3);
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);        
          }
          else if(layr == 1) {
            digitalWrite(CATHODE_PINS[cath + 4],LOW);
            digitalWrite(ANODE_PINS[anod],LOW);
            delay(.3);
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);  
          }
          else if(layr == 2) {
            digitalWrite(CATHODE_PINS[cath + 4],LOW);
            digitalWrite(ANODE_PINS[anod + 4],LOW);
            delay(.3);
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH);
          }
          else {
            digitalWrite(CATHODE_PINS[cath],LOW);
            digitalWrite(ANODE_PINS[anod + 4],LOW);
            delay(.3);
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH);
          }
        }
        else {
          if(layr == 0) {
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);
          }
          else if(layr == 1) {
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH); 
          }
          else if(layr == 2) {
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH);
          }
          else {
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH);
          }          
        }
      }
    }
  }
}

void loop()
{
    int sample;
    sample = getSample( );
    int count = 0;
    while(count < 50) {
       if(sample < 40) {
            byte arr[4][4][4] =
            {{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
            {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
            {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
            {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}};
            display(arr);
       }
       else if(sample < 80) {
            byte arr[4][4][4] =
            {{{1,0,0,1},{0,0,0,0},{0,0,0,0},{1,0,0,1}},
            {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
            {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
            {{1,0,0,1},{0,0,0,0},{0,0,0,0},{1,0,0,1}}};
            display(arr);
       }
       else if(sample < 150) {
            byte arr[4][4][4] =
            {{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
            {{0,1,1,0},{1,1,1,1},{1,1,1,1},{0,1,1,0}},
            {{0,1,1,0},{1,1,1,1},{1,1,1,1},{0,1,1,0}},
            {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}}};           
            display(arr);
       }
       else if(sample >= 150) {
            byte arr[4][4][4] =
            {{{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}},
            {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}},
            {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}},
            {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}}};        
            display(arr);
       }
       count++;
    }
    
}
