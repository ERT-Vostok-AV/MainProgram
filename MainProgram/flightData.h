#if !defined(FLIGHT_DATA_H)
#define FLIGHT_DATA_H

struct FlightData {
    double batteryLevel;
    double altitude;
    double temperature;

    //velX, velY, velZ
    double velocity[3];
    //rotA, rotX, rotY, rotZ
    double rotation[4];
};


#endif // FLIGHT_DATA_H
