#ifndef SINGLETON_H
#define SINGLETON_H

//Usage:
//  Class inherits with curiously recurring template pattern.  Ensure constructor is private.
//
//    class Test: public Singleton<Test>
//    {
//      public:
//        void doStuff();
//        ...
//      private:
//        Test();
//    };
//
//  To get singleton instance of class, call getInstance
//
//    Test& t = Test::getInstance();
//    t.doStuff();
//
//  Also works
//
//    Test::getInstance().doStuff();
//
template<typename T>
class Singleton
{
  public:
    T& getInstance()
    {
      static T instance;
      return instance;
    }
};

#endif
