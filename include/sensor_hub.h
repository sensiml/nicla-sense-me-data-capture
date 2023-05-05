#ifndef __SENSOR_CONFIG_H__
#define __SENSOR_CONFIG_H__

#include <ArduinoJson.h>
#include "Arduino.h"
#include "Arduino_BHY2.h"
#include "Nicla_System.h" //must be placed after "ArduinoJSon.h"
//
// Most commonly used configurations to consider:
// - USE_BLE
// - SERIAL_BAUD_RATE
// - ODR_IMU_MAX / ODR_IMU

#define USE_BLE 0 // set to 1 to use BLE and 0 to use serial com (tty) port

/**
 * Serial Port Settings
 */

// by default: it seems that DCL uses 115200 and open gateway uses 115200 * 4 (460800)
// when used with open gateway, change the value to 115200 * 4 (460800)
// when used with DCL, change the value to 115200 * 4 (most suggested) or 115200 * 8
#define SERIAL_BAUD_RATE 115200 * 4

// this is used for debugging when BLE is used
#define SERIAL_BAUD_RATE_DEFAULT 115200

// odr of acc and gyro could be different, but we prefer to be the same for most cases
// supported ODRs (output data rate) of the IMU inside the BHI260 chip:
// 12, 25, 50, 100, 200, 400, 800, 1600

// when using BLE: suggest to set up to 100
// when using serial: suggest to set up to 400, this depends on the performance of the PC being used as well
#define ODR_IMU 400

#define USE_SECOND_SERIAL_PORT_FOR_OUTPUT 0
#define CFG_IMITATE_NANO33BLE 0

#define CFG_LED_ON_DURATION 50
#define CFG_LED_OFF_DURATION 3000

#define ENABLE_ACCEL 1
#define ENABLE_GYRO 1
#define ENABLE_MAG 0
#define ENABLE_TEMP 0
#define ENABLE_HUMID 0
#define ENABLE_BARO 0 // ambient pressure sensor
#define ENABLE_GAS 0

#define ENABLE_AUDIO 0

#define ODR_ACC ODR_IMU
#define ODR_GYR ODR_IMU
#define ODR_MAG 25
#define ODR_TEMP 1
#define ODR_HUMID 1
#define ODR_BARO 1
#define ODR_GAS 1

#define ENABLE_RECOGNITION 1

const int WRITE_BUFFER_SIZE = 256; // about 164B

typedef unsigned long time_ms_t;

/**
 * BLE Settings
 */

#if USE_BLE
#define MAX_NUMBER_OF_COLUMNS 10
#define MAX_SAMPLES_PER_PACKET 1
#else
#define MAX_NUMBER_OF_COLUMNS 20
#define MAX_SAMPLES_PER_PACKET 6
#endif // USE_BLE

#define DELAY_BRDCAST_JSON_DESC_SENSOR_CONFIG 1000

#if ENABLE_ACCEL || ENABLE_GYRO || ENABLE_MAG || ENABLE_TEMP || ENABLE_HUMID || ENABLE_BARO || ENABLE_GAS
int setup_sensors(JsonDocument &config_message, int column_start);
int16_t *get_sensor_data_buffer();
int update_sensor_data_col(int startIndex);
#endif // #if ENABLE_ACCEL || ENABLE_GYRO || ENABLE_MAG

void sml_output_results(uint16_t model, uint16_t classification);
void sml_recognition_run(signed short *data, int num_sensors);
void setLedColor(uint32_t color);

#endif //__SENSOR_CONFIG_H__
