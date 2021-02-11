#if !defined(SENSOR_H)
#define SENSOR_H

/*
An abstract Sensor class

*/

class Sensor{

public:
   Sensor(int i2cAddress) : i2cAddress(i2cAddress) {};

private:
   virtual void measure() = 0; // these need to be overwritten
   
   int i2cAddress;
};

#endif // SENSOR_H
