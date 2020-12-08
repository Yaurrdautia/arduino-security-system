#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;
LiquidCrystal_I2C lcd(0x27, 20, 4);

//phases
int phase1ok = 0;

//wiat for return to middle
int waitformiddle = 0;

//joystick strokes to open
const int s[] = {1, 2, 3, 4};
//up is 1 down is 2 right is 3 left is 4
//inputed joystick strokes
int is[] = {0, 0, 0, 0};

//phase 2 tries
int jattempts = 3, currentjattempts = 0;
int jlocktime = 1 * 60000;

//stroke count
int sc = 0;
//button count
int bc = 0;

//joystick pins
int joyx = A5;
int joyy = A4;
int x = 0;
int y = 0;

//button on && off
int bb0 = 0, bb1 = 0, bb2 = 0, bb3 = 0, bb4 = 0;

//password
const int dig0 = 1;
const int dig1 = 0;
const int dig2 = 3;
const int dig3 = 1;

//mistake delay
//number of mistakes accepted befor temp lock
const int attempts = 3;
//time (minutes) lock will temp lock
int locktime = 1 * 60000;
int currentattempts = 0;

//button pins
const int b1 = 4, b2 = 5, b3 = 8, b4 = 7, b5 = 2;

//other pins piezo, led
const int piezo = 6, ledr = 11, ledg = 10, ledb = 9;

//input numbers
int num0 = 0, num1 = 0, num2 = 0, num3 = 0;


//max num
int maxnum = 10;

//number display function
void numdisp() {
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Ready");
  lcd.setCursor(5, 1);
  lcd.print(num0);
  lcd.setCursor(7, 1);
  lcd.print(num1);
  lcd.setCursor(9, 1);
  lcd.print(num2);
  lcd.setCursor(11, 1);
  lcd.print(num3);
}

void openclose() {
  tone(piezo, 880);
  delay(400);
  noTone(piezo);
  myservo.write(180);
  analogWrite(ledr, 0);
  analogWrite(ledg, 255);
  analogWrite(ledb, 0);
  delay(8000);
  myservo.write(90);
  analogWrite(ledr, 0);
  analogWrite(ledg, 0);
  analogWrite(ledb, 255);
  tone(piezo, 880);
  delay(100);
  noTone(piezo);
  delay(100);
  tone(piezo, 880);
  delay(300);
  noTone(piezo);
}

