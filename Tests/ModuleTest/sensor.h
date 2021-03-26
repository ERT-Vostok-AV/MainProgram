#if !defined(SENSOR_H)
#define SENSOR_H

/*
An abstract Sensor class

*/

class Sensor{

public:
   Sensor(int i2cAddress) : i2cAddress(i2cAddress) {};
   virtual void measure() = 0; // these need to be overwritten
protected:
  int i2cAddress;
private:
};

#endif // SENSOR_H
