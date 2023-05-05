#ifndef __SML_RECOGNITION_RUN_H__
#define __SML_RECOGNITION_RUN_H__

#include "sensor_hub.h"
#include "kb.h"
#include "kb_typedefs.h"

int32_t get_last_segment_length();
int32_t get_last_result_model();
int32_t get_last_result_class();
int32_t get_last_feature_bank_number();
int32_t sensiml_recognition(signed short *data, int32_t number_packets, int32_t num_sensors,
                            int32_t model_index);
void sensiml_init();

#endif //__SML_RECOGNITION_RUN_H__