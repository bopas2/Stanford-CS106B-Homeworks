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

void display(byte pattern[4][4][4]) {
  for(byte cath = 0; cath < 4; cath++) {
    for(byte anod = 0; anod < 4; anod++) {
      for(byte layr = 0; layr < 4; layr++) {
        if(pattern[cath][anod][layr] == 1) {
          if(layr == 2) {
            digitalWrite(CATHODE_PINS[cath],LOW);
            digitalWrite(ANODE_PINS[anod],LOW);   
            delay(.5);
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);        
          }
          if(layr == 1) {
            digitalWrite(CATHODE_PINS[cath],LOW);
            digitalWrite(ANODE_PINS[anod + 4],LOW);
            delay(.5);
            digitalWrite(CATHODE_PINS[cath],HIGH);
            digitalWrite(ANODE_PINS[anod + 4],HIGH);  
          }
          if(layr == 3) {
            digitalWrite(CATHODE_PINS[cath + 4],LOW);
            digitalWrite(ANODE_PINS[anod],LOW);
            delay(.5);
            digitalWrite(CATHODE_PINS[cath + 4],HIGH);
            digitalWrite(ANODE_PINS[anod],HIGH);
          }
          else {
            digitalWrite(CATHODE_PINS[cath + 4],LOW);
            digitalWrite(ANODE_PINS[anod + 4],LOW);
            delay(.5);
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
            int x = 0;
            while(x < 700) {
            byte arr0[4][4][4] =
            {{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
            {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
            {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},      //global warming, economic/power inequality, lack of education 
            {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}};
            display(arr0);
            x++;
            }
            x = 0;
            while(x < 700) {
            byte arr[4][4][4] =
            {{{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},
            {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},
            {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},     //global warming, economic/power inequality, lack of education 
            {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}}};
            display(arr);
            x++;
            }
            x = 0;
            while(x < 500) {
            byte arr1[4][4][4] =
            {{{1,1,0,0},{1,1,0,0},{1,1,0,0},{1,1,0,0}},
            {{1,1,0,0},{1,1,0,0},{1,1,0,0},{1,1,0,0}},
            {{1,1,0,0},{1,1,0,0},{1,1,0,0},{1,1,0,0}},     //global warming, economic/power inequality, lack of education 
            {{1,1,0,0},{1,1,0,0},{1,1,0,0},{1,1,0,0}}};
            display(arr1);
            x++;
            }
            x = 0;
            while(x < 500) {
            byte arr1[4][4][4] =
            {{{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}},
            {{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}},
            {{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}},     //global warming, economic/power inequality, lack of education 
            {{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}}};
            display(arr1);
            x++;
            }
            x = 0;
            while(x < 1000) {
            byte arr1[4][4][4] =
            {{{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}},
            {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}},
            {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}},     //global warming, economic/power inequality, lack of education 
            {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}}};
            display(arr1);
            x++;
            }
            x = 0;
            while(x < 500) {
            byte arr1[4][4][4] =
            {{{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}},
            {{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}},
            {{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}},     //global warming, economic/power inequality, lack of education 
            {{1,1,1,0},{1,1,1,0},{1,1,1,0},{1,1,1,0}}};
            display(arr1);
            x++;
            }
            x = 0;
            while(x < 500) {
            byte arr1[4][4][4] =
            {{{1,1,0,0},{1,1,0,0},{1,1,0,0},{1,1,0,0}},
            {{1,1,0,0},{1,1,0,0},{1,1,0,0},{1,1,0,0}},
            {{1,1,0,0},{1,1,0,0},{1,1,0,0},{1,1,0,0}},     //global warming, economic/power inequality, lack of education 
            {{1,1,0,0},{1,1,0,0},{1,1,0,0},{1,1,0,0}}};
            display(arr1);
            x++;
            }
            x = 0;
            while(x < 500) {
            byte arr[4][4][4] =
            {{{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},
            {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},
            {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},     //global warming, economic/power inequality, lack of education 
            {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}}};
            display(arr);
            x++;
            }
            x = 0;
            while(x < 500) {
            byte arr0[4][4][4] =
            {{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
            {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
            {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},      //global warming, economic/power inequality, lack of education 
            {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}};
            display(arr0);
            x++;
            }
            x = 0;

}

