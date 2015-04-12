#define DEFAULT_LIST_CAPACITY 10

template <class Item>
class List {
public:
  List(long size = DEFAULT_LIST_CAPACITY);
  long Count() const;
  Item& Get(long index) const;
  Item& Add(Item item);
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
  ListIterator(const List<Item>* aList);
  virtual void Create(List<Item>* aList);
  virtual void First();
  virtual void Next();
  virtual bool IsDone() const;
  virtual Item CurrentItem() const;
private:
  const List<Item>* _list;
  long _current;
};

