// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: fail - invalid program
class Alfa extends UndefinedClass { // Bad, not defined
  int foo = 10;
}

class Test {
  public static void main() {
    Alfa a = new Alfa(); // OK, Alfa is defined
  }
}
