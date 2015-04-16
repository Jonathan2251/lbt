#include "ch_Composite.h"

//extern "C" int printf(const char *format, ...);
#include <cstdio>

#include "ch_Iterator.cpp"

Equipment::Equipment(const char* name) {
  for (int i = 0; name[i] != '\0'; i++)
    _name[i] = name[i];
  return;
}

Watt Equipment::Power () {
  return 0;
}

Currency Equipment::NetPrice () {
  return 0;
}

Currency Equipment::DiscountPrice () {
  return 0;
}

void Equipment::Add(Equipment* equip) {
  return;
}

void Equipment::Remove(Equipment* equip) {
 return;
}

Iterator<Equipment*>* Equipment::CreateIterator() {
  return 0;
}

FloppyDisk::FloppyDisk(const char* name)
  : Equipment(name)
{
  return;
}

Watt FloppyDisk::Power () {
  return 5;
}

Currency FloppyDisk::NetPrice () {
  return 10;
}

Currency FloppyDisk::DiscountPrice () {
  return 2;
}

CompositeEquipment::CompositeEquipment(const char* name)
  : _equipment(0), Equipment(name) {
}

Watt CompositeEquipment::Power () {
  Iterator<Equipment*>* i = CreateIterator();
  Currency total = 0;
  for (i->First(); !i->IsDone(); i->Next()) {
    total += i->CurrentItem()->Power();
  }
  return total;
}

Currency CompositeEquipment::NetPrice () {
    printf("CompositeEquipment::NetPrice()\n");
#if 0
  Iterator<Equipment*>* i = CreateIterator();
#else
  ListIterator<Equipment*> it;
  it.Create(&_equipment);
  Iterator<Equipment*>* i = &it;
#endif
  Currency total = 0;
  for (i->First(); !i->IsDone(); i->Next()) {
    printf("i->CurrentItem()->NetPrice() = %d\n", i->CurrentItem()->NetPrice());
    total += i->CurrentItem()->NetPrice();
  }
//  delete i;
  return total;
}

Currency CompositeEquipment::DiscountPrice () {
  Iterator<Equipment*>* i = CreateIterator();
  Currency total = 0;
  for (i->First(); !i->IsDone(); i->Next()) {
    total += i->CurrentItem()->DiscountPrice();
  }
  return total;
}

#if 1
void CompositeEquipment::Add(Equipment* equip) {
//  printf("Add() 1\n");
  _equipment.Add(equip);
//  printf("Add() 2\n");
  return;
}

void CompositeEquipment::Remove(Equipment* equip) {
  _equipment.Remove(equip);
  return;
}

#endif

#if 1
Iterator<Equipment*>* CompositeEquipment::CreateIterator() {
#if 0
  Iter = new ListIterator<Equipment*>(&_equipment);
#else
  static ListIterator<Equipment*> it(&_equipment);
  Iter = &it;
#endif
  return Iter;
}
#endif

#if 1
Chassis::Chassis(const char* name) 
  : CompositeEquipment(name) {
  printf("Chassis() 1\n");
  return;
}
#endif

#ifndef DYNAMIC_SEARCH
void Chassis::Add(Equipment* equip) {
  CompositeEquipment::Add(equip);
  return;
}
#endif

#if 1
Cabinet::Cabinet(const char* name) 
  : CompositeEquipment(name) {
  printf("Cabinet() 1\n");
  return;
}
#endif

Bus::Bus(const char* name) 
  : CompositeEquipment(name) {
  return;
}

Card::Card(const char* name) 
  : CompositeEquipment(name) {
  return;
}

#if 1
int main() {
#if 0
  printf("1\n");
  Cabinet cabinet("PC Cabinet");
  printf("2\n");
#endif
#if 1
  Chassis chassis("PC Chassis");
  printf("3\n");
#endif
#if 0
  cabinet.Add(&chassis);
  printf("4\n");
  Bus bus("MCA Bus");
  printf("5\n");
  Card card("16Mbs Token Ring");
  printf("6\n");
  bus.Add(&card);
  printf("7\n");
  chassis.Add(&bus);
  printf("8\n");
#endif
#if 1
  FloppyDisk floppyDisk("3.5in Floppy");
  printf("8\n");
  chassis.Add(&floppyDisk);
  printf("10\n");
#endif
//  cout << "The net price is " << chassis->NetPrice() << endl;
  printf("The net price is %d\n", chassis.NetPrice());
  return 0;
}
#endif

