// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: succeed - valid program
class Loops {
  public static void main() {
    while (true)
      System.out.println(true);

    while(1 < 2) {
      System.out.println(true);
      System.out.println(false);
    }
  }
}
