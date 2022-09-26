#include <iostream>
#include <thread>
#include <future>
#include <string>
#include <chrono>
#include <functional>
#include "Header/MoChengThreadPool.h"
#include "MoChengThreadPool.cpp"
#include <future>

#include <chrono>

using namespace std;

int main()
{

  MoChengThreadPool::ThreadPool r;
  r.StartWork();
  vector<future<void>> f;

  this_thread::sleep_for(chrono::seconds(2));

  for (size_t i = 0; i < 14; i++)
  {

    auto res = r.Run([](int a)
                     {    
      cout << "cur   " << this_thread::get_id() << endl;
      this_thread::sleep_for(chrono::seconds(a)); },
                     1);
    f.push_back(move(res));
  }
  cout << "here" << endl;
  for (auto &i : f)
  {

    i.get();
    cout << endl
         << endl
         << endl;
  }
}
