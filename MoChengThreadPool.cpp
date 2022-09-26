
#include <iostream>
#include <thread>
#include <condition_variable>
#include <vector>
#include <functional>
#include <future>

#include <string>
#include <queue>
#ifndef MoCheng_ThreadPool
#define MoCheng_ThreadPool
#include "Header/MoChengThreadPool.h"
#endif

namespace MoChengThreadPool
{
    using namespace std;

    ThreadPool::ThreadPool()
    {
        this->workerNum = thread::hardware_concurrency();
        cout << "worker == " << this->workerNum << endl;
    }
    void ThreadPool::StartWork()
    {
        cout << "123" << endl;
        for (size_t i = 0; i < (size_t)this->workerNum; i++)
        {
            workers.push_back(this->SpawnWorker());
        }
        // this->StartWork();
    }

    ThreadPool::~ThreadPool()
    {
    }

    Worker *ThreadPool ::SpawnWorker()
    {
        Worker *worker = new Worker();

        worker->StartWork(this);
        return worker;
    }

    Worker::Worker() = default;

    Worker::~Worker() = default;

    void Worker::StartWork(ThreadPool *threadPool)
    {

        worker = new thread(
            [threadPool]()
            {
                

                while (true)
                {
                    function<void()> task;
                    {

                        unique_lock<mutex> lock(threadPool->myMutex);

                        threadPool->avaliable.wait(lock);

                        task = move(threadPool->taskQueue.front());

                        threadPool->taskQueue.pop();
                    }

                    task();
                }
            });
    }

}
