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
class Bar {
	public static void main(String[] a) {
		System.out.println(new Bar1().foo());
	}
}

class Bar1 {
	public int foo() {
		int aux;
		boolean aux2;
		aux = 1;
		aux2 = true;
		aux = this.foo2(aux);
		aux = this.foo3(2,5,aux2);
		return aux;
	}

	public int foo2(int p1) {
		if(p1 < 2)
			System.out.println(10);
		else
			System.out.println(0);

		return 1;
	}

	public int foo3(int p1, int p2, boolean p3) {
		if (p3) {
			System.out.println(1);
		} else {
			System.out.println(0);
		}
		System.out.println(p1);
		System.out.println(p2-p1);
		return 4;
	}
}
