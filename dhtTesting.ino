#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// LCD (change 0x27 to 0x3F if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT22 setup
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Check if reading failed
  if (isnan(temp) || isnan(hum)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error   ");
    lcd.setCursor(0, 1);
    lcd.print("Check DHT22    ");
    delay(2000);
    return;
  }

  // Display Temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223); // degree symbol
  lcd.print("C   ");

  // Display Humidity
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(hum);
  lcd.print("%    ");

  delay(2000);
}
