const int TIME1 = 12; 
const int TIME2 = 11;
const int TIME3 = 10; 
const int TIME4 = 9; 
const int TIME5 = 8;

// 0 - red / 1 - green / 2 - blue / 3 - white

const int BUTTON1 = 4; 
const int BUTTON2 = 5;
const int BUTTON3 = 6;
const int BUTTON4 = 7;

const int BUTTONINTER = 2; // Interrupt Pin for Colored Buttons

int pressArr[4] = {0, 0, 0, 0}; //array that is pressed
int randomArr[4] = {1, 1, 1, 1}; //randomized array that contains values that need to be checked
int correctArr[4] = {0, 0, 0, 0};

volatile int btnPress = -1;
int error = 1;
volatile int lastPress = 0; //what color button was last pressd
volatile int color = 0; //what is analog read from pin to decide what button was pressed

bool pressed = false; //flag that changes when interrupt triggers

volatile unsigned long infiniteTime = 0;

volatile int time = 0;
int timeLight = 5;
bool turnOff = false;
bool startTimeLights = true;

volatile bool on = false;
bool start = true;

int score = 0;

void setup() {

  Serial.begin(9600);

  //interrupt buttons
  pinMode(BUTTONINTER, INPUT);

  //Interrupt
  attachInterrupt(digitalPinToInterrupt(BUTTONINTER), btnLedPress, RISING);
  
  //Timer
  pinMode(TIME1, OUTPUT);
  pinMode(TIME2, OUTPUT);
  pinMode(TIME3, OUTPUT);
  pinMode(TIME4, OUTPUT);
  pinMode(TIME5, OUTPUT);
  
  cli();
  TCCR1A = 0;     // set register to 0
  TCCR1B = 0;     

  TIMSK1 |= (1 << OCIE1A); //Enable interrupt
  
  TCCR1B |= (1 << WGM12); //turn on CTC - clear timer on compare
  
  //Set 1024 Prescaler 
  TCCR1B |= (1 << CS12); // 101 or 4 for 1024
  TCCR1B |= (1 << CS10);

   // set compare match register to desired timer count for 1 second
  OCR1A = 15624; //(1 sec/ (1 /(16Mhz/1024)) sec) - 1 = 15624
  
  sei();
  
  //analog read
  pinMode(A5, INPUT_PULLUP);

  //seeds random function
  randomSeed(analogRead(0));
}

void loop() { 
  if(on && start) { //if game is turned on
    resetCode();
    TCNT1 = 0;
    start = false;
    Serial.print("Game Starting ");
    Serial.print("Time: ");
    Serial.println(infiniteTime);
  } else if (!on && !start) { //if game is turned off
    turnOffLights();
    turnOffCorrect();
    Serial.print("Game Ending ");
    Serial.print("Time: ");
    Serial.println(infiniteTime);
    Serial.print("Your score is: ");
    Serial.print(score);
    Serial.println(" seconds");
    start = true;
  }
  
  if(startTimeLights && !start) {
    startTimeLights = false;
    initTimeLights();
  }
  
  if (turnOff && !start) {
    turnOff = false;
    timeLight = timeLight - 1;
    changeTimeLights(timeLight);
    if (timeLight != 0) {
      Serial.print("Turning off light ");
      Serial.print("Time: ");
      Serial.println(infiniteTime);
    }
  }
  
  if (pressed && !start) {
    cli();
    if (btnPress == 3) {
      if (lastPress > -1) {
        Serial.print("You pressed: ");
        if (lastPress == 0) {
          Serial.println("red");
        } else if (lastPress == 1) {
          Serial.println("green");
        } else if (lastPress == 2) {
          Serial.println("blue");
        } else if (lastPress == 3) {
          Serial.println("white");
        }
      }
      
      pressArr[btnPress] = lastPress;
      
      btnPress = -1;
      
      // SERIAL PRINTING DEBUGGING
      Serial.print("Your code: ");
      for (int i = 0; i < 4; i++) {
        Serial.print(pressArr[i]);
      }
      Serial.println("");
      for (int i = 0; i < 4; i++) {
        Serial.print(randomArr[i]);
      }
      Serial.println("");
      
      // SERIAL PRINTING END
      
      bool sameArray = compareArray();
      if (sameArray) {
        on = false;
        start = true;
        turnOffLights();
        turnOffCorrect();
        Serial.print("Right code! You won! ");
        Serial.print("Time: ");
        score = infiniteTime;
        Serial.println(infiniteTime);
        Serial.print("Your score is: ");
        Serial.print(score);
        Serial.println(" seconds");
      } else {
        error = error + 1;
        Serial.print("Wrong code! You have ");
        Serial.print(error);
        Serial.print(" errors ");
        Serial.print("Time: ");
        Serial.println(infiniteTime);
      }
    } else {
      if (lastPress > -1) {
        Serial.print("You pressed: ");
        if (lastPress == 0) {
          Serial.println("red");
        } else if (lastPress == 1) {
          Serial.println("green");
        } else if (lastPress == 2) {
          Serial.println("blue");
        } else if (lastPress == 3) {
          Serial.println("white");
        }
      }
      
      pressArr[btnPress] = lastPress;
    }
    pressed = false;
    sei();
  } 
  
  if (error >= 5 && !start) {
    Serial.print("Too many errors! Restart! "); 
    Serial.print("Time: ");
    Serial.println(infiniteTime);
    Serial.print("Your score is: ");
    Serial.print(score);
    Serial.println(" seconds");
    turnOffLights();
    turnOffCorrect();
    on = false;
    start = true;
  }
}

