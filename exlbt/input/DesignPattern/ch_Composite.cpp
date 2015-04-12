#include "ch_Composite.h"

extern "C" int printf(const char *format, ...);

Equipment::Equipment(const char* name) {
  _name = name;
}

CompositeEquipment::CompositeEquipment(const char* name)
  : Equipment(name) {
  printf("CompositeEquipment() 1\n");
}

#if 1
void CompositeEquipment::Add(Equipment* equip) {
  printf("Add() 1\n");
  _equipment.Add(equip);
  printf("Add() 2\n");
}

Currency CompositeEquipment::NetPrice () {
  Iterator<Equipment*>* i = CreateIterator();
  Currency total = 0;
  for (i->First(); !i->IsDone(); i->Next()) {
    total += i->CurrentItem()->NetPrice();
  }
//  delete i;
  return total;
}
#endif

#if 1
Iterator<Equipment*>* CompositeEquipment::CreateIterator() {
#if 0
  Iter = new ListIterator<Equipment*>(&_equipment);
#elif 1
  Iter->Create(&_equipment);
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
}
#endif

#ifndef DYNAMIC_SEARCH // if no this then not work.
void Chassis::Add(Equipment* equip) {
  CompositeEquipment::Add(equip);
}
#endif

#if 0
Cabinet::Cabinet(const char* name) 
  : CompositeEquipment(name) {
  printf("Cabinet() 1\n");
}
#endif

Currency FloppyDisk::NetPrice() {
  return 5;
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

