// mjavac test file header
// header: 12
// parse: succeed - valid syntax
// semantics: succeed - valid program
class Math {
  public static void main() {
    System.out.println(1 + 3);
    System.out.println(1 + 2 + 1);
    System.out.println(1 + 2 + 1 + 1 - 1);
    System.out.println((1 + 1) + (2));
    System.out.println(1 * 5 / 5 + 1 - 2 * -1); // (1 * 2) / 5 + 1 - (2 * (-1)) = 4
    System.out.println(!true);
  }
}
