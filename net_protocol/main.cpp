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
#include "net_client.h"

using namespace std;

static const int BUFFERLEN = 1024*10;
static pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

static const char* msg = "hello!\n";


static string g_buffer;

static void get_msg(Message& msg)
{
    GetUserInfoRequest request;
    request.users.push_back("Audi");
    request.users.push_back("Volvo");

    request.fields.push_back("name");
    request.fields.push_back("salary");    

    msg.data.clear();
    make_request(request, msg.data);
    msg.command = CT_GetUserInfoRequest;
    msg.num = 0;

}

static void *do_productor(void *p)
{
    string* buffer = (string*)p;

    Message msg;
    timeval tv;
    string data;
    while(1)
    {
        get_msg(msg);

        msg_to_package(msg, data);

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

static void *do_consumer(void* p)
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
        get_request(request, msg.data);
        string sql;
        get_sql(request, sql);

        fprintf(stderr, "msg to sql:%s\n", sql.c_str());

        data.clear();
        msg.data.clear();

    }

}
static void init_buffer()
{
    g_buffer.reserve(BUFFERLEN);
}

static void* client_fun(void* p)
{

    tcp_client tc;
    tc.connect("localhost", 3333);
    //login

    Message msg;
    msg.command = CT_LoginRequest;
    LoginRequest request;
    request.user.first = "zf";
    request.user.second = "123456";
    make_request(request, msg.data);
    string data;
    msg_to_package(msg, data);

    tc.write(data);
    data.clear();

    tc.read(data, 1024);


    msg.data.clear();
    package_to_msg(msg, data);

    LoginResponse response;

    get_response(response, msg.data);

    if(!response.success)
    {
        printf("recv login response:failed\n");
        return 0;
    }

    printf("recv login response:success\n");


    //after login

    timeval tv;
    data.clear();
    msg.clear();
    while(1)
    {
        get_msg(msg);

        msg_to_package(msg, data);

        tc.write(data);
        data.clear();

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        select(0, 0, 0,0, &tv);

        int size = tc.read(data, 1024);
        if(size > 0)
        {
            Message msg;
            extract_msg(data, msg);
            GetUserInfoResponse response;
            get_response(response, msg.data);
        }

        data.clear();
    }

    return 0;
}


static void protocol_test()
{
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

}

#include "zf_server.h"

void net_test()
{
    zf_server server("127.0.0.1", 3333);
    server.start();

    pthread_t tid_client;
    int ret = pthread_create(&tid_client, 0, client_fun, 0);


    while(!server.is_stopped())
    {
        timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        select(0,0,0,0, &tv);
    }
}




int main(int argc, char *argv[])
{
    init_buffer();

    //protocol_test();

    net_test();

    return 0;
}
