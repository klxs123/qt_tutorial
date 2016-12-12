#include <pthread.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include "protocol_parser.h"
#include "protocol_utils.h"
#include "data_source.h"

using namespace std;

static const int BUFFERLEN = 1024*10;
static pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

static const char* msg = "hello!\n";


static string g_buffer;

void get_data(Message& msg)
{
    GetUserInfoRequest request;
    request.users.push_back("zf01");
    request.users.push_back("zf02");
    request.users.push_back("zf03");
    request.fields.push_back("name");
    request.fields.push_back("img");
    request.fields.push_back("salary");    

    msg.data.clear();
    make_getuserinfo_request(request, msg.data);
    msg.command = CT_GetUserInfoRequest;
    msg.num = 0;

}

void *do_productor(void *p)
{
    string* buffer = (string*)p;

    Message msg;
    timeval tv;
    string data;
    while(1)
    {
        get_data(msg);

        msg_to_send_data(msg, data);

        while(buffer->capacity() - buffer->size() < data.length())
        {
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            select(0, 0, 0,0, &tv);
        }

        pthread_mutex_lock(&buffer_lock);

        *buffer+= data;
        data.clear();

        pthread_mutex_unlock(&buffer_lock);

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        select(0, 0, 0,0, &tv);
    }

}

void *do_consumer(void* p)
{
    string* buffer = (string*)p;
    timeval tv;
    string data;

    Message msg;
    while(1)
    {


        pthread_mutex_lock(&buffer_lock);

        int start_index = extract_packages(*buffer, data);

        if(start_index == -1)
        {
            pthread_mutex_unlock(&buffer_lock);
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            select(0, 0, 0,0, &tv);
            continue;
        }

        package_to_msg(msg, data);


        pthread_mutex_unlock(&buffer_lock);

        cout << "consumer get msg:" << msg.data <<endl;
        GetUserInfoRequest request;

        get_getuserinfo_request(request, msg.data);

        string sql;
        get_sql(request, sql);

        fprintf(stderr, "msg to sql:%s\n", sql.c_str());

        data.clear();
        msg.data.clear();

    }

}
void init_buffer()
{
    g_buffer.reserve(BUFFERLEN);
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