// start timer leds
void initTimeLights() {
  digitalWrite(TIME1, HIGH);
  digitalWrite(TIME2, HIGH);
  digitalWrite(TIME3, HIGH);
  digitalWrite(TIME4, HIGH);
  digitalWrite(TIME5, HIGH);
}

//turn off timer leds
void turnOffLights() {
  digitalWrite(TIME1, LOW);
  digitalWrite(TIME2, LOW);
  digitalWrite(TIME3, LOW);
  digitalWrite(TIME4, LOW);
  digitalWrite(TIME5, LOW);
}

void changeTimeLights(int timeLight) {
  digitalWrite(TIME1, LOW);
  digitalWrite(TIME2, LOW);
  digitalWrite(TIME3, LOW);
  digitalWrite(TIME4, LOW);
  digitalWrite(TIME5, LOW);

  int temp = 0;
  if (timeLight == 0) {
    Serial.println("Ran out of time! ");
    Serial.print("Time: ");
    Serial.println(infiniteTime);
    on = false;
    start = true;
    turnOffLights();
    turnOffCorrect();
  } else {
    for (int j = 0; j < timeLight; j++) {
      temp = j + 8;
      if (temp != 14) {
        digitalWrite(temp, HIGH);
      }
    }
  }
}

ISR(TIMER1_COMPA_vect) {
  time = time + 1;
  infiniteTime = infiniteTime + 1;
  if (time == 7) {
    time = 0;
    turnOff = true;
  }
}

void resetCode() {
  cli();
  time = 0;
  timeLight = 5;
  error = 0;
  btnPress = -1;
  infiniteTime = 0;
  correctArr[0] = 0;
  correctArr[1] = 0;
  correctArr[2] = 0;
  correctArr[3] = 0;
  turnOff = false;
  initTimeLights();
  turnOffCorrect();
  randomize();
  sei();
}

bool compareArray() {
  int err = 0;
  for (int i = 0; i < 4; i++) {
    if (pressArr[i] != randomArr[i]) {
      err += 1;
    } else {
      correctArr[i] = 1;
    }
  }
  if (err > 0) { //if arrays are different
    flashCorrect();
    return false;
  } else {
    return true;
  }
}

void turnOffCorrect() {
  digitalWrite(BUTTON1, LOW); //4 off
  digitalWrite(BUTTON2, LOW); // 5 off
  digitalWrite(BUTTON3, LOW); // 6 off
  digitalWrite(BUTTON4, LOW); // 7 off
}

void flashCorrect() {
  int temp = 0;
  digitalWrite(BUTTON1, LOW); //4 off
  digitalWrite(BUTTON2, LOW); // 5 off
  digitalWrite(BUTTON3, LOW); // 6 off
  digitalWrite(BUTTON4, LOW); // 7 off
  
  for (int j = 0; j < 4; j++) { //cycles through and turns on if you got any correct
    temp = j + 4;
    if (correctArr[j] == 1) {
      correctArr[j] = 0;
      digitalWrite(temp, HIGH);
    }
  }
}

void randomize() {
  for (int x = 0; x < 4; x++) {
    randomArr[x] = random(0, 4);
  }
}

void btnLedPress() {
  
  color = analogRead(5);
  
// CHANGE BASED ON ACTUAL BUTTONS!!!!  
  if (color <= 979 && color >= 975 && on) {
    lastPress = 0;
    if (btnPress < 3) {
      btnPress = btnPress + 1;
    }
  } else if (color <= 931 && color >= 927 && on) {
    lastPress = 1;
    if (btnPress < 3) {
      btnPress = btnPress + 1;
    }
  } else if (color <= 894 && color >= 890 && on) {
    lastPress = 2;
    if (btnPress < 3) {
      btnPress = btnPress + 1;
    }
  } else if (color <= 740 && color >= 736 && on) {
    lastPress = 3;
    if (btnPress < 3) {
      btnPress = btnPress + 1;
    }
  } else if (color <= 802 && color >= 800) {
    on = !on;
    lastPress = -1;
  }
  pressed = true;
}
