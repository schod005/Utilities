#ifndef EVENT_H
#define EVENT_H
#include "Task.hpp"
#include <vector>

template<typename... ARGS>
class Event
{
  public:
    Event() { }

    virtual ~Event() { m_callbacks.clear(); }
    void operator()(ARGS... args) { fire(std::forward<ARGS>(args)...); }

    //member function
    template<typename CLASS>
    void addCallback(void (CLASS::*f)(ARGS...), CLASS* instance)
    {
      m_callbacks.push_back([=](ARGS... args){ (instance->*f)(std::forward<ARGS>(args)...); });
    }

    //non-member function
    void addCallback(void(*f)(ARGS...))
    {
      m_callbacks.push_back([=](ARGS... args){ f(std::forward<ARGS>(args)...); });
    }

    void fire(ARGS... args)
    {
      for(auto func : m_callbacks)
      {
        func(std::forward<ARGS>(args)...);
      }
    }
    
  private:
    std::vector<std::function<void(ARGS...)>> m_callbacks;
};


#endif
