#include "mpu6050.h"
#include <Adafruit_MPU6050.h>
#include "Arduino.h"

Adafruit_MPU6050 mpuAda;


Mpu6050::Mpu6050() : Sensor(0x76) {}
sensors_event_t acc, gyro, temp;
double gXOff, gYOff, gZOff, aXOff, aYOff, aZOff;
double currTime, prevTime, deltaTime;
// Ratio to convert radians to degrees.


bool Mpu6050::begin(){
    // Wake up the mpu
    bool succ = mpuAda.begin(i2cAddress);

    //Set settings of mpu
    mpuAda.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpuAda.setGyroRange(MPU6050_RANGE_2000_DEG);
    mpuAda.setFilterBandwidth(MPU6050_BAND_94_HZ);

    degX, degY, degZ = 0;

    getOffsets();

    currTime = millis();
}

// Get the most recent values and stores them.
void Mpu6050::measure(){
    //read the most recent values
    mpuAda.getEvent(&acc, &gyro, &temp);

    // Adjust gyro according to offset
    double gX = gyro.gyro.x;
    double gXTrue = gX - gXOff;
    double gYTrue = gyro.gyro.y - gYOff;
    double gZTrue = gyro.gyro.z - gZOff;
    // Adjust the accelerometer value according to the offset
    double aXTrue = acc.acceleration.x;
    double aYTrue = acc.acceleration.y;
    double aZTrue = acc.acceleration.z;

    // Get the delta time since last measure
    prevTime = currTime;
    currTime = millis();
    deltaTime = (currTime - prevTime) / 1000; //divide by 1000 to get seconds
    Serial.print(gX); Serial.print(" ");
    Serial.print(gXTrue); Serial.print(" ");
    // Increment orientation values
    degX = degX + (gXTrue * deltaTime * RAD_TO_DEG);
    degY = degY + gYTrue * deltaTime * RAD_TO_DEG;
    degZ = degZ + gZTrue * deltaTime * RAD_TO_DEG;
    // Increment velocity values
    velX += aXTrue * deltaTime;
    velY += aYTrue * deltaTime;
    velZ += aZTrue * deltaTime;
    Serial.println(degX);
}

// Compute offset by averarging over a 100 measures.
void Mpu6050::getOffsets(){
    gXOff, gYOff, gZOff = 0;
    aXOff, aYOff, aZOff = 0;
    for(int i =0; i<100; i++){
        mpuAda.getEvent(&acc, &gyro, &temp);

        gXOff += gyro.gyro.x;
        gYOff += gyro.gyro.y;
        gZOff += gyro.gyro.z;

        aXOff += acc.acceleration.x;
        aYOff += acc.acceleration.y;
        aZOff += acc.acceleration.z;

    }
    gXOff /= 100; gYOff /= 100; gZOff /= 100; 
    aXOff /= 100; aYOff /= 100; aZOff /= 100; 
    Serial.print("G offsets: "); Serial.print(gXOff); Serial.print(" "); Serial.print(gYOff); Serial.print(" "); Serial.println(gZOff);
}

//Getter for the velocity and orientation
double Mpu6050::getRotX() {return degX;}
double Mpu6050::getRotY() {return degY;}
double Mpu6050::getRotZ() {return degZ;}

double Mpu6050::getVelX() {return velX;}
double Mpu6050::getVelY() {return velY;}
double Mpu6050::getVelZ() {return velZ;}
