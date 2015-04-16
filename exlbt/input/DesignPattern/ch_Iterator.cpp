#include "ch_Iterator.h"

template <class Item>
ChenList<Item>::ChenList(int size) {
  _size = size;
}

template <class Item>
int ChenList<Item>::Count() const {
  return _size;
}

template <class Item>
Item ChenList<Item>::Get(int index) const {
  if (index < _size)
    return _item[index];
  else
    return 0;
}

template <class Item>
Item ChenList<Item>::Add(Item item) {
  _item[_size] = item;
  _size++;
  return _item[_size-1];
}

template <class Item>
void ChenList<Item>::Remove(Item item) {
  int i = 0;

  for (i = 0; i < _size; i++) {
    if (_item[i] == item)
      break;
  }
  if (i < _size) {
    for ( ; i < (_size - 1); i++) {
      _item[i] = _item[i+1];
    }
    _size--;
  }
  return;
}

template <class Item>
Iterator<Item>::Iterator() {
}

#if 1
template <class Item>
ListIterator<Item>::ListIterator (
  ) : _list(0), _current(0) {
}
#endif

template <class Item>
ListIterator<Item>::ListIterator (
  const ChenList<Item>* aList
  ) : _list(aList), _current(0), Iterator<Item>() {
}

template <class Item>
void ListIterator<Item>::Create (
  const ChenList<Item>* aList
  ) {
  _list = aList;
  _current = 0;
}

template <class Item>
void ListIterator<Item>::First() {
  _current = 0;
}

template <class Item>
void ListIterator<Item>::Next() {
  _current++;
}

template <class Item>
bool ListIterator<Item>::IsDone() const {
  return (_current >= _list->Count());
}

template <class Item>
Item ListIterator<Item>::CurrentItem() const {
//  if (IsDone()) {
//    throw IteratorOutOfBounds;
//  }
  return _list->Get(_current);
}

#if 0
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

