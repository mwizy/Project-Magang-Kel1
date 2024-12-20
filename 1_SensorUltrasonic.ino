#include <NewPing.h> //Library untuk HC-SR04

#define TRIGGER_PIN  D5  //Pin Trigger HC-SR04 pada NodeMCU
#define ECHO_PIN     D6  //Pin Echo HC-SR04 pada NodeMCU
#define MAX_DISTANCE 250 //Maksimum Pembacaan Jarak (cm)
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //Setup Pin HC-SR04 dan Jarak Pembacaan dengan Fungsi Library
 
void setup() {
  Serial.begin(115200); //Memulai Komunikasi Serial Monitor
}
 
void loop() {   
  float jarak = sonar.ping_cm(); //Melakukan Pembacaan Jarak dalam bentuk cm
  //Mencetak Hasil Pembacaan pada Serial Monitor          
  Serial.println("Monitoring Jarak");
  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");
  delay(1000); //Delay 1 Detik
}