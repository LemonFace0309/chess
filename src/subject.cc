#include <vector>
#include <memory>

#include "subject.h"
#include "observer.h"
#include "textdisplay.h"
using namespace std;

Subject::Subject() {}

void Subject::attach(unique_ptr<Observer> o) {
  observers.push_back(o);
}

void Subject::clear() {
  observers.clear();
}

void Subject::notifyObservers() {
  for (auto &observer : observers) {
    observer->notify();
  }
}

void Subject::renderObservers() {
  for (auto &observer : observers) {
    observer->render();
  }
}