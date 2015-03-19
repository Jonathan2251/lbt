#include <vector>
#include <cstdio>

#define COUNT1  91723
#define COUNT2  500000
#define COUNT (COUNT1+COUNT2)

using namespace std;

vector<int> v;
long long sum = 0;

int produce() {
  for (int i = 0; i < COUNT1; i++) {
    v.insert(v.end(), i);
  }
  for (int i = 0; i < COUNT2; i++) {
    v.insert(v.begin()+(i%COUNT1), i);
  }
  return 0;
}

int consume() {
  for (int i = 0; i < COUNT; i++) {
    sum += v[i];
  }
  return sum;
}

int main() {
  produce();
  consume();
  printf("sum: %lld\n", sum);

  return 0;
}
