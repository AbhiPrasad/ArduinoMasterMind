const int BUTTONRED = 9; // 0
const int BUTTONGREEN = 10; // 1
const int BUTTONBLUE = 11; // 2
const int BUTTONWHITE = 12; // 3
const int BUTTONINTER = 2; // Interrupt Pin for Colored Buttons

int pressArr[4] = {0, 0, 0, 0}; //array that is pressed
int randomArr[4] = {1, 1, 1, 1}; //randomized array that contains values that need to be checked

int correct = 0; //amount correct from array
volatile int btnPress = -1;
int error = 0;
volatile int lastPress = 0;
volatile int color = 0;

bool pressed = false;

void setup() {

  // cli();
  Serial.begin(9600); // debugging purposes\

  //interrupt buttons
  pinMode(BUTTONINTER, INPUT);

  //Interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTONINTER), btnLedPress, RISING);

  //scores
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
    delay(1000);
    noInterrupts();
    //Serial.println(color);
    //Serial.println(btnPress);
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
        //resetCode();
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
    interrupts();
  } else if (error == 5) {
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

  if (color <= 979 && color >= 975) {
    lastPress = 0;
  } else if (color <= 931 && color >= 927) {
    lastPress = 1;
  } else if (color <= 984 && color >= 980) {
    lastPress = 2;
  } else if (color <= 740 && color >= 736) {
    lastPress = 3;
  }

  if (btnPress < 3) {
    btnPress = btnPress + 1;
  }

  pressed = true;
}
