// mjavac test file header
// header: 5
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 0
class Test {
  public static void main(String[] arguments) {
    int a = true;
    if (a) {
      System.out.println(1);
    } else {
      System.out.println(0);
    }

    int i = 0;
    while (i < 5) {
      System.out.println(i);
      i = i + 1;
    }

    i = 0;

    System.out.println(0);
  }
}