void setup() {
  //boot seequence
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("4 # Pass System");
  lcd.setCursor(0, 1);
  lcd.print("By Yutaro U.");
  delay (1000);
  lcd.clear();
  //servo setup
  myservo.attach(3);
  myservo.write(90);
  Serial.print("beginning sequence complete, door locked");
  //button setup
  pinMode(joyx, INPUT);
  pinMode(joyy, INPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
  pinMode(b5, INPUT);
  pinMode(piezo, OUTPUT);
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  analogWrite(ledr, 0);
  analogWrite(ledg, 0);
  analogWrite(ledb, 255);
  delay (100);
  lcd.setCursor(0, 0);
  lcd.print("Ready");
  delay(200);
  lcd.clear();
  numdisp();
}

void loop() {
  //check joystick
  x = analogRead(joyx);
  y = analogRead(joyy);
  //check for button 1
  bb0 = digitalRead(b1);
  bb1 = digitalRead(b2);
  bb2 = digitalRead(b3);
  bb3 = digitalRead(b4);
  bb4 = digitalRead(b5);

  if (bb0 == HIGH) {
    tone(piezo, 880);
    delay(100);
    noTone(piezo);
    num0++;
    numdisp();
    delay(300);
    if ((num0 == maxnum) && (bb0 == HIGH)) {
      num0 = 0;
      numdisp();
    }
  }

  if (bb1 == HIGH) {
    tone(piezo, 880);
    delay(100);
    noTone(piezo);
    num1++;
    numdisp();
    delay(500);
    if ((num1 == maxnum) && (bb1 == HIGH)) {
      num1 = 0;
      numdisp();
    }
  }

  if (bb2 == HIGH) {
    tone(piezo, 880);
    delay(100);
    noTone(piezo);
    num2++;
    numdisp();
    delay(500);
    if ((num2 == maxnum) && (bb2 == HIGH)) {
      num2 = 0;
      numdisp();
    }
  }

  if (bb3 == HIGH) {
    tone(piezo, 880);
    delay(100);
    noTone(piezo);
    num3++;
    numdisp();
    delay(500);
    if ((num3 == maxnum) && (bb3 == HIGH)) {
      num3 = 0;
      numdisp();
    }
  }

  if (bb4 == HIGH) {
    if ((num0 == dig0) && (num1 == dig1) && (num2 == dig2) && (num3 == dig3)) {
      phase1ok = 1;
      currentattempts = 0;

      lcd.setCursor(0, 0);
      lcd.print("Phase 1 unlocked");
      tone(piezo, 1000);
      delay(400);
      noTone(piezo);
      delay(500);
    }
    else if (phase1ok == 0) {
      analogWrite(ledr, 255);
      analogWrite(ledg, 0);
      analogWrite(ledb, 0);
      tone(piezo, 261);
      delay(300);
      noTone(piezo);
      delay(500);
      currentattempts++;
      lcd.setCursor(0, 0);
      lcd.print("Attempts Left: ");
      lcd.print(attempts - currentattempts);
      lcd.print("/");
      lcd.print(attempts);
      delay(900);
      analogWrite(ledr, 0);
      analogWrite(ledg, 0);
      analogWrite(ledb, 255);
      if (currentattempts == attempts) {
        lcd.setCursor(1, 0);
        lcd.clear();
        lcd.print("Device Locked");
        delay(locktime);
      }
    }
    delay(200);
    num0 = 0;
    num1 = 0;
    num2 = 0;
    num3 = 0;
    numdisp();
  }


  //second phase
  if (phase1ok == 1) {
    if (waitformiddle == 1 &&
        x > 500 && x < 900 && y > 500 && y < 700) {
      waitformiddle = 0;
    } 
    else if ((sc < 4) && (waitformiddle == 0)) {
      //down
      if (x < 500) {
        is[sc] = 2;
      }
      //up
      else if (x > 900) {
        is[sc] = 1;
      }
      //right
      else if (y > 700) {
        is[sc] = 3;
      }
      //left
      else if (y < 500) {
        is[sc] = 4;
      }
      else {
        is[sc] = 0;
      }

      if (is[sc] != 0)
      {
        sc++;
        
        tone(piezo, 880);
        delay(100);
        noTone(piezo);
        waitformiddle = 1;
      }
    }
    if (sc == 4) {
      if ((s[0] == is[0]) && (s[1] == is[1]) && (s[2] == is[2]) && (s[3] == is[3])) {
        openclose();
        sc = 0;
        is[0] = is[1] = is[2] = is[3] = 0;
        phase1ok = 0;
        numdisp();
        currentjattempts = 0;
      }
      else {
        waitformiddle = 0;
        sc = 0;
        is[0] = is[1] = is[2] = is[3] = 0;
        analogWrite(ledr, 255);
        analogWrite(ledg, 0);
        analogWrite(ledb, 0);
        tone(piezo, 261);
        delay(300);
        noTone(piezo);
        delay(300);
        tone(piezo, 261);
        delay(300);
        noTone(piezo);
        currentjattempts++;
        lcd.setCursor(0, 0);
        lcd.print("Attempts Left1: ");
        lcd.print(jattempts - currentjattempts);
        lcd.print("/");
        lcd.print(jattempts);
        delay(900);
        analogWrite(ledr, 0);
        analogWrite(ledg, 0);
        analogWrite(ledb, 255);
        if (currentjattempts == jattempts) {
          lcd.setCursor(1, 0);
          lcd.clear();
          lcd.print("Device Locked2");
          delay(jlocktime);
        }
      }
    }
  }
}

