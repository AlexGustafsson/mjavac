// mjavac test file header
// header: 10
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 5
// 1
// 1
// 1
// 1
//
class BasicClass {
  int foo = 1;
}

class ReturnTypes {
  static void main() {
    int integer = ReturnTypes.integerMethod();
    boolean bool = ReturnTypes.booleanMethod();
    BasicClass basicClass = ReturnTypes.classMethod();
    int[] array = ReturnTypes.arrayMethod();

    System.out.println(integer);
    System.out.println(bool);
    System.out.println(basicClass.foo);
    System.out.println(array[0]);
  }

  static int integerMethod() {
    return 1;
  }

  static boolean booleanMethod() {
    return true;
  }

  static BasicClass classMethod() {
    return new BasicClass();
  }

  static int[] arrayMethod() {
    int[] array;
    array[0] = 1;
    return array;
  }
}
