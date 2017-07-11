const int BUTTONRED = 9; // 0
const int BUTTONGREEN = 10; // 1
const int BUTTONBLUE = 11; // 2
const int BUTTONWHITE = 12; // 3
const int BUTTONINTER = 2; // Interrupt Pin for Colored Buttons

int pressArr[4] = {0, 0, 0, 0}; //array that is pressed
int randomArr[4] = {0, 0, 0, 0}; //randomized array that contains values that need to be checked

int correct = 0; //amount correct from array
volatile int btnPress = 0;
int error = 0;
volatile int lastPress = 0;

bool pressed = false;

void setup() {

  // cli();
  Serial.begin(9600); // debugging purposes\

  //Interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTONINTER), btnLedPress, RISING);

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
  if (pressed) {
    Serial.println(btnPress);
    if (btnPress == 4) {
      btnPress = 0;
      /*
      bool sameArray = compareArray();
      if (sameArray) {
        handleServo();
        resetCode();
      } else {
        error = error + 1;
      }
      */
    } else {
      //Serial.println(lastPress);
      pressArr[btnPress] = lastPress;
    }
    pressed = false;
  }

  if (error == 5) {
    resetCode();
  }
}

//TODO
void handleServo() {
  
}

//TODO
void resetCode() {
  
}

//TODO
bool compareArray() {
  for (int i = 0; i < 4; i++) {
    if (pressArr[i] != randomArr[i]) {
      return false;
    }
  }
  return true;
}

void randomize() {
  for (int x = 0; x < 4; x++) {
    randomArr[x] = random(0, 4);
  }
}

void btnLedPress() {
  if (btnPress < 4) {
    btnPress = btnPress + 1;
  }
  
  int red = digitalRead(BUTTONRED);
  int green = digitalRead(BUTTONGREEN);
  int blue = digitalRead(BUTTONBLUE);
  int white = digitalRead(BUTTONWHITE);

  if (red == HIGH) {
    lastPress = 0;
  } else if (green == HIGH) {
    lastPress = 1;
  } else if (blue == HIGH) {
    lastPress = 2;
  } else if (white == HIGH) {
    lastPress = 3;
  }
  
  pressed = true;
}
