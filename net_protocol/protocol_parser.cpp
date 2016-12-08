#include "protocol_parser.h"

#include <cstring>
#include <malloc.h>


const char* PROTOCOL_START = "!start!";
const char* PROTOCOL_END = "!end!";

int data_to_send(const DataBuffer *data, DataBuffer *send)
{
    send->len = strlen(PROTOCOL_START) + strlen(PROTOCOL_END) + sizeof(data->len) + data->len;
    send->data = (uint8_t*)malloc(send->len);
    memcpy(send->data, PROTOCOL_START, strlen(PROTOCOL_START));
    memcpy(send->data+strlen(PROTOCOL_START), &data->len, sizeof(data->len));//data len field
    memcpy(send->data+strlen(PROTOCOL_START)+ sizeof(data->len), data->data, data->len);
    memcpy(send->data+ strlen(PROTOCOL_START) + sizeof(data->len) + data->len, PROTOCOL_END, strlen(PROTOCOL_END));
    return send->len;
}

int data_to_send(const string &data, string &send)
{
    return 0;
}

int send_to_data(const DataBuffer *send, DataBuffer *data)
{
    const char* start = strstr((const char*)send->data, PROTOCOL_START);
    if(start == 0)
    {
        return -1;//not find
    }
    int start_index = (uint64_t)start - (uint64_t)send->data;

    start+=strlen(PROTOCOL_START);

    int data_len = *(uint32_t*)start;

    const char* end = strstr(start+sizeof(uint32_t)+data_len, PROTOCOL_END);

    if(end - start - sizeof(uint32_t) != data_len)
    {
        return -1;
    }

    data->len = data_len;
    data->data = (uint8_t*)malloc(data_len);

    memcpy(data->data, start+sizeof(uint32_t), data_len);

    return start_index+strlen(PROTOCOL_START)+sizeof(uint32_t)+data->len + strlen(PROTOCOL_END);
}
