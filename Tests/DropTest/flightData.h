#if !defined(FLIGHT_DATA_H)
#define FLIGHT_DATA_H

struct FlightData {
    double batteryLevel;
    double altitude;
    double temperature;

    //x, y, z
    double velocity[3];
    double rotation[3];
};


#endif // FLIGHT_DATA_H
