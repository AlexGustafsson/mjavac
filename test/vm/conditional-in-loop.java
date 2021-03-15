// mjavac test file header
// header: 10
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 5
// 0
// 1
// 6
// 7
// 8
//
class Test {
  public static void main() {
    int i = 0;
    while (i < 9) {
      if (i < 2 || 5 < i)
        System.out.println(i);
      i = i + 1;
    }
  }
}
