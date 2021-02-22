// mjavac test file header
// header: 11
// parse: succeed - valid syntax
// semantics: succeed - valid program
// output: 5
// 5
// 10
// 10
// 5
//
class Alfa {
  int foo = 5;
  int bar = 10;

  public int getFoo() {
    return this.foo;
  }

  public int getBar() {
    return this.bar;
  }
}

class Bravo extends Alfa {
  int foo = 10;

  public int getBar() {
    return 5;
  }
}

class Test {
  public static void main() {
    Alfa a = new Alfa();
    Bravo b = new Bravo();

    System.out.println(a.getFoo());
    System.out.println(a.getBar());
    System.out.println(b.getFoo());
    System.out.println(b.getBar());
  }
}
