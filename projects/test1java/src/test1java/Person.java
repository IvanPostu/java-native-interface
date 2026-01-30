package test1java;

public class Person {

    static String EXAMPLE_FIELD = "example";
    static int EXAMPLE_INT_FIELD = 22;

    private int age = 33;
    private String name;

    public Person(String name) {
        this.name = name;
    }

    public String getName(int i) {
        return name + " " + i;
    }

    public String getName() {
        return name;
    }

    public static String getExampleField(int i) {
        return EXAMPLE_FIELD + " " + i;
    }

    @Override
    public String toString() {
        return String.format("name=%s,age=%s,EXAMPLE_FIELD=%s,EXAMPLE_INT_FIELD=%s",
                name, age, EXAMPLE_FIELD, EXAMPLE_INT_FIELD);
    }

}
