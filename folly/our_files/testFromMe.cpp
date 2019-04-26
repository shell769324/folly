#include <folly/ConcurrentSkipList.h>
#include <iostream>

typedef folly::ConcurrentSkipList<int> intSkipList;
typedef folly::ConcurrentSkipList<int>::Accessor intAccessor;

void* multi(void* ptr) {
  pthread_detach(pthread_self());
  intAccessor acc = *(intAccessor*) ptr;
  std::cout << (*acc.begin()) << " ";
  return NULL;
}


int main() {
  pthread_t thread1, thread2;
  intAccessor acc = intSkipList::create();
  acc.insert(10);
  intAccessor* acc1 = &acc;
  pthread_create(&thread1, NULL, multi, (void*) acc1);
  pthread_create(&thread2, NULL, multi, (void*) acc1);
  for(int i = 5; i >= -300; i--) {
    acc.insert(i);
  }
  std::cout << "here";
}
