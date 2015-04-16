#ifndef CH_COMPOSITE_H
#define CH_COMPOSITE_H

#define DYNAMIC_SEARCH // The dynamic search virtual table is not support at this point.

//typedef typename std::int Currency;
#define Currency int
#define Watt int

#include "ch_Iterator.h"

class Equipment {
public:
  const char* Name() { return _name; }
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
  virtual void Add(Equipment*);
  virtual void Remove(Equipment*);
  virtual Iterator<Equipment*>* CreateIterator();
protected:
  Equipment(const char* name);
private:
  char _name[100];
};

class FloppyDisk : public Equipment {
public:
  FloppyDisk(const char*);
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
};

class CompositeEquipment : public Equipment {
public:
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
  virtual void Add(Equipment* equip);
  virtual void Remove(Equipment* equip);
  virtual Iterator<Equipment*>* CreateIterator();
  ListIterator<Equipment*>* Iter;
protected:
  CompositeEquipment(const char* name);
//private:
public:
  ChenList<Equipment*> _equipment;
};

class Chassis : public CompositeEquipment {
public:
  Chassis(const char*);
#if 0
  virtual Watt Power();
  virtual Currency NetPrice();
  virtual Currency DiscountPrice();
#endif
#ifndef DYNAMIC_SEARCH
  virtual void Add(Equipment* equip);
#endif
};

class Cabinet : public CompositeEquipment {
public:
  Cabinet(const char*);
};

class Bus : public CompositeEquipment {
public:
  Bus(const char*);
};

class Card : public CompositeEquipment {
public:
  Card(const char*);
};

#endif

