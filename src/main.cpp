/*
 * This sketch shows how nicla can be used in standalone mode.
 * Without the need for an host, nicla can run sketches that
 * are able to configure the bhi sensors and are able to read all
 * the bhi sensors data.
 */

#include "Arduino.h"
#include "sensor_hub.h"
#include "sensiml_recognition.h"

#include "Arduino_BHY2.h"
#include "Nicla_System.h" //must be placed after "ArduinoJSon.h"

static int8_t json_cfg_buffer[WRITE_BUFFER_SIZE];

static unsigned long previousMs;
static uint16_t interval = 1000 / ODR_IMU;
static bool config_received = false;
static uint8_t column_index = 0;

DynamicJsonDocument config_message(256);
#if USE_SECOND_SERIAL_PORT_FOR_OUTPUT
auto &dataOutSerial = Serial1;
#else
auto &dataOutSerial = Serial;
#endif // USE_SECOND_SERIAL_PORT_FOR_OUTPUT

static void sendJsonConfig()
{
    serializeJson(config_message, (void *)json_cfg_buffer, WRITE_BUFFER_SIZE);
    dataOutSerial.println((char *)json_cfg_buffer);
    dataOutSerial.flush();
}

uint32_t get_free_memory_size()
{
    const uint32_t START = 16 * 1024;
    uint8_t *mem;
    uint32_t size = START;
    uint32_t floor = 0;

    while (size > 0)
    {
        mem = (uint8_t *)malloc(size);
        if (mem != NULL)
        {
            free(mem);
            floor = size;
            size = (size << 1);
            break;
        }
        else
        {
            size = size >> 1;
        }
    }

    while (size > floor)
    {
        mem = (uint8_t *)malloc(size);
        if (mem != NULL)
        {
            free(mem);
            break;
        }
        else
        {
            size--;
        }
    }

    Serial.print("free mem size: ");
    Serial.println(String(size));
    return size;
}

// void checkSensorCfg()
//{
//   struct bhy2_virt_sensor_conf cfg;
//
//   memset(&cfg, 0, sizeof(cfg));
//   BHY2.getSensorCfg(SENSOR_ID_ACC_RAW, &cfg);
//   Serial.println("acc:");Serial.println(String(cfg.sensitivity));Serial.println(String(cfg.range));Serial.println(String(cfg.sample_rate));
//
//
//   memset(&cfg, 0, sizeof(cfg));
//   BHY2.getSensorCfg(SENSOR_ID_GYRO_RAW, &cfg);
//   Serial.println("gyr:");Serial.println(String(cfg.sensitivity));Serial.println(String(cfg.range));Serial.println(String(cfg.sample_rate));
//
//   memset(&cfg, 0, sizeof(cfg));
//   BHY2.getSensorCfg(SENSOR_ID_TEMP, &cfg);
//   Serial.println("temp:");Serial.println(String(cfg.sensitivity));Serial.println(String(cfg.range));Serial.println(String(cfg.sample_rate));
//
//   memset(&cfg, 0, sizeof(cfg));
//   BHY2.getSensorCfg(SENSOR_ID_BARO, &cfg);
//   Serial.println("BARO:");Serial.println(String(cfg.sensitivity));Serial.println(String(cfg.range));Serial.println(String(cfg.sample_rate));
// }

void setup()
{
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial)
        ;

    NiclaSettings niclaSettings(NICLA_I2C, 0, NICLA_VIA_ESLOV, 0);
    BHY2.begin(niclaSettings);
#if ENABLE_RECOGNITION
    sensiml_init();
#endif // ENABLE_RECOGNITION

#if ENABLE_ACCEL || ENABLE_GYRO || ENABLE_MAG || ENABLE_GAS || ENABLE_TEMP
    column_index = setup_sensors(config_message, column_index);
#endif
    config_message["samples_per_packet"] = MAX_SAMPLES_PER_PACKET;

    delay(1000);
    sendJsonConfig();

#if DEBUG
    // checkSensorCfg();
#endif
    get_free_memory_size();
}

static uint8_t packetNum = 0;
static uint8_t sensorRawIndex = 0;

void loop()
{
    unsigned long currentMs;
    bool connected_to_host = false;

    BHY2.update();

    currentMs = millis();

    if (!config_received)
    {
        sendJsonConfig();
        delay(DELAY_BRDCAST_JSON_DESC_SENSOR_CONFIG);
        if (dataOutSerial.available() > 0)
        {
            String rx = dataOutSerial.readString();
            Serial.println(rx);
            if (rx.equals("connect") || rx.equals("cnnect"))
            {
#if USE_SECOND_SERIAL_PORT_FOR_OUTPUT
                Serial.println("Got Connect message");
#endif
                config_received = true;
            }
            else
            {
            }
        }
    }
    else
    {
        if (dataOutSerial.available() > 0)
        {
            String rx = dataOutSerial.readString();
            if (rx.equals("disconnect"))
            {
                config_received = false;
            }
        }
    }

    if (config_received)
    {
        connected_to_host = true;
    }

    if (connected_to_host)
    {
        // CHECKME
        if ((int32_t)currentMs - (int32_t)previousMs >= interval)
        {

#if ENABLE_ACCEL || ENABLE_GYRO || ENABLE_MAG
            sensorRawIndex = update_sensor_data_col(sensorRawIndex);
            packetNum++;
            int16_t *pData = get_sensor_data_buffer();

            if (packetNum == MAX_SAMPLES_PER_PACKET)
            {
                dataOutSerial.write((uint8_t *)pData, sensorRawIndex * sizeof(int16_t));
                dataOutSerial.flush();
#if ENABLE_RECOGNITION
                sensiml_recognition(pData, MAX_SAMPLES_PER_PACKET, MAX_NUMBER_OF_COLUMNS, 0);
#endif // ENABLE_RECOGNITION
                sensorRawIndex = 0;
                memset(pData, 0, MAX_NUMBER_OF_COLUMNS * MAX_SAMPLES_PER_PACKET * sizeof(int16_t));
                packetNum = 0;
            }
#endif // #if ENABLE_ACCEL || ENABLE_GYRO || ENABLE_MAG
        }
    }
}
