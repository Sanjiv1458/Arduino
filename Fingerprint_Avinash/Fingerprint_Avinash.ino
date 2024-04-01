#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>

// Pin connections
const int registerButton = 2;
const int markAttendanceButton = 3;
const int deleteUserButton = 4;
const int buzzerPin = 5;

// Buzzer duration
const int buzzerDuration = 100;

// LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Fingerprint sensor
SoftwareSerial mySerial(10, 11);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(registerButton, INPUT_PULLUP);
  pinMode(markAttendanceButton, INPUT_PULLUP);
  pinMode(deleteUserButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  finger.begin(57600);
  if (finger.verifyPassword()) {
    lcd.print("Found fingerprint sensor!");
  } else {
    lcd.print("Did not find fingerprint sensor :(");
    while (1);
  }
}

void loop() {
  if (digitalRead(registerButton) == LOW) {
    registerUser();
  } else if (digitalRead(markAttendanceButton) == LOW) {
    markAttendance();
  } else if (digitalRead(deleteUserButton) == LOW) {
    deleteUser();
  }
}

void registerUser() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place Finger");
  delay(2000);

  int id = 1;
  while (finger.checkEnrolled(id) == FINGERPRINT_OK) {
    id++;
  }

  if (id == 127) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Database Full");
    delay(2000);
    return;
  }

  int result = finger.enrollStart(id);
  if (result != FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enroll Failed");
    delay(2000);
    return;
  }

  while (true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place Finger");
    delay(2000);

    result = finger.getImage();
    if (result != FINGERPRINT_OK) {
      continue;
    }

    result = finger.image2Tz(1);
    if (result != FINGERPRINT_OK) {
      continue;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Remove Finger");
    delay(2000);

    result = finger.getImage();
    while (result != FINGERPRINT_NOFINGER) {
      result = finger.getImage();
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Place Same Finger");
    delay(2000);

    result = finger.getImage();
    if (result != FINGERPRINT_OK) {
      continue;
    }

    result = finger.image2Tz(2);
    if (result != FINGERPRINT_OK) {
      continue;
    }

    result = finger.createModel();
    if (result != FINGERPRINT_OK) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enroll Failed");
      delay(2000);
      return;
    }

    result = finger.storeModel(id);
    if (result == FINGERPRINT_OK) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enroll Success");
      lcd.setCursor(0, 1);
      lcd.print("ID: ");
      lcd.print(id);
      delay(2000);
      return;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enroll Failed");
      delay(2000);
      return;
    }
  }
}

void markAttendance() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place Finger");
  delay(2000);

  int result = finger.getImage();
  if (result != FINGERPRINT_OK) {
    return;
  }

  result = finger.image2Tz();
  if (result != FINGERPRINT_OK) {
    return;
  }

  result = finger.fingerFastSearch();
  if (result == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Attendance Marked");
    lcd.setCursor(0, 1);
    lcd.print("ID: ");
    lcd.print(finger.fingerID);
    delay(2000);

    // Sound the buzzer
    digitalWrite(buzzerPin, HIGH);
    delay(buzzerDuration);
    digitalWrite(buzzerPin, LOW);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Not Recognized");
    delay(2000);
  }
}

void deleteUser() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place Finger");
  delay(2000);

  int result = finger.getImage();
  if (result != FINGERPRINT_OK) {
    return;
  }

  result = finger.image2Tz();
  if (result != FINGERPRINT_OK) {
    return;
  }

  result = finger.fingerFastSearch();
  if (result != FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Not Recognized");
    delay(2000);
    return;
  }

  int id = finger.fingerID;

  result = finger.deleteModel(id);
  if (result == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("User Deleted");
    lcd.setCursor(0, 1);
    lcd.print("ID: ");
    lcd.print(id);
    delay(2000);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Delete Failed");
    delay(2000);
  }
}

