package test1java.example1;

public class Dog extends Animal {

	@Override
	public native String bark();

	@Override
	public String speak() {
		return "Dog: speak";
	}

}
