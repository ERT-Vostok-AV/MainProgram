/*
An abstract Sensor class



*/


class Sensor{

public:
   Sensor(int i2cAddress) : i2cAddress(i2cAddress) {};
   // virtual void init() = 0; we sure about that one chief?
   virtual void measure() = 0; // these need to be overwritten

private:
   int i2cAddress;
};
