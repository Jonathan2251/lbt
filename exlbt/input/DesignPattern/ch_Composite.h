//#define DYNAMIC_SEARCH // The dynamic search virtual table is not support at this point.

//typedef typename std::int Currency;
#define Currency int
#define Watt int

#include "ch_Iterator.h"

class Equipment {
public:
  virtual ~Equipment();
  const char* Name() { return _name; }
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
  virtual void Add(Equipment*);
  virtual void Remove(Equipment*);
  virtual Iterator<Equipment*>* CreateIterator();
protected:
  Equipment(const char*);
private:
  const char* _name;
};

class FloppyDisk : public Equipment {
public:
  FloppyDisk(const char*);
  virtual ~FloppyDisk();
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
};

class CompositeEquipment : public Equipment {
public:
  virtual ~CompositeEquipment();
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
  virtual void Add(Equipment*);
  virtual void Remove(Equipment*);
  virtual Iterator<Equipment*>* CreateIterator();
  ListIterator<Equipment*>* Iter;
protected:
  CompositeEquipment(const char*);
private:
  List<Equipment*> _equipment;
};

class Chassis : public CompositeEquipment {
public:
  Chassis(const char*);
  virtual ~Chassis();
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
#ifndef DYNAMIC_SEARCH
  virtual void Add(Equipment* equip);
#endif
};

class Cabinet : public CompositeEquipment {
public:
  Cabinet(const char*);
  virtual ~Cabinet();
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
};

class Bus : public CompositeEquipment {
public:
  Bus(const char*);
  virtual ~Bus();
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
};

class Card : public CompositeEquipment {
public:
  Card(const char*);
  virtual ~Card();
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
};

