#include <DHT.h>
#define DHTPIN 2    // Pin sensor DHT11
#define DHTTYPE DHT11

#define TRIG_PIN 3 // Pin Trig sensor ultrasonik
#define ECHO_PIN 4 // Pin Echo sensor ultrasonik

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Membaca data dari sensor DHT11
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Membaca data dari sensor ultrasonik
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // Mengirim data ke serial monitor
  Serial.print("Temperature:"); Serial.print(temp); Serial.print(",");
  Serial.print("Humidity:"); Serial.print(humidity); Serial.print(",");
  Serial.print("Distance:"); Serial.println(distance);

  delay(1000);
}
