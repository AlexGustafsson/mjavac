// mjavac test file header
// header: 10
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 5
// 0
// 1
// 2
// 3
// 4
//
class Test {
  public static void main() {
    int i = 0;
    while (i < 5) {
      System.out.println(i);
      i = i + 1;
    }
  }
}
