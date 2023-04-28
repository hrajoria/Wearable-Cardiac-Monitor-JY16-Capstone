#include <Arduino.h>
#include <Wire.h>

int ADCin = A5;
float mv_per_lsb = 3600.0F/1024.0F; // 10-bit ADC with 3.6V input range

void setup() {
  // put your setup code here, to run once:
  pinMode(13,INPUT);
  Serial.begin(9600);


}

void loop() {
  float adcvalue = analogRead(ADCin);

  // Display the results
  //Serial.print(adcvalue);
  //Serial.print(" [");
  Serial.println((float)adcvalue * mv_per_lsb);
  //Serial.println(" mV]");

  delay(10);
}
