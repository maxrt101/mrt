#include "pool.h"


mrt::threads::ThreadPool::ThreadPool() {
  for (int i = 0; i < threads_num_; i++) {
    pool_.push_back(std::thread(&ThreadPool::ThreadWorkerFunction, this));
  }
}

mrt::threads::ThreadPool::~ThreadPool() {
  if (!stopped) {
    WaitForAll();
  }
}

void mrt::threads::ThreadPool::AddJob(threads::Job job) {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    pending_jobs_.push(job);
  }
  cv_.notify_one();
}

void mrt::threads::ThreadPool::WaitForAll() {
  terminate_.store(true);
  Terminate();
}

void mrt::threads::ThreadPool::FinishAll() {
  finish_.store(true);
  Terminate();
}

void mrt::threads::ThreadPool::ThreadWorkerFunction() {
  while (1) {
    Job job;
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      cv_.wait(lock, [&]{ return !pending_jobs_.empty() || terminate_.load() || finish_.load(); });
      if (terminate_.load()) return;
      if (finish_.load() && pending_jobs_.empty()) return;
      job = pending_jobs_.front();
      pending_jobs_.pop();
    }
    job.Run();
  }
}

void mrt::threads::ThreadPool::Terminate() {
  cv_.notify_all();

  for (auto &thread : pool_) {
    thread.join();
  }

  pool_.clear();
  stopped = true;
}


