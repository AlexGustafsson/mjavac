// mjavac test file header
// header: 10
// parse: succeed - valid syntax
// semantics: succeed - valid program
class Config {
  public int getMaxValue() {
    return 2;
  }

  public boolean isBelowMaxValue(int i) {
    return i < 2;
  }
}

class ConditionsTest {
  public static void main() {
    int i = 0;
    while (i < Config.getMaxValue()) {
      System.out.println(i);
      i = i + 1;
    }

    if (Config.isLessThanOrEqualToMaxValue(i))
      System.out.println(i);

    int useBoolsLikeInC = 1;
    if (useBoolsLikeInC)
      System.out.println(1);

    int useBoolsLikeInC2 = 0;
    if (useBoolsLikeInC2)
      System.out.println(1);
  }
}
