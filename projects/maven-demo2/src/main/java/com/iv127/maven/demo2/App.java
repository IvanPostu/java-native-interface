package com.iv127.maven.demo2;

import com.iv127.maven.demo2.misc.Unsafe;

import java.lang.reflect.Field;
import java.util.Arrays;

public class App {

    static {
        NativeUtils.loadLibraryFromJar("/native/libimpl.so");
        registerNatives();
    }

    public static native void doubleArrayAddViaRegionAccessor(double[] arr, double x);

    public static native void doubleArrayAddViaElementsAccessor(double[] arr, double x);

    public static native void doubleArrayAddViaCriticalAccessor(double[] arr, double x);

    private static native void printStringUTF(String s);

    private static native void printStringCriticalUTF(String s);

    // Convention name
    private static native void registerNatives();

    private static native int doubleAge(int age);

    private static native int tripleAge(int age);

    public static void main(String[] args) throws Exception {
//        demo1();
//        demo2();
//        demo3();
//        demo4();
        demo5();
    }

    private static void demo5() throws Exception {
        Unsafe unsafe = getUnsafeInstance();
        long pointer = Unsafe.allocateMemory(10);
        System.out.printf("address: 0x%s%n", Long.toHexString(pointer));

        unsafe.putInt(pointer, 90);
        System.out.println(unsafe.getInt(pointer));

        unsafe.putDouble(pointer + 4, 22.33);
        System.out.println(unsafe.getDouble(pointer + 4));

        unsafe.putLong(pointer + 4 + 8, 999999181);
        System.out.println(unsafe.getLong(pointer + 4 + 8));

        unsafe.putByte(pointer + 4 + 8 + 8, (byte)122);
        System.out.println(unsafe.getByte(pointer + 4 + 8 + 8));

        Unsafe.freeMemory(pointer);
    }

    private static Unsafe getUnsafeInstance() throws ClassNotFoundException, NoSuchFieldException, IllegalAccessException {
        Class<?> clas = Class.forName("com.iv127.maven.demo2.misc.Unsafe");
        Field field = clas.getDeclaredField("theUnsafe");
        boolean accessible = field.canAccess(null);
        Unsafe unsafe = null;
        try {
            field.setAccessible(true);
            unsafe = (Unsafe) field.get(null);
        } finally {
            field.setAccessible(accessible);
        }
        return unsafe;
    }

    private static void demo4() {
        Person p1 = Person.allocate("Jim", 22);
        Person p2 = Person.allocate("Rick", 65);

        p1.sayHi();
        p2.sayHi();

        Person.free(p1);
        Person.free(p2);
    }

    private static void demo3() {
        System.out.println("doubleAge(3)=" + App.doubleAge(3));
        System.out.println("doubleAge(6)=" + App.doubleAge(6));
        System.out.println("doubleAge(33)=" + App.doubleAge(33));

        System.out.println("tripleAge(3)=" + App.tripleAge(3));
        System.out.println("tripleAge(6)=" + App.tripleAge(6));
        System.out.println("tripleAge(33)=" + App.tripleAge(33));
    }

    private static void demo2() {
        printStringUTF("Test 123");
        printStringCriticalUTF("Test 123");
    }

    private static void demo1() {
        {
            double x = 10.5;
            double[] arr = {1, 2, 3, 55.5};
            double[] result = arr.clone();
            App.doubleArrayAddViaRegionAccessor(result, x);
            System.out.printf("Initial: %s, Result:%s%n", Arrays.toString(arr), Arrays.toString(result));
        }
        {
            double x = 10.5;
            double[] arr = {1, 2, 3, 55.5};
            double[] result = arr.clone();
            App.doubleArrayAddViaElementsAccessor(result, x);
            System.out.printf("Initial: %s, Result:%s%n", Arrays.toString(arr), Arrays.toString(result));
        }
        {
            double x = 10.5;
            double[] arr = {1, 2, 3, 55.5};
            double[] result = arr.clone();
            App.doubleArrayAddViaCriticalAccessor(result, x);
            System.out.printf("Initial: %s, Result:%s%n", Arrays.toString(arr), Arrays.toString(result));
        }
    }

}
