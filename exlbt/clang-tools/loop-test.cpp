int expr() {
  return 100;
}

int main() {
  int res = 0;
//  for (int i = 0; i < 100 ; i++) {
  for (int i= 0; i < expr(); ++i) {
    res = res + i;
  }
  return res;
}
