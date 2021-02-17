// mjavac test file header
// header: 3
// parse: fail - invalid syntax
class MyClass {
  public static void main() {
    int test error recovery
    int test = 5;
  }

  public missingType() {
    int this whole method should be skipped
  }

  public void test() {
    int thisWorks;
    = thisdoesnt
  }

  public void correctMethod() {

  }
}
