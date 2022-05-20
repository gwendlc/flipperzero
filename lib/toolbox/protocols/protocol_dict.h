#pragma once
#include "protocol.h"

typedef struct ProtocolDict ProtocolDict;

typedef int32_t ProtocolId;

#define PROTOCOL_NO (-1)

ProtocolDict* protocol_dict_alloc(const ProtocolBase** protocols, size_t protocol_count);

void protocol_dict_free(ProtocolDict* dict);

void protocol_dict_set_data(
    ProtocolDict* dict,
    size_t protocol_index,
    const uint8_t* data,
    size_t data_size);

void protocol_dict_get_data(
    ProtocolDict* dict,
    size_t protocol_index,
    uint8_t* data,
    size_t data_size);

size_t protocol_dict_get_data_size(ProtocolDict* dict, size_t protocol_index);

size_t protocol_dict_get_max_data_size(ProtocolDict* dict);

const char* protocol_dict_get_name(ProtocolDict* dict, size_t protocol_index);

const char* protocol_dict_get_manufacturer(ProtocolDict* dict, size_t protocol_index);

void protocol_dict_decoders_start(ProtocolDict* dict);

ProtocolId protocol_dict_decoders_feed(ProtocolDict* dict, bool level, uint32_t duration);

void protocol_dict_decoders_reset(ProtocolDict* dict);

bool protocol_dict_encoder_start(ProtocolDict* dict, size_t protocol_index);

LevelDuration protocol_dict_encoder_yield(ProtocolDict* dict, size_t protocol_index);

void protocol_dict_encoder_reset(ProtocolDict* dict, size_t protocol_index);