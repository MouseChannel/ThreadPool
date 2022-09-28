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

  auto r = make_shared<MoChengThreadPool::ThreadPool>();
  // make_shared<MoChengThreadPool::ThreadPool>(1, []()
  //                                            { MoChengThreadPool::ThreadPool::DeletePool(); });
  r->Start();
  vector<future<void>> f;

  this_thread::sleep_for(chrono::seconds(2));

  for (size_t i = 0; i < 66; i++)
  {

    auto res = r->Run([i](int a)
                      {    
      // cout << "cur   "<<i<<"     "<<a  << endl;
      cout<<this_thread::get_id()<<endl;
      this_thread::sleep_for(chrono::seconds(a)); },
                      2);

    f.push_back(move(res));
    // f.push_back(move(res1));
    // this_thread::sleep_for(chrono::milliseconds(50));
  }

  // this_thread::sleep_for(chrono::milliseconds(1000));

  cout << "here" << endl;
  cout << "usecount   " << r.use_count() << endl;
  this_thread::sleep_for(chrono::milliseconds(3000));

  cout << "exist Tasks: ------------------------------  " << r  << endl;
  // for (auto &i : f)
  // {

  //   i.get();
  // }
  // auto e = make_unique<MoChengThreadPool::Worker>();
}
