#include <queue>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <future>

namespace kiq::log
{

template <typename T = std::function<void()>>
class reactive_queue
{
using mutex_t  = std::mutex;
using queue_t  = std::queue<T>;
using cond_t   = std::condition_variable;
using lock_t   = std::lock_guard<std::mutex>;
using u_lock_t = std::unique_lock<std::mutex>;
//-------------------------------------------------
queue_t queue_;
mutex_t mutex_;
cond_t  cond_;
//-------------------------------------------------
public:
void push(T fn)
{
  {
    lock_t lock(mutex_);
    queue_.push(fn);
  }
  cond_.notify_one();
}
//-------------------------------------------------
T wait_and_pop()
{
  u_lock_t lock(mutex_);
  while (queue_.empty())
    cond_.wait(lock);
  const auto fn = queue_.front();
  queue_.pop();
  return fn;
}
//-------------------------------------------------
};
//-------------------------------------------------
//-------------------------------------------------
template <typename T = std::function<void()>>
class active_object
{
  using future_t = std::future<void>;
//-------------------------------------------------
  reactive_queue<T> queue_;
  future_t          fut_;
  bool              done_{false};
//-------------------------------------------------
public:
  active_object()
  {
    run();
  }
//-------------------------------------------------
  ~active_object()
  {
    put([this]() noexcept { done_ = true; });
    if (fut_.valid())
      fut_.wait();
  }
//-------------------------------------------------
  void run()
  {
    fut_ = std::async(std::launch::async, [this]
    {
      while (!done_)
        queue_.wait_and_pop()();
    });
  }
//-------------------------------------------------
  void put(T&& fn)
  {
    queue_.push(std::move(fn));
  }
};
} // ns kiq::log
