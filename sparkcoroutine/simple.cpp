#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include "sparkcoroutine\coroutine.hpp"

using namespace Spark::Thread;

bool g_shutdown;
std::vector<int> g_vecTask;

//������
void do_producer()
{
    const int RUN_TIMES = 100;
    g_shutdown = false;

    for (int i = 1; i <= RUN_TIMES; i++)
    {
        printf("������%d������\r\n", i);
        g_vecTask.push_back(i);

        //֪ͨ������
        Coroutine::yield();
    }

    g_shutdown = true;
}

//������
void do_consumer()
{
    while (!g_shutdown)
    {
        if (g_vecTask.size() == 0)
        {
            //û������֪ͨ������
            Coroutine::yield();
            continue;
        }

        printf("���ѵ�%d������\r\n", g_vecTask.back());
        g_vecTask.pop_back();

        //����ִ�����֪ͨ������������
        Coroutine::yield();
    }
}

/**
 *  ʵ��������-������ģ��
 */
int main(int argc, char* argv[])
{
    Coroutine::init();
    std::shared_ptr<void> auto_uinitialize(nullptr, [](void*){ Coroutine::uninit(); });

    Coroutine consumer([]() { do_consumer(); });
    Coroutine producer([]() { do_producer(); });

    Coroutine::manage(consumer);
    Coroutine::manage(producer);

    while (!g_shutdown) {
        Coroutine::yield();
    };

    return 0;
}

