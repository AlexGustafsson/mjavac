// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: succeed - valid program
class Conditionals {
  public static void main(int i) {
    if (i == 0) {
      System.out.println(0);
      System.out.println(0);
    } else
      System.out.println(i);
  }
}
