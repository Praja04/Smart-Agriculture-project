#define BLYNK_TEMPLATE_ID "TMPL60-1Vu5En"
#define BLYNK_TEMPLATE_NAME "SENSOR KELEMBAPAN TANAH"
#define BLYNK_AUTH_TOKEN "pVZd5vU8cC_pEYApya1SuObmx_umVBoC"

#define BLYNK_PRINT Serial

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define DHTPIN 15        // Pin data pada sensor DHT22
#define DHTTYPE DHT22    // Tipe sensor DHT22
#define SOIL_PIN 34      // Pin data pada soil sensor

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 4);  // Alamat I2C dan ukuran LCD (16x2)

#define RELAY_PIN 18     // Pin kontrol pada relay

BLYNK_WRITE(V3) {  // Mendapatkan nilai dari button switch di Blynk
  int switchValue = param.asInt();
  if (switchValue == 1) {
    digitalWrite(RELAY_PIN, HIGH);  // Mengaktifkan relay
    Serial.println("Relay diaktifkan");
    Blynk.logEvent("relay", "Relay diaktifkan");
  }else{
    digitalWrite(RELAY_PIN, LOW);   // Mematikan relay
    Serial.println("Relay dimatikan");
    Blynk.logEvent("relay", "Relay dimatikan");
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run();

  delay(2000);  

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilMoisture = analogRead(SOIL_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca data dari sensor DHT22!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C, Kelembaban: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Kelembaban Tanah: ");
  Serial.println(soilMoisture);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Kelembaban: ");
  lcd.print(humidity);
  lcd.print(" %");

  lcd.setCursor(0, 2);
  lcd.print("Kelembaban Tanah:");
  lcd.setCursor(0, 3);
  lcd.print(soilMoisture);

  // Mengontrol relay berdasarkan suhu
  if (temperature > 28.0) {
    digitalWrite(RELAY_PIN, HIGH);  // Mengaktifkan relay
    Serial.println("Relay diaktifkan.");
  } else {
    digitalWrite(RELAY_PIN, LOW);   // Mematikan relay
    Serial.println("Relay dimatikan.");
  }

  Blynk.virtualWrite(V1, temperature); 
  Blynk.virtualWrite(V2, humidity);    
  Blynk.virtualWrite(V0, soilMoisture); 
}