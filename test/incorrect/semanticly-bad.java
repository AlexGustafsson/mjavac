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
    5 = 10;
    true = false;
    this = 1;
    this.nonValidAssigns = 10;
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
}

class Calls2 {
  private void nonPublicStaticMain() {
    Calls.notCallable();
    Calls.callable();
    Calls.nonCallable2();
  }
}
