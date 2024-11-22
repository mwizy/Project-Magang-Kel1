#include <NewPing.h>         // Library untuk HC-SR04
#include <Servo.h>           // Library untuk servo
#include <DHT.h>             // Library untuk DHT11
#include <ESP8266WiFi.h>     // Library untuk WiFi

#define TRIGGER_PIN  D5      // Pin Trigger HC-SR04 pada NodeMCU
#define ECHO_PIN     D6      // Pin Echo HC-SR04 pada NodeMCU
#define MAX_DISTANCE 250     // Maksimum Pembacaan Jarak (cm)
#define DHTPIN       D2      // Pin untuk DHT11
#define DHTTYPE      DHT11   // Menentukan tipe DHT11
#define SECRET_CH_ID 2749102 // ThingSpeak Channel ID
#define THINGSPEAK_URL "http://api.thingspeak.com/update"

// Pendefinisian lampu
int lampuMerah = D3;
int lampuKuning = D4;
int lampuHijau = D7;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // Setup HC-SR04
Servo myservo; // Setup nama variabel servo
DHT dht(DHTPIN, DHTTYPE); 

const char* ssid = "[∆]";            // Nama WiFi
const char* password = "fikri2604";  // Password WiFi
const char* apiKey = "0CINTND0XHF3KOR9";  // API Key ThingSpeak

WiFiClient client;                   // Client WiFi untuk ThingSpeak

// Asynccronous 
unsigned long previousSensorMillis = 0;
unsigned long previousThingSpeakMillis = 0;
unsigned long sensorInterval = 3000;  // Interval pembacaan sensor 1 detik
unsigned long thingSpeakInterval = 15000; // Interval pengiriman data ke ThingSpeak 15 detik

float jarak, h, t;

void setup() {
  Serial.begin(9600);         // Memulai komunikasi serial
  myservo.attach(D1);           // Hubungkan servo ke pin D1
  myservo.write(0);             // Set servo ke posisi awal
  dht.begin();                  // Inisialisasi sensor DHT11
  pinMode(lampuMerah, OUTPUT);
  pinMode(lampuKuning, OUTPUT);
  pinMode(lampuHijau, OUTPUT);
  WiFi.mode(WIFI_STA);          // Mode WiFi sebagai Station
  Serial.print("Menghubungkan ke WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // Menghubungkan dengan WIFI yang tersedia
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi Tersambung!");
}

void bacaSensor() {
  jarak = sonar.ping_cm();
  h = dht.readHumidity();
  t = dht.readTemperature();
  
  // Menggerakan servo sesuai dengan kondisi yang diberikan
  if (jarak > 0 && jarak <= 30) { 
    int posisiServo = map(jarak, 0, 30, 180, 0);
    posisiServo = constrain(posisiServo, 0, 180);
    myservo.write(posisiServo);
  } else if (jarak > 40) {
    myservo.write(0);
  }

  if (!isnan(jarak) || !isnan(t) || !isnan(h)) {
    Serial.print("Jarak: ");
    Serial.print(jarak);
    Serial.print(" cm | Suhu: ");
    Serial.print(t);
    Serial.print(" *C | Kelembaban: ");
    Serial.print(h);
    Serial.println(" %");

    if (t > 24) {
      digitalWrite(lampuMerah, HIGH);
      digitalWrite(lampuKuning, LOW);
      digitalWrite(lampuHijau, LOW);
      delay(1000);
    } else if (t >= 24 && t <= 30) {
      digitalWrite(lampuKuning, HIGH);
      digitalWrite(lampuMerah, LOW);
      digitalWrite(lampuHijau, LOW);
      delay(1000);
    } else {
      digitalWrite(lampuHijau, HIGH);
      digitalWrite(lampuMerah, LOW);
      digitalWrite(lampuKuning, LOW);
      delay(1000);
    }
  } else {
    Serial.println("Data sensor tidak valid.");
  }
}

// Fungsi kirim ke ThingSpeak
void kirimThingSpeak() {

  // Membuat URL untuk mengirim data ke ThingSpeak
  String url = String(THINGSPEAK_URL) + "?api_key=" + String(apiKey) +
               "&field1=" + String(jarak, 2) + 
               "&field2=" + String(t, 2) + 
               "&field3=" + String(h, 2);

  Serial.print("Mengirim data ke ThingSpeak: ");
  Serial.println(url);

  if (client.connect("api.thingspeak.com", 80)) {
    client.print("GET " + url + " HTTP/1.1\r\n");
    client.print("Host: api.thingspeak.com\r\n");
    client.print("Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        // Serial.println("Gagal menghubungi ThingSpeak");
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }

    Serial.println("\nData berhasil dikirim ke ThingSpeak.");
    client.stop();
  } else {
    Serial.println("Gagal menghubungi ThingSpeak.");
  }
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousSensorMillis >= sensorInterval) {
    previousSensorMillis = currentMillis;
    bacaSensor();
  }

  if (currentMillis - previousThingSpeakMillis >= thingSpeakInterval) {
    previousThingSpeakMillis = currentMillis;
    kirimThingSpeak();
  }
}
