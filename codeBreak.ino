const int BUTTONPRESS = 13; // 0 pressed button

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

int correct = 0; //amount correct from array
volatile int btnPress = -1;
int error = 0;
volatile int lastPress = 0; //what color button was last pressd
volatile int color = 0; //what is analog read from pin to decide what button was pressed

bool pressed = false; //flag that changes when interrupt triggers

volatile int time = 0;
int timeLight = 5;
bool turnOff = false;
bool startTimeLights = true;

void setup() {

  // cli();
  Serial.begin(9600); // debugging purposes\

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
  
  // set compare match register to desired timer count for 1 second
  OCR1A = 15624; //(1 sec/ (1 /(16Mhz/1024)) sec) - 1 = 15624
  
  TCCR1B |= (1 << WGM12); //turn on CTC - clear timer on compare
  
  //Set 1024 Prescaler 
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  
  TIMSK1 |= (1 << OCIE1A); //Enable interrupt
  
  sei();
  
  //analog read
  pinMode(A5, INPUT_PULLUP);

  //seeds random function
  randomSeed(analogRead(0));
  randomize();
}

void loop() {
  if(startTimeLights) {
  	startTimeLights = false;
    initTimeLights();
  }
  
  if (turnOff) {
    turnOff = false;
    Serial.println("Timer Light Off!");
    timeLight = timeLight - 1;
    changeTimeLights(timeLight);
  }
  
  if (pressed) {
    delay(10);
    btnPressFlash();
    Serial.print("button press: ");
    
    if (lastPress == 0) {
      Serial.println("red");
    } else if (lastPress == 1) {
      Serial.println("blue");
    } else if (lastPress == 2) {
      Serial.println("green");
    } else if (lastPress == 3) {
      Serial.println("white");
    }
      
    if (btnPress == 3 ) {
      pressArr[btnPress] = lastPress;
      btnPress = -1;
      
      /*
      // SERIAL PRINTING DEBUGGING
      for (int i = 0; i < 4; i++) {
        Serial.print(pressArr[i]);
      }
      Serial.println("");
      for (int i = 0; i < 4; i++) {
        Serial.print(randomArr[i]);
      }
      Serial.println("");
      */
      // SERIAL PRINTING END
      
      bool sameArray = compareArray();
      if (sameArray) {
        //handleServo();
        resetCode();
        Serial.println("Right code!");
      } else {
        error = error + 1;
        Serial.println("Wrong code!");
      }
    } else {
      pressArr[btnPress] = lastPress;
    }
    pressed = false;
  } else if (error == 5) {
    Serial.println("Too many errors! Code Randomized"); 
    resetCode();
  }
}

//start timer leds
void initTimeLights() {
  digitalWrite(TIME1, HIGH);
  digitalWrite(TIME2, HIGH);
  digitalWrite(TIME3, HIGH);
  digitalWrite(TIME4, HIGH);
  digitalWrite(TIME5, HIGH);
}

void changeTimeLights(int time) {
  digitalWrite(TIME1, LOW);
  digitalWrite(TIME2, LOW);
  digitalWrite(TIME3, LOW);
  digitalWrite(TIME4, LOW);
  digitalWrite(TIME5, LOW);
  
  int temp = 0;
  if (time == 0) {
    Serial.println("Ran out of time!");
    resetCode();
  } else {
    for (int j = 0; j < time; j++) {
      temp = j + 8;
      digitalWrite(temp, HIGH);
    }
  }
}

ISR(TIMER1_COMPA_vect) {
  time = time + 1;
  if (time == 5) {
  	time = 0;
    turnOff = true;
  }
}


//TODO
void handleServo() {
  
}

//TODO
void victoryBlink() {
  
}

//TODO
void resetCode() {
  cli();
  initTimeLights();
  time = 0;
  timeLight = 5;
  error = 0;
  randomize();
  sei();
  
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
  digitalWrite(BUTTONPRESS, HIGH);
  delay(50);
  digitalWrite(BUTTONPRESS, LOW);
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
