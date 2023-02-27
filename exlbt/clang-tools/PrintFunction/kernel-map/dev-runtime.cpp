// This is demo for how device-runtime work with KernelMap_t.

#include "kernel-map.h"
#include <cstdio>

extern KernelMap_t KM[];

struct Command {
  int Id;
  void *Msg;
};

// Get Command from Host.
void GetCmd(Command &Cmd) {
  // The following is demo for running Kernel Id 0 and 1 alternately.
  static int count = 0;
  count = count % 2; 
  Cmd.Id = count;
  count++;
}

int main() {
  Command cmd;
  int x = 100;
  int y = 200;
  int i = 0;

  GetCmd(cmd);
  i = cmd.Id;
  printf("Run Node: %s\n", KM[i].GraphNodeName);
  KM[i].KernelFunc("%d %d", x, y);

  GetCmd(cmd);
  i = cmd.Id;
  printf("Run Node: %s\n", KM[i].GraphNodeName);
  KM[i].KernelFunc("%d %d", x, y);
}
