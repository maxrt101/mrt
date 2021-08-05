#include <mrt/threads/pool.h>

mrt::threads::ThreadPool::ThreadPool(int threads_num) {
  if (threads_num) m_threads_num = threads_num;
  for (int i = 0; i < m_threads_num; i++) {
    m_pool.push_back(std::thread(&ThreadPool::threadWorkerFunction, this));
  }
}

mrt::threads::ThreadPool::~ThreadPool() {
  if (!m_stopped) {
    waitForAll();
  }
}

void mrt::threads::ThreadPool::addJob(threads::Job job) {
  {
    std::unique_lock<std::mutex> lock(m_queue_mutex);
    m_pending_jobs.push(job);
  }
  m_cv.notify_one();
}

void mrt::threads::ThreadPool::waitForAll() {
  m_finish.store(true);
  terminate();
}

void mrt::threads::ThreadPool::finishAll() {
  m_terminate.store(true);
  terminate();
}

void mrt::threads::ThreadPool::threadWorkerFunction() {
  while (1) {
    Job job;
    {
      std::unique_lock<std::mutex> lock(m_queue_mutex);
      m_cv.wait(lock, [&]{ return !m_pending_jobs.empty() || m_terminate.load() || m_finish.load(); });
      if (m_terminate.load()) return;
      if (m_finish.load() && m_pending_jobs.empty()) return;
      job = m_pending_jobs.front();
      m_pending_jobs.pop();
    }
    job.run();
  }
}

void mrt::threads::ThreadPool::terminate() {
  m_cv.notify_all();

  for (auto &thread : m_pool) {
    thread.join();
  }

  m_pool.clear();
  m_stopped = true;
}


