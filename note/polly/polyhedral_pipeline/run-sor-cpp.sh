#CC=gcc
CC=clang

${CC} -O0 sor.cpp -o sor
${CC} -O0 sor2.cpp -o sor2
${CC} -O0 sor3.cpp -o sor3
${CC} -O3 sor.cpp -o sor.O3
${CC} -O3 sor2.cpp -o sor2.O3
${CC} -O3 sor3.cpp -o sor3.O3

echo "${CC} -O0:"
echo "sor:"
time ./sor > sor.out
echo "sor2:"
time ./sor3 > sor2.out
echo "sor3:"
time ./sor3 > sor3.out
diff sor.out sor3.out
echo "${CC} -O3:"
echo "sor:"
time ./sor.O3 > sor.O3.out
echo "sor2:"
time ./sor2.O3 > sor2.O3.out
echo "sor3:"
time ./sor3.O3 > sor3.O3.out
diff sor.O3.out sor3.O3.out

