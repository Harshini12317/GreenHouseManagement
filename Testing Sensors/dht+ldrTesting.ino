#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// DHT22
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LDR
#define LDR_PIN 34

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  dht.begin();

  lcd.setCursor(0,0);
  lcd.print("System Starting");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int light = analogRead(LDR_PIN);

  // Check sensor error
  if (isnan(temp) || isnan(hum)) {
    lcd.setCursor(0,0);
    lcd.print("DHT Error      ");
    lcd.setCursor(0,1);
    lcd.print("Check Sensor   ");
    delay(2000);
    return;
  }

  // Display Temperature
  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C ");

  // Display Humidity
  lcd.print("H:");
  lcd.print(hum);
  lcd.print("% ");

  // Display Light (second line)
  lcd.setCursor(0,1);
  lcd.print("Light:");
  lcd.print(light);
  lcd.print("   ");

  // Serial monitor (optional)
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C  Hum: ");
  Serial.print(hum);
  Serial.print(" %  Light: ");
  Serial.println(light);

  delay(2000);
}
