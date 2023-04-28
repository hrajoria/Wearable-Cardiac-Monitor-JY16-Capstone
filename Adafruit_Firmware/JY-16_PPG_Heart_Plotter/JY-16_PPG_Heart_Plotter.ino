/*
  Heart beat plotting!
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 20th, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  Shows the user's heart beat on Arduino's serial plotter

  Instructions:
  1) Load code onto Redboard
  2) Attach sensor to your finger with a rubber band (see below)
  3) Open Tools->'Serial Plotter'
  4) Make sure the drop down is set to 115200 baud
  5) Checkout the blips!
  6) Feel the pulse on your neck and watch it mimic the blips

  It is best to attach the sensor to your finger using a rubber band or other tightening
  device. Humans are generally bad at applying constant pressure to a thing. When you
  press your finger against the sensor it varies enough to cause the blood in your
  finger to flow differently which causes the sensor readings to go wonky.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/

#include <Wire.h>
#include "MAX30105.h"
#include <SPI.h>
#include <SD.h>

#include "heartRate.h"

MAX30105 particleSensor;
File myFile;
bool SDdetected;
int count;

void writeToSD(int IR, int Red){
  myFile.print(IR); myFile.print(","); myFile.print(Red);
  myFile.println();
}

void setup()
{
  count = 0;
  Serial.begin(9600);
  Serial.println("Initializing...");

   Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("No SD card failed!");
    //while (1);
    SDdetected = false;
  }
  else{
    myFile = SD.open("PPG.txt", FILE_WRITE);
    SDdetected = true;
  }
  Serial.println("initialization done.");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }  

  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  //particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  particleSensor.setSampleRate(sampleRate);


  //Arduino plotter auto-scales annoyingly. To get around this, pre-populate
  //the plotter with 500 of an average reading from the sensor

  //Take an average of IR readings at power up
  /*const byte avgAmount = 64;
  long baseValue = 0;
  for (byte x = 0 ; x < avgAmount ; x++)
  {
    baseValue += particleSensor.getIR(); //Read the IR value
  }
  baseValue /= avgAmount;

  //Pre-populate the plotter so that the Y scale is close to IR values
  for (int x = 0 ; x < 500 ; x++)
    Serial.println(baseValue);*/
}

void loop()
{
  if(count == 100){
    Serial.println("Done collecting samples");
    myFile.close();
    while(1);
  }

  
  
  //Serial.println(particleSensor.getIR()); //Send raw data to plotter  
  Serial.print("IR:"); Serial.print(particleSensor.getIR());
  Serial.print(",");
  Serial.print("Red:"); Serial.print(particleSensor.getRed());
  //Serial.print(",");
  //Serial.print("BeatCheck:"); Serial.print(checkForBeat(particleSensor.getIR()));
  Serial.println();

  if(SDdetected){
    Serial.println(count);
    writeToSD(particleSensor.getIR(), particleSensor.getRed());
  }
  count++;

  //Serial.print("Green:"); Serial.print(particleSensor.getGreen());
  
  //delay(100);
}
