#include "ch_Iterator.h"

template <class Item>
List<Item>::List (long size) {
  _size = 0;
}

template <class Item>
Item& List<Item>::Add (Item item) {
  _item[_size] = item;
  _size++;
  return &item;
}

template <class Item>
ListIterator<Item>::ListIterator (
  ) : _list(0), _current(0) {
}

template <class Item>
ListIterator<Item>::ListIterator (
  const List<Item>* aList
  ) : _list(aList), _current(0) {
}

template <class Item>
void ListIterator<Item>::Create (
  List<Item>* aList
  ) {
  _list = aList;
  _current = 0;
}

template <class Item>
void ListIterator<Item>::First () {
  _current = 0;
}

template <class Item>
void ListIterator<Item>::Next () {
  _current++;
}

template <class Item>
Item ListIterator<Item>::CurrentItem () const {
//  if (IsDone()) {
//    throw IteratorOutOfBounds;
//  }
  return _list->Get(_current);
}


