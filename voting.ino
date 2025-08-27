#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Voting buttons
const int cand1Btn = 22;
const int cand2Btn = 23;
const int cand3Btn = 24;
const int cand4Btn = 25;
const int resultBtn = 26;

// Vote counters
int cand1Votes = 0;
int cand2Votes = 0;
int cand3Votes = 0;
int cand4Votes = 0;

// Keypad setup
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {30, 31, 32, 33}; 
byte colPins[COLS] = {34, 35, 36, 37}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Passwords
String voterPIN = "0903070501900275";   // PIN to allow voting
String adminPIN = "1980";   // PIN for results
String inputPIN = "";

// States
bool verified = false;  // Access control

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Voting Machine");
  lcd.setCursor(0,1);
  lcd.print("Enter PIN");

  pinMode(cand1Btn, INPUT_PULLUP);
  pinMode(cand2Btn, INPUT_PULLUP);
  pinMode(cand3Btn, INPUT_PULLUP);
  pinMode(cand4Btn, INPUT_PULLUP);
  pinMode(resultBtn, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // Keypad handling
  char key = keypad.getKey();
  if (key) {
    if (key == '#') { // Check PIN
      if (inputPIN == voterPIN) {
        verified = true;
        lcd.clear();
        lcd.print("Access Granted");
        delay(1000);
        lcd.clear();
        lcd.print("Ready to Vote");
      } 
      else if (inputPIN == adminPIN) {
        lcd.clear();
        lcd.print("Admin Access");
        delay(1000);
        showResults();
      }
      else {
        lcd.clear();
        lcd.print("Access Denied");
        delay(1000);
        lcd.clear();
        lcd.print("Enter PIN");
      }
      inputPIN = ""; // reset after check
    }
    else if (key == '*') {
      inputPIN = ""; // clear entry
      lcd.clear();
      lcd.print("Enter PIN");
    }
    else {
      inputPIN += key;
      lcd.setCursor(0,1);
      lcd.print(inputPIN);
    }
  }

  // Voting only if verified
  if (verified) {
    if (digitalRead(cand1Btn) == LOW) {
      cand1Votes++;
      showVote("Candidate 1");
    }
    if (digitalRead(cand2Btn) == LOW) {
      cand2Votes++;
      showVote("Candidate 2");
    }
    if (digitalRead(cand3Btn) == LOW) {
      cand3Votes++;
      showVote("Candidate 3");
    }
    if (digitalRead(cand4Btn) == LOW) {
      cand4Votes++;
      showVote("Candidate 4");
    }
  }
}

void showVote(const char* candidate) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Vote Recorded");
  lcd.setCursor(0,1);
  lcd.print(candidate);
  delay(1000);
  lcd.clear();
  lcd.print("Ready to Vote");
}

void showResults() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Sarion:"); lcd.print(cand1Votes);
  lcd.print(" Rizam:"); lcd.print(cand2Votes);

  lcd.setCursor(0,1);
  lcd.print("Nizam:"); lcd.print(cand3Votes);
  lcd.print(" Norman:"); lcd.print(cand4Votes);

  delay(4000);
  lcd.clear();
  lcd.print("Enter PIN");
  verified = false; // reset after admin check
}
