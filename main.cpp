#include <iostream>
#include <thread>
#include <future>
#include <string>
#include <chrono>
#include <functional>
#include "Header/MoChengThreadPool.hpp"
// #include "MoChengThreadPool.cpp"
#include <future>

#include <chrono>

using namespace std;

int main()
{

  MoChengThreadPool::ThreadPool *r = new MoChengThreadPool::ThreadPool;
  r->StartWork();
  vector<future<void>> f;

  this_thread::sleep_for(chrono::seconds(2));

  for (size_t i = 0; i < 66; i++)
  {

    auto res = r->Run([i](int a)
                      {    
      cout << "cur   "<<i<<"     "<<a  << endl;
      this_thread::sleep_for(chrono::seconds(a)); },
                      1);

    f.push_back(move(res));
    // f.push_back(move(res1));
  }

  for (size_t i = 0; i < 3; i++)
  {
    cout << endl
         << r->taskQueue.size() << endl
         << endl;
    auto res = r->Run([i](double a)
                      {
      cout << "cur   "<<i<<"     "  << endl;
      this_thread::sleep_for(chrono::seconds(1)); },
                      1.1);
    f.push_back(move(res));
  }

  this_thread::sleep_for(chrono::milliseconds(100));
  r->StopWork();
  cout << "here" << endl;
  for (auto &i : f)
  {

    i.get();
  }
}
