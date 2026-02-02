package com.iv127.maven.demo2;

public class Person {

    private long pointer;

    private static native long allocate0(String name, int age);
    private static native void free0(long pointer);

    private static native String getName0(long pointer);
    private static native int getAge0(long pointer);
    private static native void sayHi0(long pointer);

    public static Person allocate(String name, int age) {
        return new Person(name, age);
    }

    public static void free(Person person) {
        Person.free0(person.pointer);
    }

    private Person(String name, int age) {
        this.pointer = Person.allocate0(name, age);
    }

    public String getName() {
        return getName0(pointer);
    }

    public int getAge() {
        return getAge0(pointer);
    }

    public void sayHi() {
        sayHi0(pointer);
    }

}
