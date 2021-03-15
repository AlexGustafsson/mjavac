// mjavac test file header
// header: 7
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 1
// 100
//
class SumUtility {
  public static void main(String[] a){
    System.out.println(new Test().Sum(100));
  }
}

class Test {
  public int Sum(int num) {
    int sum;
    sum = 0;
    while (0 < num) {
      if (sum < 100) {
    	  sum = sum + num;
      }
      num = num - 1;
    }
    return sum;
  }
}
