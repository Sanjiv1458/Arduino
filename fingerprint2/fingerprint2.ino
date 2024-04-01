#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>

// Initialize LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize fingerprint sensor
SoftwareSerial fingerSerial(10, 11);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

// Define pin numbers for buttons and buzzer
const int BUTTON_REGISTER = 2;
const int BUTTON_MARK_ATTENDANCE = 3;
const int BUTTON_DELETE = 4;
const int BUZZER_PIN = 6;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize fingerprint sensor
  finger.begin(57600);

  // Set button and buzzer pins as inputs/outputs
  pinMode(BUTTON_REGISTER, INPUT_PULLUP);
  pinMode(BUTTON_DELETE, INPUT_PULLUP);
  pinMode(BUTTON_MARK_ATTENDANCE, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize LCD display
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Display welcome message on LCD
  displayWelcomeMessage();

  // Check if the fingerprint sensor is connected
  if (!finger.verifyPassword()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fingerprint Sensor");
    lcd.setCursor(0, 1);
    lcd.print("Not Found!");
    Serial.println("Did not find fingerprint sensor!");
    while (1) {
      delay(1);
    }
  }
}

void loop() {
  // Check if register button is pressed
  if (digitalRead(BUTTON_REGISTER) == LOW) {
    registerFingerprint();
  }

  // Check if delete button is pressed
  if (digitalRead(BUTTON_DELETE) == LOW) {
    deleteFingerprint();
  }

  // Check if mark attendance button is pressed
  if (digitalRead(BUTTON_MARK_ATTENDANCE) == LOW) {
    markAttendance();
  }
}

// Display welcome message on LCD
void displayWelcomeMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to FAS");
  lcd.setCursor(0, 1);
  lcd.print("Press a button");
}

// Register a new fingerprint
void registerFingerprint() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place finger on");
  lcd.setCursor(0, 1);
  lcd.print("sensor to register");

  int fingerprintID = -1;
  while (fingerprintID == -1) {
    int result = finger.getImage();
    if (result == FINGERPRINT_OK) {
      result = finger.image2Tz();
      if (result == FINGERPRINT_OK) {
        result = finger.createModel();
        if (result == FINGERPRINT_OK) {
          result = finger.storeModel(fingerprintID);
          if (result == FINGERPRINT_OK) {
            lcd.clear();
            lcd.print("Fingerprint stored");
            beep();
            delay(1000);
          } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error storing");
            lcd.setCursor(0, 1);
            lcd.print("fingerprint");
            delay(1000);
          }
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Error creating");
          lcd.setCursor(0, 1);
          lcd.print("model");
          delay(1000);
        }
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error converting");
        lcd.setCursor(0, 1);
        lcd.print("image");
        delay(1000);
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error capturing");
      lcd.setCursor(0, 1);
      lcd.print("image");
      delay(1000);
    }
  }
}

// Delete an existing fingerprint
void deleteFingerprint() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place finger on");
  lcd.setCursor(0, 1);
  lcd.print("sensor to delete");

  while (true) {
    int result = finger.getImage();
    if (result == FINGERPRINT_OK) {
      result = finger.image2Tz();
      if (result == FINGERPRINT_OK) {
        result = finger.fingerFastSearch();
        if (result == FINGERPRINT_OK) {
          result = finger.deleteModel(result);
          if (result == FINGERPRINT_OK) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Fingerprint deleted");
            beep();
            delay(1000);
            break;
          } else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Error deleting");
            lcd.setCursor(0, 1);
            lcd.print("fingerprint");
            delay(1000);
            break;
          }
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fingerprint not");
          lcd.setCursor(0, 1);
          lcd.print("found");
          delay(1000);
          break;
        }
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error converting");
        lcd.setCursor(0, 1);
        lcd.print("image");
        delay(1000);
      }
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error capturing");
      lcd.setCursor(0, 1);
      lcd.print("image");
      delay(1000);
    }
  }
}

// Mark attendance using fingerprint
void markAttendance() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place finger on");
  lcd.setCursor(0, 1);
  lcd.print("sensor to mark");
  delay(1000);

  while (true) {
    int result = finger.getImage();
    if (result == FINGERPRINT_OK) {
      result = finger.image2Tz();
      if (result == FINGERPRINT_OK) {
        int id = finger.fingerFastSearch();
        if (id == FINGERPRINT_OK) {
          // Fingerprint found
          lcd.clear();
          lcd.print("Attendance marked");
          beep();
          delay(1000);
          break;
        } else {
          // Fingerprint not found
          lcd.clear();
          lcd.print("Fingerprint not");
          lcd.setCursor(0, 1);
          lcd.print("recognized");
          delay(1000);
          break;
        }
      } else {
        // Error converting image
        lcd.clear();
        lcd.print("Error converting");
        lcd.setCursor(0, 1);
        lcd.print("image");
        delay(1000);
      }
    } else {
      // Error capturing image
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error capturing");
      lcd.setCursor(0, 1);
      lcd.print("image");
      delay(1000);
    }
  }
}

// Make a beep sound using the buzzer
void beep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}
