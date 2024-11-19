#include <Servo.h>

#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float temperature, humidity;

void setup(){
  dht.begin();
}

void bacaSensor(){
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if(!isnan(temperature) && !isnan(humidity)){
    serial.print("Suhu: ");
    serial.print(temperature);
    serial.print(" *C | Kelembaban: ");
    serial.print(humidity);
    serial.println(" %");
  } else {
    serial.println("Data sensor tidak valid");
  }
}