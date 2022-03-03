#include "Arduino.h"

#include "../include/sensor_stream.h"

#include "Arduino_BHY2.h"
#if ENABLE_ACCEL
SensorXYZ accel(SENSOR_ID_ACC_RAW);
#endif
#if ENABLE_GYRO
SensorXYZ gyro(SENSOR_ID_GYRO_RAW);
#endif
#if ENABLE_MAG
SensorXYZ mag(SENSOR_ID_MAG_RAW);
#endif
#if ENABLE_TEMP
Sensor temp(SENSOR_ID_TEMP);
#endif
#if ENABLE_HUMID
Sensor humid(SENSOR_ID_HUM);
#endif
#if ENABLE_BARO
Sensor baro(SENSOR_ID_BARO);
#endif
#if ENABLE_GAS
Sensor gas(SENSOR_ID_GAS);
#endif

uint16_t actual_odr;

#if ENABLE_ACCEL || ENABLE_GYRO || ENABLE_MAG || ENABLE_TEMP || ENABLE_HUMID || ENABLE_BARO || ENABLE_GAS
int16_t sensorDataCol[MAX_SAMPLES_PER_PACKET * MAX_NUMBER_OF_COLUMNS];

int16_t* get_sensor_data_buffer()
{
    return &sensorDataCol[0];
}

int setup_sensors(JsonDocument& config_message, int column_start)
{
    int column_index = column_start;
    // Set units.

#if ENABLE_ACCEL && (ENABLE_GYRO == 0)
    accel.configure(ODR_ACC, 0);

    config_message["column_location"]["AccelerometerX"] = column_index++;
    config_message["column_location"]["AccelerometerY"] = column_index++;
    config_message["column_location"]["AccelerometerZ"] = column_index++;
    actual_odr                                          = ODR_ACC;
    config_message["sample_rate"]                       = actual_odr;

#elif (ENABLE_ACCEL && ENABLE_GYRO)
    accel.configure(ODR_ACC, 0);
    gyro.configure(ODR_GYR, 0);
    actual_odr                                          = ODR_ACC;
#if CFG_IMITATE_NANO33BLE
    config_message["sample_rate"]                       = 119;
#else
    config_message["sample_rate"]                       = actual_odr;
#endif
    config_message["column_location"]["AccelerometerX"] = column_index++;
    config_message["column_location"]["AccelerometerY"] = column_index++;
    config_message["column_location"]["AccelerometerZ"] = column_index++;
    config_message["column_location"]["GyroscopeX"]     = column_index++;
    config_message["column_location"]["GyroscopeY"]     = column_index++;
    config_message["column_location"]["GyroscopeZ"]     = column_index++;
    actual_odr                                          = ODR_GYR;
#else  // gyro only
    gyro.configure(ODR_GYR, 0);
    actual_odr                                      = ODR_GYR;
    config_message["sample_rate"]                   = actual_odr;
    config_message["column_location"]["GyroscopeX"] = column_index++;
    config_message["column_location"]["GyroscopeY"] = column_index++;
    config_message["column_location"]["GyroscopeZ"] = column_index++;
#endif

#if ENABLE_MAG
    mag.configure(ODR_MAG, 0);
    actual_odr                                      = ODR_MAG;
    config_message["sample_rate"]                   = actual_odr;
    config_message["column_location"]["MagnetometerX"] = column_index++;
    config_message["column_location"]["MagnetometerY"] = column_index++;
    config_message["column_location"]["MagnetometerZ"] = column_index++;
#endif

#if ENABLE_TEMP
    mag.configure(ODR_TEMP, 0);
    actual_odr                                      = ODR_TEMP;
    config_message["sample_rate"]                   = actual_odr;
    config_message["column_location"]["MagnetometerX"] = column_index++;
    config_message["column_location"]["MagnetometerY"] = column_index++;
    config_message["column_location"]["MagnetometerZ"] = column_index++;
#endif


    return column_index;
}

int update_sensor_data_col(int startIndex)
{
    int index = startIndex;
#if ENABLE_ACCEL
    sensorDataCol[index++] = accel.x();
    sensorDataCol[index++] = accel.y();
    sensorDataCol[index++] = accel.z();
#endif

#if ENABLE_GYRO
    sensorDataCol[index++] = gyro.x();
    sensorDataCol[index++] = gyro.y();
    sensorDataCol[index++] = gyro.z();
#endif

#if ENABLE_MAG
    sensorDataCol[index++] = mag.x();
    sensorDataCol[index++] = mag.y();
    sensorDataCol[index++] = mag.z();
#endif

    return index;
}

#endif
