#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <future>
#ifndef MoCheng_ThreadPool
#define MoCheng_ThreadPool

namespace MoChengThreadPool
{
    using namespace std;

    class Worker;
    class ThreadPool
    {
    private:
        vector<Worker *> workers;

        int workerNum;
        mutex myMutex;
        condition_variable avaliable;

    public:
        ThreadPool();
        Worker *SpawnWorker();
        queue<function<void()>> taskQueue;
        friend Worker;

        auto Run(auto &&action, auto &&...args)
            -> future<decltype(action(args...))>;

        void StartWork();
        ~ThreadPool();
    };

    class Worker
    {
    private:
        thread *worker;

    public:
        Worker();

        void StartWork(ThreadPool *threadPool);

        ~Worker();
    };

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

    auto ThreadPool::Run(auto &&action, auto &&...args)
        -> future<decltype(action(args...))>
    {
        using returnType = decltype(action(args...));

        auto task = make_shared<packaged_task<returnType()>>(
            bind(forward<decltype(action)>(action), forward<decltype(args)>(args)...));

        this->taskQueue.push([task]()
                             { (*task)(); });

        this->avaliable.notify_one();

        return task->get_future();
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
                        // cout << "lock" << endl;
                        threadPool->avaliable.wait(lock, [threadPool]
                                                   { return !(threadPool->taskQueue).empty(); });

                        task = move(threadPool->taskQueue.front());

                        threadPool->taskQueue.pop();
                    }

                    task();
                }
            });
    }

}
#endif
