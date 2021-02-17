// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: succeed - valid program
class Array {
  int[] array;

  public static void main() {
    System.out.println(this.array[3]);
    System.out.println(3[4]);
  }
}
