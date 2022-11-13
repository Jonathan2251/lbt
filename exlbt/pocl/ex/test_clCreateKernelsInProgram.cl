// Modify the same file in tests/runtime from "Hello\nWorld" to "Hello World!" 
// for identification that this .cl triggered by runtime compilation since
// .c file call it directly without ahead of compiling .cl.

__kernel void
kernel_1()
{
  printf("Hello ");
}

__kernel void
kernel_2()
{
  printf("World!\n");
}

void
notakernel()
{
  printf("This should not get printed!\n");
}

