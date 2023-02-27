/* ~/git/lbt/exlbt/riscv$ ~/riscv/riscv_newlib/bin/riscv64-unknown-elf-gcc -c func_hello_start.s 
~/git/lbt/exlbt/riscv$ ~/riscv/riscv_newlib/bin/riscv64-unknown-elf-gcc -c caller_hello.c 
~/git/lbt/exlbt/riscv$ ~/riscv/riscv_newlib/bin/riscv64-unknown-elf-ld caller_hello.o func_hello_start.o
~/git/lbt/exlbt/riscv$ ~/riscv/riscv_newlib/bin/riscv64-unknown-elf-ld caller_hello.o func_hello_start.o -o a.out
~/git/lbt/exlbt/riscv$ ~riscv/git/qemu/build/qemu-riscv64 a.out
Hello World
*/

extern void hello();

int main() {
  hello();
}
