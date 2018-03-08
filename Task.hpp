#ifndef TASK_H
#define TASK_H

#include <functional>

class Task
{
  public:
    virtual void operator()() = 0;
    virtual ~Task() {};
};

template<typename FUNC, typename... ARGS>
class TemplateTask : public Task
{
  public:
    TemplateTask(FUNC f, ARGS... args):_func(std::bind(std::forward<FUNC>(f), std::forward<ARGS>(args)...)) {}
    ~TemplateTask() {}
    virtual void operator()() { _func(); }
  private:
    decltype( std::bind(std::declval<FUNC>(), std::declval<ARGS>()... )) _func;
};

#endif
