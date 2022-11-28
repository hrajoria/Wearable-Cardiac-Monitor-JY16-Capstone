
// Demo for getting individual unified sensor data from the ICM20649


#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

Adafruit_ICM20649 icm;
Adafruit_Sensor *icm_temp, *icm_accel, *icm_gyro;

#define ICM_CS 5
// For software-SPI mode we need SCK/MOSI/MISO pins
#define ICM_SCK 13
#define ICM_MISO 12
#define ICM_MOSI 11


void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit ICM20649 test!");

  if (!icm.begin_I2C()) {
     if (!icm.begin_SPI(ICM_CS)) {
     if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {
    Serial.println("Failed to find ICM20649 chip");
    while (1) {
      delay(10);
    }
     }
     }
  }

  Serial.println("ICM20649 Found!");
  icm_temp = icm.getTemperatureSensor();
  icm_temp->printSensorDetails();

  icm_accel = icm.getAccelerometerSensor();
  icm_accel->printSensorDetails();

  icm_gyro = icm.getGyroSensor();
  icm_gyro->printSensorDetails();
}

void loop() {
  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  icm_temp->getEvent(&temp);
  icm_accel->getEvent(&accel);
  icm_gyro->getEvent(&gyro);

  /*Serial.print("\t\tTemperature ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  /*Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  /*Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z);
  Serial.println(" radians/s ");
  Serial.println();

  delay(100);*/

  //  serial plotter friendly format
  //Serial.print(temp.temperature);
  //Serial.print(",");
  
  Serial.print("Acceleration_x:"); Serial.print(accel.acceleration.x);
  Serial.print(","); 
  Serial.print("Acceleration_y:"); Serial.print(accel.acceleration.y);
  Serial.print(","); 
  Serial.print("Acceleration_z:"); Serial.print(accel.acceleration.z);
  Serial.print(",");
  Serial.print("Magnitude:"); Serial.print(sqrt((accel.acceleration.x * accel.acceleration.x) + 
  (accel.acceleration.y * accel.acceleration.y) + (accel.acceleration.z * accel.acceleration.z)));
  Serial.print(",");
  Serial.print("Gyro_x:"); Serial.print(gyro.gyro.x);
  Serial.print(","); 
  Serial.print("Gyro_y:"); Serial.print(gyro.gyro.y);
  Serial.print(","); 
  Serial.print("Gyro_z:"); Serial.print(gyro.gyro.z);
  Serial.println();
  delay(100);
}



