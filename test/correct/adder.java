// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: succeed - valid program
class Adder {
  int a;
  int b;

  public static int main() {
    int result = 0;
    result = a + b;
    System.out.println(result);
  }
}
