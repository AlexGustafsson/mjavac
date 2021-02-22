// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: fail - invalid program
class InvalidTypes {
  public static void main() {
    int a = 5;
    System.out.println(a.length); // Invalid use of ".length" on int

    int[] array;
    System.out.println(array.length); // Permitted use of ".length" on an array

    boolean b;
    int c;
    b = b * a; // Bad use of two different types

    array[b] = 1; // Invalid, cannot use a boolean as an index
  }
}
