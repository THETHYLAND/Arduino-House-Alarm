#include <Keypad.h> // include keypad library
#include <LiquidCrystal.h> // include lcd library
#include <Password.h>

#define magDoor 17 // define entry/exit door reed switch
#define Hallway 15 // define Hallway PIR
#define Kitchen 16 // define kitchen PIR
#define Bedroom 14 // define bedroom PIR
#define buzzer 8 // define buzzer
#define led 13 // define led
#define water 18 // define water sensor
#define button 21

LiquidCrystal lcd(53, 51, 49, 47, 45, 43); // initialize lcd
const byte numRows = 4; // amount of keypad rows
const byte numCols = 4; // amount of keypad columns
char keymap[numRows][numCols] = { // keypad keys
  {
    '1', '2', '3', 'A'
  }
  ,
  {
    '4', '5', '6', 'B'
  }
  ,
  {
    '7', '8', '9', 'C'
  }
  ,
  {
    '*', '0', '#', 'D'
  }
};
byte rowPins[numRows] = { // set keypad rows
  25, 27, 29, 31
};
byte colPins[numCols] = { // set keypad columns
  33, 35, 37, 39
};

char password1[] = {'1', '2', '3', '4'}; //password
char inputPassword[4]; // input password
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols); // initialize keypad
Password password = Password(password1); // initialize password
String newpass; // new password entry 1
String newpass1; // new password entry 2
int correct = 0; // correct numbers

char c; // button pressed
int lock = 0; // incorrect password entries
int x = 0; // password loop amount
int setting = 0; // if setting menu is entered

int HallVal = LOW; // hallway PIR
int KitVal = LOW; // kitchen PIR
int BedVal = LOW; // bedroom PIR
int DoorVal = LOW; // entry/exit door reed switch
int waterVal = 0; // water sensor
int buttonState = 0; // panic button

unsigned long previousMillis = 0; // delay previous time
unsigned long currentMillis; // delay current time
int Delay = 5000; // entry/exit delay

int night = 0; // if night mode is enabled. night == 1
int ntime = 21600; // how long the night alarm will last
long time; // night timer

