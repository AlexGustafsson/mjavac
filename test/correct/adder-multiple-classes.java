// mjavac test file header
// header: 6
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 1
// 3
//

class Application {
  public static void main() {
    Adder adder = new Adder();
    System.out.println(adder.add(1, 2));
  }
}

class Adder {
  public int add(int a, int b) {
    return a + b;
  }
}
