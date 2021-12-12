#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <memory>

class Observer;

class Subject {
  std::vector<std::unique_ptr<Observer>> observers;
 public:
  void attach(Observer *o);
  void detach(Observer *o);
  void notifyObservers();
  virtual ~Subject() = 0;
};

#endif