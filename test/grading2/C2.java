// mjavac test file header
// header: 107
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 100
// 0
// 100
// 199
// 297
// 394
// 490
// 585
// 679
// 772
// 864
// 955
// 1045
// 1134
// 1222
// 1309
// 1395
// 1480
// 1564
// 1647
// 1729
// 1810
// 1890
// 1969
// 77
// 76
// 75
// 74
// 73
// 72
// 71
// 70
// 69
// 68
// 67
// 66
// 65
// 64
// 63
// 62
// 61
// 60
// 59
// 58
// 57
// 56
// 55
// 54
// 53
// 52
// 51
// 50
// 49
// 48
// 47
// 46
// 45
// 44
// 43
// 42
// 41
// 40
// 39
// 38
// 37
// 36
// 35
// 34
// 33
// 32
// 31
// 30
// 29
// 28
// 27
// 26
// 25
// 24
// 23
// 22
// 21
// 20
// 19
// 18
// 17
// 16
// 15
// 14
// 13
// 12
// 11
// 10
// 9
// 8
// 7
// 6
// 5
// 4
// 3
// 2
// 1
// 5050
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
      if (50 < num && sum < 2000) {
    	  System.out.println(sum);
      } else {
    	  System.out.println(num);
      }
      sum = sum + num;
      num = num - 1;
    }
    return sum;
  }
}
