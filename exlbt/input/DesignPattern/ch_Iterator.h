#ifndef CH_ITERATOR_H
#define CH_ITERATOR_H

#define DEFAULT_LIST_CAPACITY 10

template <class Item>
class ChenList {
public:
  ChenList(int size = DEFAULT_LIST_CAPACITY);
  int Count() const;
  Item Get(int index) const;
  Item Add(Item item);
  void Remove(Item item);
private:
  int _size;
  Item _item[100];
};

template <class Item>
class Iterator {
public:
  virtual void First() = 0;
  virtual void Next() = 0;
  virtual bool IsDone() const = 0;
  virtual Item CurrentItem() const = 0;
protected:
  Iterator();
};

template <class Item>
class ListIterator : public Iterator<Item> {
public:
  ListIterator();
  ListIterator(const ChenList<Item>* aList);
  virtual void Create(const ChenList<Item>* aList);
  virtual void First();
  virtual void Next();
  virtual bool IsDone() const;
  virtual Item CurrentItem() const;
private:
  const ChenList<Item>* _list;
  int _current;
};
#endif

