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
  Iter = new ListIterator<Equipment*>(&_equipment);
  return Iter;
}
#endif

Chassis::Chassis(const char* name) 
  : CompositeEquipment(name) {
  printf("Chassis() 1\n");
}

#if 1
int main() {
  printf("1\n");
  Cabinet cabinet("PC Cabinet");
  printf("2\n");
#if 1
  Chassis chassis("PC Chassis");
  printf("3\n");
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
  FloppyDisk floppyDisk("3.5in Floppy");
  printf("8\n");
  chassis.Add(&floppyDisk);
  printf("10\n");
//  cout << "The net price is " << chassis->NetPrice() << endl;
  printf("The net price is %d\n", chassis.NetPrice());
#endif
  return 0;
}
#endif

