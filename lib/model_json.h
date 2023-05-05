#ifndef __MODEL_JSON_H__
#define __MODEL_JSON_H__

const char recognition_model_string_json[] = {"{\"NumModels\":1,\"ModelIndexes\":{\"0\":\"FLOW_BLOCKED_RANK_3\"},\"ModelDescriptions\":[{\"Name\":\"FLOW_BLOCKED_RANK_3\",\"ClassMaps\":{\"1\":\"Fan_Off\",\"2\":\"Fan_On\",\"3\":\"Flow_Blocked\",\"0\":\"Unknown\"},\"ModelType\":\"PME\",\"FeatureFunctions\":[\"MFCC\",\"MFCC\",\"MFCC\",\"MFE\",\"MFE\",\"SpectralEntropy\"]}]}"};

int32_t recognition_model_string_json_len = sizeof(recognition_model_string_json);

#endif /* __MODEL_JSON_H__ */
