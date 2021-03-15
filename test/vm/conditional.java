// mjavac test file header
// header: 14
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 9
// 1
// 0
// 1
// 1
// 0
// 1
// 0
// 1
// 1
//

class Test {
  public static void main() {
    int a = 1;
    int b = 0;
    int c = 2;
    boolean yes = true;
    boolean no = false;

    if (b < a)
      System.out.println(1);
    else
      System.out.println(0);

    if (a < b)
      System.out.println(1);
    else
      System.out.println(0);

    if (a < c) {
      if (b < a) {
        System.out.println(1);
      } else {
        System.out.println(0);
      }
    } else {
      System.out.println(0);
    }

    if (yes)
      System.out.println(1);
    else
      System.out.println(0);

    if (no)
      System.out.println(1);
    else
      System.out.println(0);

    if (yes || no)
      System.out.println(1);
    else
      System.out.println(0);

    if (yes && no)
      System.out.println(1);
    else
      System.out.println(0);

    if (yes && !no)
      System.out.println(1);
    else
      System.out.println(0);

    if (!(a < b))
      System.out.println(1);
    else
      System.out.println(0);
  }
}
