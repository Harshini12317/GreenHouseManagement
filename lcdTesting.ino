#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Change address if needed (0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Long text for scrolling
String names = "Harshini, Drashti, Khushboo, Nidhi, Vaishali, Pal   ";

void setup() {
  // Initialize I2C for ESP32
  Wire.begin(21, 22);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // First line
  lcd.setCursor(0, 0);
  lcd.print("AI Project");

  delay(1000);
}

void loop() {

  // Scroll names on second line
  for (int i = 0; i < names.length() - 15; i++) {
    lcd.setCursor(0, 1);
    lcd.print(names.substring(i, i + 16));
    delay(300); // speed of scrolling
  }

  delay(1000);

  // Optional: small pause and clear effect
  lcd.setCursor(0, 1);
  lcd.print("                "); // clear line
  delay(500);
}
