//
// Created by Per-Arne on 26.02.2017.
//

#ifndef WARC2SIM_PRIORITYQUEUE_H
#define WARC2SIM_PRIORITYQUEUE_H

#include <functional>
#include <vector>
#include <queue>
#include <iostream>
template<typename T, typename priority_t>
struct PriorityQueue {
  typedef std::pair<priority_t, T> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>,
                 std::greater<PQElement>> elements;

  inline bool empty() const { return elements.empty(); }

  inline void put(T item, priority_t priority) {
    elements.emplace(priority, item);
  }

  inline T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
    inline void clear()
    {

      while(!empty()){

        elements.pop();
      }
    }

};
#endif //WARC2SIM_PRIORITYQUEUE_H
