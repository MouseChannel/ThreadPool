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
        queue<function<void()>> taskQueue;

    public:
        ThreadPool();
        Worker *SpawnWorker();
      
        friend Worker;
    

        template <typename Action, typename... Args>
        auto Run(Action &&action, Args &&...args)
            -> future<decltype(action(args...))>;
        void StartWork();
        ~ThreadPool();
    };

    template <typename Action, typename... Args>
    auto ThreadPool::Run(Action &&action, Args &&...args)
        -> future<decltype(action(args...))>
    {
        using returnType = decltype(action(args...));

        auto task = make_shared<packaged_task<returnType()>>(
            bind(forward<Action>(action), forward<Args>(args)...));
        this->taskQueue.emplace([task]()
                                { (*task)(); });

        this->avaliable.notify_one();

        return task->get_future();
    }

    class Worker
    {
    private:
        thread *worker;

    public:
        Worker();

        void StartWork(ThreadPool *threadPool);

        ~Worker();
    };

}
#endif
