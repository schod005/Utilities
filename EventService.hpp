#ifndef EVENTSERVICE_H
#define EVENTSERVICE_H
#include "Event.hpp"
#include <unordered_map>

namespace EventService
{

template<typename ENUM, typename... ARGS>
class EventService
{
  public:
    static EventService& getInstance()
    {
      static EventService<ENUM, ARGS...> es;
      return es;
    }

    Event<ARGS...>& getEvent(ENUM e)
    {
      return m_eventMap[e];
    }
  private:
    EventService() {};
    std::unordered_map<ENUM, Event<ARGS...>> m_eventMap;
};

template<typename ENUM, typename... ARGS>
void addCallbackToEvent(ENUM e, void(*f)(ARGS...))
{
  EventService<ENUM, ARGS...>::getInstance().getEvent(e).addCallback(f);
}

template<typename ENUM, typename CLASS, typename... ARGS>
void addCallbackToEvent(ENUM e, void(CLASS::*f)(ARGS...), CLASS* c)
{
  EventService<ENUM, ARGS...>::getInstance().getEvent(e).addCallback(f, c);
}

template<typename ENUM, typename... ARGS>
void fireEvent(ENUM e, ARGS... args)
{
  EventService<ENUM, ARGS...>::getInstance().getEvent(e).fire(std::forward<ARGS>(args)...);
}

}
#endif
