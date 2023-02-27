#include <vector>
#include <string>
#include <cstdio>
#include <assert.h>

using namespace std;

vector<string> getArrayFromString(string &str) {
  static vector<string> d;

  d.clear();
  size_t begin = str.find_first_of("{");
  assert(begin != string::npos);
  for (begin++; begin < str.size() && str[begin] == ' '; begin++) {}
  size_t end = str.find_first_of("}", begin);
  assert(end != string::npos);
  for (end--; end > 0 && str[end] == ' '; end--) {}
  if (begin > end) {
    printf("{   }, array is empty!");
  }
  assert(begin <= end);
  string str2 = str.substr(begin, end-begin+1);
  printf("str2: %s\n", str2.c_str());

  for (size_t i = 0; i < str2.size(); i++) {
    size_t j = i;
    for (; j < str2.size(); j++) {
      if (str2[j] == ',' || str2[j] == ' ') {
        d.push_back(str2.substr(i, j-i));
        printf("add d: %s\n", str2.substr(i, j-i).c_str());
        break;
      }
    }
    if (j == str2.size()) {
      d.push_back(str2.substr(i, j-i));
      printf("add d: %s\n", str2.substr(i, j-i).c_str());
      break;
    }
    i = j+1;
  }

  printf("d: size: %lu ", d.size());
  for (size_t i = 0; i < d.size(); i++) {
    printf("%s ", d[i].c_str());
  }
  return d;
}

int main() {
//  string s = "Type0 a_D[] = {B, C}";
  string s2 = "Type0 a_B[] = {A}";

//  vector<string> d = getArrayFromString(s);
  vector<string> d2 = getArrayFromString(s2);
}
