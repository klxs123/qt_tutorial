#ifndef __protocol_parser_h__
#define __protocol_parser_h__

#include "protocol_def.h"

#include <stdint.h>
#include <string>

using std::string;

struct DataBuffer
{
    uint8_t* data;
    uint32_t len;
};

int data_to_send(const DataBuffer* data, DataBuffer* send);

int send_to_data(const DataBuffer* send, DataBuffer* data);

int data_to_send(const string& data, string& send);


#endif
