// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: fail - invalid program
class BasicClass {

}

class ReturnTypes {
  static void main() {
    int integer = ReturnTypes.booleanMethod(); // Invalid, bad assignment
    boolean bool = ReturnTypes.classMethod(); // Invalid, bad assignment
    BasicClass basicClass = ReturnTypes.arrayMethod(); // Invalid, bad assignment
    int[] array = ReturnTypes.integerMethod(); // Invalid, bad assignment
    int voidValue = ReturnTypes.voidMethod(); // Invalid, bad assignment
  }

  static int integerMehtod() {
    return; // Must return a value
  }

  static boolean booleanMethod() {
    return 25; // Bad value
  }

  static void voidMethod() {
    return 1; // Fail - cannot return a value in a void method
  }

  static UndefinedClass undefinedClassMethod() { // Bad, no such class
    return new UndefinedClass(); // Bad, no such class
  }
}
