// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: fail - invalid program
class BasicClass {
  int foo = 1;
}

class ReturnTypes {
  static void main() {
    int integer = ReturnTypes.booleanMethod(); // Invalid, bad assignment
    boolean bool = ReturnTypes.classMethod(); // Invalid, bad assignment
    BasicClass basicClass = ReturnTypes.arrayMethod(); // Invalid, bad assignment
    int[] array = ReturnTypes.integerMethod(); // Invalid, bad assignment
    int voidValue = ReturnTypes.voidMethod(); // Invalid, bad assignment
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

  static void voidMethod() {
    return 1; // Fail - cannot return a value in a void method
  }

  static UndefinedClass undefinedClassMethod() { // Bad, no such class
    return new UndefinedClass(); // Bad, no such class
  }
}
