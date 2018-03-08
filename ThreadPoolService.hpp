#ifndef THREADPOOLSERVICE
#define THREADPOOLSERVICE
#include "ThreadPool.hpp"

//  Usage:
//    ThreadPoolService creates a singleton ThreadPool that can be accessed anywhere this
//    file is included.
//
//    Create the ThreadPool:
//      ThreadPoolService::createThreadPool(20);
//
//    Add tasks:
//      ThreadPoolService::addTask(/*callable type, args*/);
//
//    Finish all currently pending tasks:
//      ThreadPoolService::finishCurrentTasks();
//
class ThreadPoolService
{
  public:
    static void createThreadPool(int numThreads = 8)
    {
      m_threadPool = new ThreadPool(numThreads);
    }

    template<typename FUNC, typename... ARGS>
    static void addTask(FUNC f, ARGS... args)
    {
      m_threadPool->addTask(std::forward<FUNC>(f), std::forward<ARGS>(args)...);
    }

    static void finishCurrentTasks()
    {
      m_threadPool->finishCurrentTasks();
    }

  private:
    static ThreadPool* m_threadPool;
};

ThreadPool* ThreadPoolService::m_threadPool = nullptr;

#endif
