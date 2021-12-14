#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <memory>

class Observer;

class Subject {
  std::vector<std::unique_ptr<Observer>> observers;
 public:
  Subject();
  void attach(std::unique_ptr<Observer>);
  void clear();
  void notifyObservers();
  void renderObservers();
  virtual ~Subject() = 0;
};

#endif