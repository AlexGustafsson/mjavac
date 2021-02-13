class Application {
  public static void main() {
    System.out.println(new Adder().add(1, 2));
  }
}

class Adder {
  public int add(int a, int b) {
    return a + b;
  }
}
