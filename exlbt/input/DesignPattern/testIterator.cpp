
#include "ch_Iterator.h"

#include <cstdio>

#if 1
int NetPrice () {
  ChenList<int> ll(0);
  ll.Add(10);
  ll.Add(20);
  ListIterator<int> it(&ll);
  Iterator<int>* i = &it;
  int total = 0;
  for (i->First(); !i->IsDone(); i->Next()) {
    printf("i->CurrentItem() = %d\n", i->CurrentItem());
    total += i->CurrentItem();
  }
  return total;
}

int main() {
  int price = NetPrice();
  printf("price = %d\n", price);
}
#endif
