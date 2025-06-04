///@see https://medium.com/data-science/ultra-tinyml-machine-learning-for-8-bit-microcontroller-9ec8f7c8dd12

#include "Adafruit_MPU6050.h"

#define NUM_SAMPLES 50
Adafruit_MPU6050 mpu;
void setup() {
  // init serial port
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  // init IMU sensor
  if (!mpu.begin()) {
    while (1) {
      delay(10);
    }
  }
  
  // configure IMU sensor
  // [...]
  // print the CSV header (ax0,ay0,az0,...,gx49,gy49,gz49,target)
  for (int i=0; i<NUM_SAMPLES; i++) {
    Serial.print("aX");
    Serial.print(i);
    Serial.print(",aY");
    Serial.print(i);
    Serial.print(",aZ");
    Serial.print(i);
    Serial.print(",gX");
    Serial.print(i);
    Serial.print(",gY");
    Serial.print(i);
    Serial.print(",gZ");
    Serial.print(i);
    Serial.print(",");
  }
  Serial.println("target");
}

#define NUM_GESTURES    30
#define GESTURE_0       0
#define GESTURE_1       1
#define GESTURE_TARGET  GESTURE_0 
#define ACC_THRESHOLD   30
//#define GESTURE_TARGET  GESTURE_1 
uint8_t gesturesRead = 0;
uint8_t samplesRead = NUM_SAMPLES;
void loop() {
  sensors_event_t a, g, temp;
  
  while(gesturesRead < NUM_GESTURES) {
    // wait for significant motion
    while (samplesRead >= NUM_SAMPLES) {
      // read the acceleration data
      mpu.getEvent(&a, &g, &temp);
      
      // sum up the absolutes
      float aSum = fabs(a.acceleration.x) + 
                   fabs(a.acceleration.y) + 
                   fabs(a.acceleration.z);
      // check if it's above the threshold
      if (aSum >= ACC_THRESHOLD) {
        Serial.println(aSum);
        // reset the sample read count
        samplesRead = 0;
        break;
      }
    }
  
    // read samples of the detected motion
    while (samplesRead < NUM_SAMPLES) {
        // read the acceleration and gyroscope data
        mpu.getEvent(&a, &g, &temp);
  
        samplesRead++;
  
        // print the sensor data in CSV format
        Serial.print(a.acceleration.x, 3);
        Serial.print(',');
        Serial.print(a.acceleration.y, 3);
        Serial.print(',');
        Serial.print(a.acceleration.z, 3);
        Serial.print(',');
        Serial.print(g.gyro.x, 3);
        Serial.print(',');
        Serial.print(g.gyro.y, 3);
        Serial.print(',');
        Serial.print(g.gyro.z, 3);
        Serial.print(',');

        // print target at the end of samples acquisition
        if (samplesRead == NUM_SAMPLES) {
          Serial.println(GESTURE_TARGET);
        }
        
        delay(10);
    }

    gesturesRead++;
  }
  int j = 0;
}
