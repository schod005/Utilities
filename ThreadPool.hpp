#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "Task.hpp"

//
//  Usage:
//    Test functions:
//
//      void func(int i, int j)
//      {
//        std::cout << i << " " << j << " ";
//      }
//      
//      void func2()
//      {
//        std::cout << "---"; 
//      }
//      
//      class test
//      {
//        public:
//          test() {};
//          void func() { std::cout << "TEST CLASS" << " "; }
//      };
//
//    Create ThreadPool:
//
//      ThreadPool tp(20);
//
//    Add tasks.  ThreadPool.addTask() takes any callable type with its arguments:
//
//      tp.addTask([]{std::cout << "something" << std::endl;});
//      tp.addTask(&func, 1, 5);
//      tp.addTask(&func2);
//      tp.addTask(&test::func, &t);
//
//    To finish all currently pending tasks:
//      tp.finishCurrentTasks();
//
//    Upon destruction, ThreadPool finishes the currently running tasks and scraps the pending tasks.
//
class ThreadPool
{
  public:
    template<typename FUNC, typename... ARGS>
    void addTask(FUNC f, ARGS... args)
    {
      std::unique_lock<std::mutex> mut1(m_addMutex);
      {
        std::unique_lock<std::mutex> mut2(m_queueMutex);
        m_queue.push(std::unique_ptr<Task>(new TemplateTask<FUNC, ARGS...>(std::forward<FUNC>(f), std::forward<ARGS>(args)...)));
      }
      m_cv.notify_one();
    }

    ThreadPool(int numThreads = 8) :m_destroyed(false)
    {
      for(int i = 0; i < numThreads; i++)
      {
        m_threads.push_back(std::thread( [&]{
              while(true)
              {
                std::unique_ptr<Task> task;
                {
                  std::unique_lock<std::mutex> mut(m_queueMutex);

                  if(m_queue.empty() && !m_destroyed)
                    m_cv.wait(mut, [&]{ return m_destroyed ? true : !m_queue.empty(); });

                  if(m_destroyed)
                    break;

                  task = std::move(m_queue.front());
                  m_queue.pop();

                  if(m_queue.empty())
                    m_cv_tasksDone.notify_one();
                }
                if(task)
                {
                  (*task)();
                }
              }
            }));
      }
    }

    void finishCurrentTasks()
    {
      std::unique_lock<std::mutex> mut1(m_addMutex);
      std::unique_lock<std::mutex> mut2(m_queueMutex);
      if(!m_queue.empty())
        m_cv_tasksDone.wait(mut2);
    }

    ~ThreadPool()
    {
      {
        std::unique_lock<std::mutex> mut{m_queueMutex};
        m_destroyed = true;
        m_cv.notify_all();
      }
      for(auto& thread : m_threads)
      {
        thread.join();
      }
    }

  private:
    std::queue<std::unique_ptr<Task>> m_queue;
    std::vector<std::thread> m_threads;
    std::mutex m_queueMutex;
    std::mutex m_addMutex;
    std::condition_variable m_cv;
    std::condition_variable m_cv_tasksDone;
    bool m_destroyed;
};

#endif
