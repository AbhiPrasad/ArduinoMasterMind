const int BUTTONRED = 9; // 0 red
const int BUTTONGREEN = 10; // 1 green
const int BUTTONBLUE = 11; // 2 blue 
const int BUTTONWHITE = 12; // 3 white

const int BUTTON1 = 4; 
const int BUTTON2 = 5;
const int BUTTON3 = 6;
const int BUTTON4 = 7;

const int BUTTONINTER = 2; // Interrupt Pin for Colored Buttons

int pressArr[4] = {0, 0, 0, 0}; //array that is pressed
int randomArr[4] = {1, 1, 1, 1}; //randomized array that contains values that need to be checked

int correct = 0; //amount correct from array
volatile int btnPress = -1;
int error = 0;
volatile int lastPress = 0;
volatile int color = 0;

bool pressed = false; //flag that changes when interrupt triggers

void setup() {

  // cli();
  Serial.begin(9600); // debugging purposes\

  //interrupt buttons
  pinMode(BUTTONINTER, INPUT);

  //Interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTONINTER), btnLedPress, RISING);

  // LED for button press
  pinMode(BUTTONRED, OUTPUT);
  pinMode(BUTTONGREEN, OUTPUT);
  pinMode(BUTTONBLUE, OUTPUT);
  pinMode(BUTTONWHITE, OUTPUT);
  
  //analog read
  pinMode(A5, INPUT_PULLUP);

  //seeds random function
  randomSeed(analogRead(0));
  //randomize();
}

void loop() {
  if (pressed) {
    btnPressFlash();
    Serial.print("button press: ");
    Serial.println(btnPress);
    if (btnPress == 3 ) {
      pressArr[btnPress] = lastPress;
      btnPress = -1;
      for (int i = 0; i < 4; i++) {
        Serial.print(pressArr[i]);
      }
      Serial.println("");
      for (int i = 0; i < 4; i++) {
        Serial.print(randomArr[i]);
      }
      Serial.println("");
      bool sameArray = compareArray();
      if (sameArray) {
        //handleServo();
        resetCode();
        Serial.println("Same array!");
      } else {
        error = error + 1;
        Serial.println("Wrong array");
      }
    } else {
      //Serial.println(lastPress);
      pressArr[btnPress] = lastPress;
    }
    pressed = false;
    //interrupts();
  } else if (error == 5) {
    resetCode();
  }
}

//TODO
void handleServo() {
  
}

void victoryBlink() {
  
}

//TODO
void resetCode() {
  //randomize();
  
}

bool compareArray() {
  int err = 0;
  for (int i = 0; i < 4; i++) {
    if (pressArr[i] != randomArr[i]) {
      err += 1;
    }
  }
  if (err > 0) { //if arrays are different
    flashCorrect(err);
    return false;
  } else {
    return true;
  }
}

void flashCorrect(int num) {
  int temp = 0;
  digitalWrite(BUTTON1, LOW); //4 off
  digitalWrite(BUTTON2, LOW); // 5 off
  digitalWrite(BUTTON3, LOW); // 6 off
  digitalWrite(BUTTON4, LOW); // 7 off
  
  for (int j = 0; j < 4 - num; j++) { //cycles through and turns on if you got any correct
    temp = j + 4;
    digitalWrite(temp, HIGH);
  }
}

void randomize() {
  for (int x = 0; x < 4; x++) {
    randomArr[x] = random(0, 4);
  }
}

void btnPressFlash() {
  if (lastPress == 0) {
    digitalWrite(BUTTONRED, HIGH);
    delay(50);
    digitalWrite(BUTTONRED, LOW);
  } else if (lastPress == 1) {
    digitalWrite(BUTTONGREEN, HIGH);
    delay(50);
    digitalWrite(BUTTONGREEN, LOW);
  } else if (lastPress == 2) {
    digitalWrite(BUTTONBLUE, HIGH);
    delay(50);
    digitalWrite(BUTTONBLUE, LOW);
  } else if (lastPress == 3) {
    digitalWrite(BUTTONWHITE, HIGH);
    delay(50);
    digitalWrite(BUTTONWHITE, LOW);
  }
}

void btnLedPress() {
  color = analogRead(5);

// CHANGE BASED ON ACTUAL BUTTONS!!!!  
  if (color <= 934 && color >= 930) {
    lastPress = 0;
  } else if (color <= 847 && color >= 843) {
    lastPress = 1;
  } else if (color <= 783 && color >= 779) {
    lastPress = 2;
  } else if (color <= 705 && color >= 701) {
    lastPress = 3;
  }

  if (btnPress < 3) {
    btnPress = btnPress + 1;
  }

  pressed = true;
}
