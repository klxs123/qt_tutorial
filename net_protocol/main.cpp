#include <pthread.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <unistd.h>
#include "protocol_parser.h"

static const int BUFFERLEN = 1024*10;
static pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

static const char* msg = "hello!\n";

struct Buffer
{
    DataBuffer data;
    int pos;
};

static Buffer g_buffer;

void get_data(DataBuffer& buffer)
{
    buffer.len = strlen(msg);
    buffer.data  = (uint8_t*)malloc(buffer.len);
    memcpy(buffer.data, msg, buffer.len);
}

void *do_productor(void *p)
{
    Buffer* buffer = (Buffer*)p;
    DataBuffer data;
    DataBuffer send;
    timeval tv;
    while(1)
    {
        get_data(data);
        data_to_send(&data, &send);
        while(buffer->data.len - buffer->pos < send.len)
        {
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            select(0, 0, 0,0, &tv);
        }

        pthread_mutex_lock(&buffer_lock);

        memcpy(buffer->data.data+buffer->pos, send.data, send.len);
        buffer->pos += send.len;

        pthread_mutex_unlock(&buffer_lock);

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        select(0, 0, 0,0, &tv);
    }

}

void *do_consumer(void* p)
{
    Buffer* buffer = (Buffer*)p;
    timeval tv;
    DataBuffer data;
    while(1)
    {
        if(buffer->pos == 0)
        {
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            select(0, 0, 0,0, &tv);
            continue;
        }
        pthread_mutex_lock(&buffer_lock);

         int ret = send_to_data(&buffer->data, &data);
        if(ret > 0 )
        {
            memmove(buffer->data.data, buffer->data.data+ret, buffer->pos - ret);
            buffer->pos -= ret;
            fprintf(stderr, "consumer:%s\n", data.data);
            free(data.data);
            data.len = 0;
        }
        pthread_mutex_unlock(&buffer_lock);

    }

}
void init_buffer()
{
    g_buffer.data.data = (uint8_t*)malloc(BUFFERLEN);
    g_buffer.data.len = BUFFERLEN;
    g_buffer.pos = 0;
}

int main(int argc, char *argv[])
{
    init_buffer();
    pthread_t tid_productor;
    pthread_t tid_consumer;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int ret  = pthread_create(&tid_productor, &attr, do_productor, &g_buffer);
    ret = pthread_create(&tid_consumer, &attr, do_consumer, &g_buffer);

    pthread_join(tid_consumer, 0);
    pthread_join(tid_productor, 0);

    pthread_attr_destroy(&attr);
    pthread_exit(NULL);
    return 0;
}
