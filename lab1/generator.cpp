#include "iostream"
#include "string"
#include <fstream>
#include "func_1.h"
using namespace std;

int main(void) {
  string type1, type2;
  cin >> type1;
  cin >> type2;
  cout << type1 << ' ' << type2 << endl;
  ofstream fout("func_1.cpp");
  fout << "#include <iostream>" << endl;
  fout << "#include \"func_1.h\"" << endl;
  fout << "int f(void) {" << endl;
  fout << type1 << " var1 = 1;" << endl;
  fout << type1 << " var2 {3};" << endl;
  fout << "std::cout << var1 + var2 << std::endl;" << endl;
  fout << "std::cout << var1 - var2 << std::endl;" << endl;
  fout << "std::cout << var1 / var2 << std::endl;" << endl;
  fout << "std::cout << var1 * var2 << std::endl;" << endl;
  fout << "return 0;" << endl;
  fout << "}" << endl;
  fout << "int main() {" << endl;
  fout << " f();" << endl;
  fout << "}" << endl;
  fout.close();
  return 0;
}