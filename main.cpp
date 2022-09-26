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

  MoChengThreadPool::ThreadPool r;
  r.StartWork();
  vector<future<void>> f;

  this_thread::sleep_for(chrono::seconds(2));

  for (size_t i = 0; i < 66; i++)
  {

    auto res = r.Run([i](int a)
                     {    
      cout << "cur   "<<i<<"     "<<a  << endl;
      this_thread::sleep_for(chrono::seconds(a)); },
                     1);
    auto res1 = r.Run([i](string a)
                      {    
      cout << "cur   "<<i<<"     "<<a  << endl;
      this_thread::sleep_for(chrono::seconds(1)); },
                      "momo");
    f.push_back(move(res));
    f.push_back(move(res1));
    }
  // this_thread::sleep_for(chrono::seconds(1));
  for (size_t i = 0; i < 3; i++)
  {
    cout << endl
         << r.taskQueue.size() << endl
         << endl;
    auto res = r.Run([i](double a)
                     {
      cout << "cur   "<<i<<"     "  << endl;
      this_thread::sleep_for(chrono::seconds(1)); },
                     1.1);
    f.push_back(move(res));
  }
  // for (size_t i = 0; i < 14; i++)
  // {

  //   auto res = r.Run([](string aa, int w = 3)
  //                    {
  //     cout << "wwww" << w<<this_thread::get_id() << endl;
  //     this_thread::sleep_for(chrono::seconds(1)); },
  //                    "1");
  //   f.push_back(move(res));
  // }
  cout << "here" << endl;
  for (auto &i : f)
  {

    i.get();
    // cout << endl
    //      << endl
    //      << endl;
  }
}
