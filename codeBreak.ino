const int BUTTONRED = 9;
const int BUTTONGREEN = 10;
const int BUTTONBLUE = 11;
const int BUTTONWHITE = 12;
const int BUTTONINTER = 2;

volatile int* pressArr[4] = {0, 0, 0, 0}; //array that is pressed
 
int randomArr[4] = {0, 0, 0, 0}; //randomized array that contains values that need to be checked

int correct = 0; //amount correct from array
int errors = 0;

void setup() {
  Serial.begin(9600); // debugging purposes\
  
  //Interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTONINTER), btnLedPress, HIGH);
  
  //input pins
  pinMode(BUTTONRED, INPUT);
  pinMode(BUTTONGREEN, INPUT);
  pinMode(BUTTONBLUE, INPUT);
  pinMode(BUTTONWHITE, INPUT);
  
  
  pinMode(BUTTONINTER, INPUT);
  
  //seeds random function
  randomSeed(analogRead(0));
  randomize();
}

void loop() {
  delay(500);
  
  Serial.println(randomArr[2]);
}

void randomize() {
  for (int x = 0; x < sizeof(randomArr)/sizeof(int); x++) {
    randomArr[x] = random(0, 4);
  }
}

void btnLedPress() {
  
}
