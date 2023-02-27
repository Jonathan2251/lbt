#ifndef LT_KERNEL_MAP_H
#define LT_KERNEL_MAP_H

struct KernelMap_t {
  int GraphNodeId;
  char GraphNodeName[256];
  int (*KernelFunc)(const char *format, ...);
};

#endif
