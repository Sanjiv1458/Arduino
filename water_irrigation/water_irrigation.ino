#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION");
  lcd.setCursor(0, 1);
  lcd.print("SYSTEM IS ON ");
  lcd.print("");
  delay(3000);
  lcd.clear();
}

void loop() {
  int value = analogRead(A0);
  Serial.println(value);
  if (value > 850) {
    digitalWrite(3, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is ON ");
  } else {
    digitalWrite(3, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Water Pump is OFF");
  }

  if (value < 300) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture : HIGH");
  } else if (value > 300 && value < 850) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture : MID ");
  } else if (value > 850) {
    lcd.setCursor(0, 1);
    lcd.print("Moisture : LOW ");
  }
}
