#include "mpu6050.h"
#include <MPU6050_light.h>
#include "Arduino.h"

MPU6050 mpuAda(Wire);

#define radToDeg 57.2958

Mpu6050::Mpu6050() : Sensor(0x68) {}
double gXOff, gYOff, gZOff, aXOff, aYOff, aZOff;
double currTime, prevTime, deltaTime;
// Ratio to convert radians to degrees.


bool Mpu6050::begin(){
    // Wake up the mpu
    mpuAda.begin();

    degX, degY, degZ = 0;
    velX, velY, velZ = 0;

    mpuAda.calcGyroOffsets();
}

// Get the most recent values and stores them.
void Mpu6050::measure(){

    /*
    //read the most recent values
    mpuAda.getEvent(&acc, &g, &temp);

    // Adjust gyro according to offset
    double gX = g.gyro.x;
    double gXTrue = gX - gXOff;
    double gYTrue = g.gyro.y - gYOff;
    double gZTrue = g.gyro.z - gZOff;
    // Adjust the accelerometer value according to the offset
    double aXTrue = acc.acceleration.x;
    double aYTrue = acc.acceleration.y;
    double aZTrue = acc.acceleration.z;
    //Serial.print(gX); Serial.print(" ");
    //Serial.print(gXTrue); Serial.print(" ");
    // Get the delta time since last measure
    prevTime = currTime;
    currTime = millis();
    deltaTime = (currTime - prevTime) / 1000; //divide by 1000 to get seconds
    // Increment orientation values
    degX = degX + gXTrue * deltaTime * radToDeg;
    //Serial.print(degX); Serial.println(" ");
    degY = degY + gYTrue * deltaTime * radToDeg;
    degZ = degZ + gZTrue * deltaTime * radToDeg;
    // Increment velocity values
    velX += aXTrue * deltaTime;
    velY += aYTrue * deltaTime;
    velZ += aZTrue * deltaTime;
    */
    mpuAda.update();
    
    degX = mpuAda.getAngleX();
    degY = mpuAda.getAngleY();
    degZ = mpuAda.getAngleZ();

    velX = mpuAda.getAccX();
    velY = mpuAda.getAccY();
    velZ = mpuAda.getAccZ();
    //TODO change to velocity
}

// Compute offset by averarging over a 100 measures.
void Mpu6050::getOffsets(){
    /**
    gXOff, gYOff, gZOff = 0;
    aXOff, aYOff, aZOff = 0;
    for(int i =0; i<100; i++){
        mpuAda.getEvent(&acc, &g, &temp);

        gXOff += g.gyro.x;
        gYOff += g.gyro.y;
        gZOff += g.gyro.z;

        aXOff += acc.acceleration.x;
        aYOff += acc.acceleration.y;
        aZOff += acc.acceleration.z;

    }
    gXOff /= 100; gYOff /= 100; gZOff /= 100; 
    aXOff /= 100; aYOff /= 100; aZOff /= 100; 
    Serial.print("G offsets: "); Serial.print(gXOff); Serial.print(" "); Serial.print(gYOff); Serial.print(" "); Serial.println(gZOff);
    */
}

//Getter for the velocity and orientation
double Mpu6050::getRotX() {return degX;}
double Mpu6050::getRotY() {return degY;}
double Mpu6050::getRotZ() {return degZ;}

double Mpu6050::getVelX() {return velX;}
double Mpu6050::getVelY() {return velY;}
double Mpu6050::getVelZ() {return velZ;}
