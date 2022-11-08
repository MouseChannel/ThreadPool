
#include "Header/MoChengThreadPool.hpp"
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <string>
#include <thread>
// #include "MoChengThreadPool.cpp"
#include <future>

#include <chrono>
#include <tuple>

using namespace std;

int main() {

  MoChengThreadPool::ThreadPool p;
 
  vector<future<void>> f;
  for (int i = 0; i < 66; i++) {
    auto aa = p.Run(
        [](int a) {
          this_thread::sleep_for(chrono::seconds(a));
          std::cout << this_thread::get_id() << endl;
        },
        2);

    f.emplace_back(std::move(aa));
  }
  // for (auto &i : f) {
  //   i.get();
  // }
}

// auto r = make_shared<MoChengThreadPool::ThreadPool>();
//                                            {
//                                            MoChengThreadPool::ThreadPool::DeletePool();
//                                            });
// r->Start();
// vector<future<void>> f;

// this_thread::sleep_for(chrono::seconds(2));

// for (size_t i = 0; i < 66; i++)
// {

//   auto res = r->Run([i](int a)
//                     {
//     // cout << "cur   "<<i<<"     "<<a  << endl;
//     cout<<this_thread::get_id()<<endl;
//     this_thread::sleep_for(chrono::seconds(a)); },
//                     2);

//   f.push_back(move(res));
// }
