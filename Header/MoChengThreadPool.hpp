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

    // class Worker;
    class ThreadPool 
    {
    private:
        condition_variable avaliable;
        mutex myMutex;
        queue<function<void()>> taskQueue;
        unique_ptr<thread> workerThread;

        int workerNum;

        vector<unique_ptr<thread>> workers;

    public:
        ThreadPool();

        auto Run(auto &&action, auto &&...args)
            -> future<decltype(action(args...))>;

        void Start();
        void StartSingleWorker();
        void StopWorks();
        void StopSingleWorker();
        ~ThreadPool();
    };

    class Worker
    {
    private:
        unique_ptr<thread> workerThread;
        bool stopped = false;

        weak_ptr<ThreadPool> threadPool;

    public:
        Worker();

        void StartWork(weak_ptr<ThreadPool> threadPool);

        void StopWorker();

        ~Worker();
    };

    ThreadPool::ThreadPool()
    {
        this->workerNum = thread::hardware_concurrency();
        cout << "worker == " << this->workerNum << endl;
    }
    void ThreadPool::Start()
    {
        cout << "start" << endl;

        for (size_t i = 0; i < (size_t)this->workerNum; i++)
        {

            StartSingleWorker();
        }
        cout << "vector size = " << workers.size() << endl;
    }
    void ThreadPool::StartSingleWorker()
    {
        auto threadPtr =
            make_unique<thread>(
                [this](stop_token st)
                {
                    while (true)
                    {

                        auto stopped = bind([this](stop_token st)
                                            { return st.stop_requested() || !(this->taskQueue).empty(); },
                                            st);

                        function<void()> task;

                        {
                            unique_lock<mutex> lock(this->myMutex);

                            this->avaliable.wait(lock, stopped);

                            if (st.stop_requested() && this->taskQueue.empty())
                            {
                                cout << "break" << endl;
                                break;
                            }
                            cout << "queue size " << this->taskQueue.size() << endl;
                            task = move(this->taskQueue.front());

                            this->taskQueue.pop();
                        }

                        task();
                    }
                }

            );
        
        this->workers.emplace_back(move(threadPtr));
         
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

    ThreadPool::~ThreadPool()
    {
        
        cout << "thread delete" << endl;
        this->avaliable.notify_all();
        this->StopWorks();
    }

    void ThreadPool::StopWorks()
    {
        for (auto &i : this->workers)
        {
            i->request_stop();
            
        }
        for (auto &i : this->workers)
        {
             i->join();
        }
    }

}
#endif
