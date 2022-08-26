/*
~/llvm/14.x/llvm-project/build/bin/clang-query test1.cpp --
  $ clang-query> m varDecl()
*/

int addTwo(int num) 
{ 
    return num + 2; 
} 

int main(int, char**) 
{ 
    return addTwo(3); 
} 
