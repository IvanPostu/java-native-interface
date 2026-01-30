package test1java;

import java.io.IOException;
import java.lang.reflect.Field;
import java.util.Arrays;

import test1java.example1.Animal;
import test1java.example1.Dog;

public class TestNative {
	private static final String FULL_PATH = System.getenv("PROJECT_ROOT") + "/projects/test1java/src/jni";
	private static final String DYNAMIC_LIB_NAME = "test1java_TestNative";

	static {
		// System.load(FULL_PATH + "/" + "lib" + DYNAMIC_LIB_NAME + ".so");
		// System.load("test1java_TestNative");
		// "/jni/" + "lib" + DYNAMIC_LIB_NAME + ".so"
		loadLibrary();

	}

	public native double multiply(double a, double b);

	public native String speak();

	public native String speak(String s);

	// linspace(0,5,5) -> dx = (5-0)/4; [0, 0+dx, 0+2*dx, 0+3*dx, 0+4*dx]
	// last element = start + (num-1)*dx -> dx=(end-start)/(num-1)
	// (0, 10, num=5) = [ 0. 2.5 5. 7.5 10. ]
	public native double[] linspace(double start, double end, int num);

	public native double sumOfDoubleArray(double[] array);

	public native double[][] ones(int rows, int cols);

	public native double max(double[][] elements);

	public native void nativePrintln(String s);

	public native String nativeConcat(String s1, String s2);

	public native void printPerson(Person person);

	public static native void printStaticPerson();

	public static native void setPerson(Person p, String name, int age, String exampleStaticValue);

	public static void main(String[] args) throws Exception {
		TestNative testNative = new TestNative();
		double result = testNative.multiply(1.1, 2.2);

		System.out.println("multiply(1.1, 2.2)=" + result);
		System.out.println("speak()=" + testNative.speak());
		System.out.println("speak(\"abc\")=" + testNative.speak("abc"));
		System.out.println("Dog.bark()=" + (new Dog().bark()));
		System.out.println("Animal.bark()=" + (new Animal().bark()));
		System.out.println("linspace(0, 20, 10): " + Arrays.toString(testNative.linspace(0.0, 20.0, 10)));
		System.out.println("sumOfDoubleArray(5.5, 10.25): " + testNative.sumOfDoubleArray(new double[] { 5.5, 10.25 }));
		System.out.println("ones(3, 10): " + Arrays.deepToString(testNative.ones(3, 10)));
		System.out.println("max([[3, 55] [10, 76]]): " + testNative.max(new double[][] { { 3, 55 }, { 10, 76 } }));
		testNative.nativePrintln("test native print");
		System.out.println(testNative.nativeConcat("test1-", "test2"));

		playWithReflection();

		Person person = new Person("Bob");
		testNative.printPerson(person);

		TestNative.printStaticPerson();

		TestNative.setPerson(person, "H1", 9909, "11a11");
		System.out.println("qqq3" + person + ", example=" + Person.EXAMPLE_FIELD);
	}

	private static void loadLibrary() {
		if (!JarUtils.isJar()) {
			System.load(FULL_PATH + "/" + "lib" + DYNAMIC_LIB_NAME + ".so");
			return;
		}

		try {
			NativeUtils.loadLibraryFromJar("/jni/" + "lib" + DYNAMIC_LIB_NAME + ".so");
		} catch (IOException e) {
			throw new IllegalStateException(e);
		}
	}

	@SuppressWarnings({ "unchecked" })
	private static void playWithReflection()
			throws NoSuchFieldException, SecurityException, IllegalArgumentException, IllegalAccessException {

		System.out.println("Calling: playWithReflection get ===========");

		Person person = new Person("Bob");
		Class<Person> clazz = (Class<Person>) person.getClass();
		Field f1 = clazz.getDeclaredField("name");

		{
			boolean initialAccessibility = f1.canAccess(person);
			try {
				f1.setAccessible(true);
				System.out.println(f1);
				Object nameObj = f1.get(person);
				System.out.println("name=" + nameObj);
			} finally {
				f1.setAccessible(initialAccessibility);
			}
		}

		// .getField accesses only public field
		Field staticField1 = Person.class.getDeclaredField("EXAMPLE_FIELD");
		Object staticFieldValue1 = staticField1.get(Person.class);
		System.out.println("EXAMPLE_FIELD=" + staticFieldValue1);
		// for private static fields the approach is similar to the one above

		Field staticField2 = Person.class.getDeclaredField("EXAMPLE_INT_FIELD");
		Object staticFieldValue2 = staticField2.get(Person.class);
		System.out.println("EXAMPLE_FIELD=" + staticFieldValue2 + " as int=" + staticField2.getInt(Person.class));

		System.out.println("Calling: playWithReflection set ===========");

		Person p2 = new Person("Dummy name");
		Class<Person> clazz2 = (Class<Person>) person.getClass();
		Field f2 = clazz2.getDeclaredField("name");
		setPrivateField(f2, p2, "James");
		setPrivateField(staticField1, null, "adsfdafdafa"); // set static field through reflection

		System.out.println("qqq1" + p2);
		System.out.println("qqq2" + Person.EXAMPLE_FIELD);

	}

	private static void setPrivateField(Field field, Object fieldOwner, Object value)
			throws IllegalArgumentException, IllegalAccessException {
		boolean initialAccessibility = field.canAccess(fieldOwner);
		try {
			field.setAccessible(true);
			field.set(fieldOwner, value);
		} finally {
			field.setAccessible(initialAccessibility);
		}
	}

}
