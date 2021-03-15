// mjavac test file header
// header: 6
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 1
// 15
//
class Test {
  public static void main(String[] args) {
    System.out.println(Test.cumulativeSum(5, 0));
  }

  public static int cumulativeSum(int start, int sum) {
    if (start < 1)
      return sum;

    sum = Test.cumulativeSum(start - 1, sum + start);
    return sum;
  }
}
