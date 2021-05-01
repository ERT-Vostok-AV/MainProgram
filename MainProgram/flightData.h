#if !defined(FLIGHT_DATA_H)
#define FLIGHT_DATA_H

struct FlightData {
    double batteryLevel;
    double altitude;
    double temperature;

    //accelX, accelY, accelZ
    double acceleration[3];
    //rotA, rotX, rotY, rotZ
    double rotation[4];
};


#endif // FLIGHT_DATA_H
