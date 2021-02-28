// mjavac test file header
// header: 4
// parse: succeed - valid syntax
// semantics: fail - it looks like someone spent too much time on StackOverflow and too little reading books
class DuplicateClass {
  int duplicateMember;
  int duplicateMember;

  private int duplicateFunction() {

  }

  private int duplicateFunction() {

  }

}

class DuplicateClass {
  private int overloadedFunction() {
    int duplicateVariable;
    int duplicateVariable = 5;
  }

  private int overloadedFunction(int a) {

  }

  private void badParameters(int duplicateParameter, int duplicateParameter) {

  }

  private void badMethodCall(int a, int b) {
    badMethodCall(true, false);
  }

  private void missingValue() {
    this.nonExistingValue = 5;
  }

  private void nonValidAssigns() {
    5 = 10; // Assignment to non-variable
    true = false; // Assignment to non-variable
    this = 1; // Assignment to this
    this.nonValidAssigns = 10; // Assignment to non-defined object value
    int assignmentWithABadType;
    assignmentWithABadType = false;
    nonDefined = false; // Assignment to non-defined identifier value
  }

  private void badValues() {
    boolean[] arrayOfUnsupportedType;
  }

  private void badOperators() {
    int correctInts = 1 + 1;
    int badInts = 1 + false;
    1 + false;
    1 + 1; // Unused result
  }
}

class BadExtend extends NonExistingClass {

}

class BadExtendSelf extends BadExtendSelf {

}

class Calls {
  private static void notCallable() {

  }

  public static void callable() {

  }

  public void nonCallable2() {

  }

  public int multipleReturns() {
    return 1;
    return 2;
    return 3;
  }
}

class Calls2 {
  private void nonPublicStaticMain() {
    Calls.notCallable();
    Calls.callable();
    Calls.nonCallable2();
  }
}