void setup() {
  lcd.begin(16, 2); // initialize lcd
  Serial.begin(9600); //initialize serial port
  pinMode(Bedroom, INPUT); //initialize Bedroom PIR
  pinMode(magDoor, INPUT_PULLUP); //initialize Magnet Door Switch
  pinMode(Kitchen, INPUT);  //initialize Kitchen PIR
  pinMode(Hallway, INPUT); //initialize Hallway PIR
  pinMode(water, INPUT);  //initialize water sensor
  pinMode(button, INPUT); //initialize panic button
  pinMode(buzzer, OUTPUT); //initialize buzzer
  pinMode(led, OUTPUT); //initialize led
}
void startLoop() {//this loop runs at the start until a button is pressed
  while (c == 0) {
    lcd.clear();
    lcd.print("Welcome");
    lcd.setCursor(0, 1);
    lcd.print("Press a button for menu");
    lcd.setCursor(0, 0);
    delay(50);
    x = 0;
    night = 0;
    lock = 0;
    setting = 0;
    c = myKeypad.getKey();
    buttonState = digitalRead(button);
    if (buttonState == HIGH) {
      panicSound();
    }
  }
}
void panicSound() { // this is for the panic alarm 
  lcd.clear();
  lcd.print("Panic Alarm");
  tone(buzzer, 1500);
  digitalWrite(led, HIGH);
  Serial.write(2);
  delay(5000);
  Serial.write(0);
  digitalWrite(led, LOW);
  noTone(buzzer);
}
void loop() { // this is the main menu
  startLoop();
  lcd.clear();
  lcd.print("1=alarm 2=night");
  lcd.setCursor(0, 1);
  lcd.print("3=change settings");
  c = myKeypad.waitForKey();
  switch (c) {
    case '1':
      night = 0;
      alarmSet();
    case '2':
      night = 1;
      alarmSet();
    case '3':
      setting = 1;
      alarmSet();
    case 'A':
      c = 0;
      startLoop();
  }
}
void alarmSet() { // this is to set the alarm 
  while (x == 0) {
    if (lock < 3) {
      lcd.clear();
      lcd.print("Enter Passcode: ");
      for (int x = 0; x <= 4; x++) {
resetnewinput:
        c = myKeypad.waitForKey();
        switch (c) {
          case 'A':
            return;
          case 'B':
          case 'C':
          case 'D':
            lcd.clear();
            lcd.print("Invalid Entry");
            goto resetnewinput;
          case '*':
            goto resetnewinput;
          case '#':
            correct = 0;
            for (int x = 0; x < 4; x++) {
              if (inputPassword[x] == password1[x]) {
                correct = correct + 1;
              }
            }
            if (correct == 4) {
              x = 1;
              correct = 0;
              lock = 0;
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Password Correct");
              if (setting == 1) {
                settings();
                return;
              }
              else {
                delay(Delay);
                alarmOn();
              }
            } else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Incorrect Password");
              lock += 1;
              correct = 0;
              delay(2000);
            }
          default:
            inputPassword[x] = c;
        }
      }
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Locked for 5 sec");
      delay(5000);
      lock = 0;
    }
  }
}
void alarmOn() { // this runs when the alarm is on
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alarm Armed");
  if (night == 1) {
    time = millis();
    while (time <= ntime) {
      while (DoorVal == LOW && KitVal == LOW && HallVal == LOW && waterVal == 0) {
        DoorVal = digitalRead(magDoor);  // read input value
        KitVal = digitalRead(Kitchen);
        HallVal = digitalRead(Hallway);
        waterVal = digitalRead(water);
      }
      alarmSound();
    }
  }
  else if (night == 0) {
    while (DoorVal == LOW && KitVal == LOW && HallVal == LOW && BedVal == LOW && waterVal == 0) {
      DoorVal = digitalRead(magDoor);  // read input value
      KitVal = digitalRead(Kitchen);
      HallVal = digitalRead(Hallway);
      BedVal = digitalRead(Bedroom);
      waterVal = digitalRead(water);
    }
    lcd.clear();
    if (DoorVal == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alarm Delay");
      currentMillis = millis();
      previousMillis = currentMillis;
      alarmDelay();
    }
    else {
      alarmSound();
    }
  }
}
void alarmDelay() { //this is when the entry/exit door is opened
  while (currentMillis - previousMillis < Delay) {
    lcd.clear();
    lcd.print("Enter Passcode: ");
    for (int x = 0; x <= 4; x++) {
resetnewinput:
      c = myKeypad.waitForKey();
      switch (c) {
        case 'A':
          return;
        case 'B':
        case 'C':
        case 'D':
          lcd.clear();
          lcd.print("Invalid Entry");
          goto resetnewinput;
        case '*':
          goto resetnewinput;
        case '#':
          for (int x = 0; x < 4; x++) {
            if (inputPassword[x] == password1[x]) {
              x = 1;
              lock = 0;
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Password Correct");
              c = 0;
              startLoop();
            } else {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Incorrect Password");
              delay(2000);
            }
          }
        default:
          inputPassword[x] = c;
      }
    }
  }
  if (currentMillis - previousMillis >= Delay) {
    alarmSound();
  }
}
void alarmSound() { // this is to sound the alarm
  lcd.clear();
  lcd.print("Intruder Detected");
  tone(buzzer, 1000);
  digitalWrite(led, HIGH);
  if (DoorVal == HIGH)Serial.write(5);
  if (waterVal == HIGH)Serial.write(6);
  if (BedVal == HIGH)Serial.write(1);
  if (KitVal == HIGH)Serial.write(3);
  if (HallVal == HIGH)Serial.write(4);
  delay(5000);
  Serial.write(0);
  if (night == 1) {
    DoorVal = digitalRead(magDoor);  // read input value
    KitVal = digitalRead(Kitchen);
    HallVal = digitalRead(Hallway);
    waterVal = digitalRead(water);
    if (KitVal == LOW && DoorVal == LOW && HallVal == LOW && waterVal == 0) {
      digitalWrite(led, LOW);
      noTone(buzzer);
      lcd.clear();
      lcd.print("All Clear");
      delay(1000);
      alarmOn();
    }
  }
  else if (night == 0) {
    DoorVal = digitalRead(magDoor);  // read input value
    BedVal = digitalRead(Bedroom);  // read input value
    KitVal = digitalRead(Kitchen);
    HallVal = digitalRead(Hallway);
    waterVal = digitalRead(water);
    if (BedVal == LOW && KitVal == LOW && DoorVal == LOW && HallVal == LOW && waterVal == 0) {
      digitalWrite(led, LOW);
      noTone(buzzer);
      lcd.clear();
      lcd.print("All Clear");
      delay(1000);
      alarmOn();
    }
  }
}
void settings() { // this is the settings menu
  int a = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1=ch.pas 2=ch.ee");
  lcd.setCursor(0, 1);
  lcd.print("3= ch night");
  c = myKeypad.waitForKey();
  if (c == '1') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter New Pass:");
    for (int x = 0; x < 8; x++) {
resetnewinput:
      if (x == 4) {
        a = 1;
      }
      c = myKeypad.waitForKey();
      switch (c) {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case '*':
        case '#':
          lcd.clear();
          lcd.print("Invalid Entry");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter New Pass:");
          goto resetnewinput;
        default:
          if (a == 1) {
            newpass1 += c;
            lcd.setCursor(x, 1);
            lcd.print("*");
            for (int s = 0; s < 4; s++) {
              lcd.setCursor(s, 1);
              lcd.print(" ");
            }
            break;
          }
          else {
            newpass += c;
            lcd.setCursor(x, 1);
            lcd.print("*");
            break;
          }
      }
    }
    if (newpass == newpass1) {
      int passlen = newpass.length() + 1;
      char newPassword[passlen];
      newpass.toCharArray(newPassword, newpass.length() + 1);
      for (int x = 0; x < 4; x++) {
        password1[x] = newPassword[x];
      }
      password.reset();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("New pass set");
      delay(2000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Incorrect pass");
      delay(2000);
    }
  }
  else if (c == '2') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter new e/e time");
    lcd.setCursor(0, 1);
    lcd.print("Num x 5 sec");
    c = myKeypad.waitForKey();
    Delay = 5000 * c;
    lcd.clear();
  }
  else if (c == 3) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter night time");
    lcd.setCursor(0, 1);
    lcd.print("Num x 1 hour");
    c = myKeypad.waitForKey();
    ntime = 3600000 * c;
    lcd.clear();
  }
}
