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
    using thread = std::jthread;

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
        void StopWork();
        ~ThreadPool();
    };

    class Worker
    {
    private:
        thread *worker;
        bool stopped = false;

    public:
        Worker();

        void StartWork(ThreadPool *threadPool);
        void StopWorker(ThreadPool *threadPool);

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

        this->taskQueue.emplace([task]()
                                { (*task)(); });

        this->avaliable.notify_one();

        return task->get_future();
    }

    ThreadPool::~ThreadPool() = default;

    void ThreadPool::StopWork()
    {
        for (auto &i : this->workers)
        {
            i->StopWorker(this);
        }
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
            [threadPool](stop_token st)
            {
                while (true)
                {
                    auto stopped = bind([](ThreadPool *threadPool, stop_token st)
                                        { return st.stop_requested() || !(threadPool->taskQueue).empty(); },
                                        threadPool, st);

                    function<void()> task;
                    {
                        unique_lock<mutex> lock(threadPool->myMutex);

                        threadPool->avaliable.wait(lock, stopped);
                        if (st.stop_requested() && threadPool->taskQueue.empty())
                        {
                            break;
                        }

                        task = move(threadPool->taskQueue.front());

                        threadPool->taskQueue.pop();
                    }

                    task();
                }
            });
    }
    void Worker::StopWorker(ThreadPool *threadPool)
    {
        this->worker->request_stop();
    }
}
#endif
