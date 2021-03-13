// mjavac test file header
// header: 10
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 5
// 0
// 1
// 2
// 1
//
class RecursiveSum {
  public static void main(String[] a){
	 System.out.println(new Test().Sum(10000));
  }
}

class Test {
  public int Sum(int num) {
    int sum;
    if (num < 2)
      sum = 1;
    else
      sum = num + this.Sum(num-1);
    return sum;
  }
}
