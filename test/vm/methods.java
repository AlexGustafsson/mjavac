// mjavac test file header
// header: 13
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 7
// 1
// 2
// 3
// 0
// 1
// 7
// 7
//
class Test {
  public static void main() {
    Foo.printAllParameters(1, 2, 3, false, true);
    System.out.println(Bar.sum(5, 2));
    System.out.println(Bar.sum(5, 2));
  }
}

class Foo {
  public static void printAllParameters(int a, int b, int c, boolean d, boolean e) {
    System.out.println(a, b, c, d, e);
  }
}

class Bar {
  public static int sum(int a, int b) {
    return a + b;
  }
}
