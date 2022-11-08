
#include <algorithm>
#include <memory>
#include <functional>
#include <thread>
#include <condition_variable>
namespace MoChengThreadPool {
class Worker {
private:
  std::unique_ptr<std::thread> m_thread;
 
public:
  Worker( );

  void Run();
};

Worker::Worker( ) {

  m_thread = std::make_unique<std::thread>([]() {
   std::function<void()> task;
    {
                             

                            

                             
                            cout << "queue size " << this->taskQueue.size() << endl;
                            task = move(this->taskQueue.front());

                            this->taskQueue.pop();
                        }

                        task();

  }

  );
}

} // namespace MoChengThreadPool