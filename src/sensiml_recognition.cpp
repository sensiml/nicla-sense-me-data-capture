#include "sensiml_recognition.h"

static int32_t last_model_result;
static int32_t last_class_result;
static int32_t last_segment_length;
static int32_t last_feature_bank_number;


uint8_t ledFault = red;
uint8_t ledNormal = green;
uint8_t ledBlockedFlow = blue;
uint8_t ledFanOff = white;
bool led_is_on;

int32_t get_last_segment_length()
{
    return last_segment_length;
}

int32_t get_last_feature_bank_number()
{
    return last_feature_bank_number;
}

int32_t get_last_result_model()
{
    return last_model_result;
}

int32_t get_last_result_class()
{
    return last_class_result;
}

int32_t sensiml_recognition(signed short *data, int32_t number_packets, int32_t num_sensors, int32_t model_index)
{
    int32_t ret = -1;
    int32_t feature_bank_number = 0;
    int32_t compIdx = 0;
    sml_get_feature_bank_number(model_index, &feature_bank_number);

    for (int32_t i = 0; i < number_packets; i++)
    {

        if (feature_bank_number == 1)
        {
            ret = kb_run_model((SENSOR_DATA_T *)&data[compIdx], num_sensors, model_index);
        }
        else
        {
            ret = kb_run_model_with_cascade_features((SENSOR_DATA_T *)&data[compIdx], num_sensors, model_index);
        }

        if (ret >= 0)
        {
            // sml_output_results(model_index, ret);
            kb_reset_model(model_index);
            switch (ret)
            {
            case 1:
                setLedColor(red);
                break;
            case 2:
                setLedColor(green);
                break;
            case 3:
                setLedColor(blue);
                break;
            default:
                setLedColor(red);
                break;
            };
            compIdx += num_sensors;
        }
        return ret;
    }
}

void sensiml_init()
{
    kb_model_init();
}