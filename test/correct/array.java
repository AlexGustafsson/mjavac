// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: succeed - valid program
class Array {
  int[] array;

  public static void main() {
    array[0] = 3;
    array[1] = 5;
    System.out.println(this.array[3]);
  }
}
