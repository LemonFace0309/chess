#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <memory>

class Observer;

class Subject {
  std::vector<std::shared_ptr<Observer>> observers;
 public:
  Subject();
  void attach(std::shared_ptr<Observer>);
  void clear();
  void notifyObservers();
  void renderObservers();
  virtual ~Subject() = 0;
};

#endif