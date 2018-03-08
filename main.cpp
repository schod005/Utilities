#include "ThreadPoolService.hpp"
#include "Event.hpp"
#include "EventService.hpp"
#include <iostream>
#include <unistd.h>

void func(int i, int j)
{
  for(int i = 0; i < 100000; i++)
    i += j;
  std::cout << i << " " << j << " ";
}

void func2()
{
  std::cout << "---"; 
}

class test
{
  public:
    test() {};
    void func() { std::cout << "TEST CLASS" << " "; }
};

enum event_t
{
  EVENT1,
  EVENT2
};

int main()
{
  test t;
  ThreadPoolService::createThreadPool(20);
  for(int i = 0; i < 1000; i++)
  {
    ThreadPoolService::addTask([=]{for(int j = 0; j < 5000000; j++){ } std::cout << i << std::endl;});
    ThreadPoolService::addTask(&func, 1, 5);
    ThreadPoolService::addTask(&func2);
    ThreadPoolService::addTask(&test::func, &t);
  }
  ThreadPoolService::finishCurrentTasks();

  Event<> e;
  e.addCallback(func2);
  e.addCallback(&test::func, &t);
  e();

  EventService::addCallbackToEvent(EVENT1, &func);
  EventService::addCallbackToEvent(EVENT2, &func2);
  EventService::fireEvent(EVENT1, 56, 23);
  EventService::fireEvent(EVENT2);
  std::cout << std::endl;
}
