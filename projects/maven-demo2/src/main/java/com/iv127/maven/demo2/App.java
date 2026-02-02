package com.iv127.maven.demo2;

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

    public static void main(String[] args) {
//        demo1();
//        demo2();
        demo3();
    }

    private static void demo3() {
        System.out.println("doubleAge(3)=" + App.doubleAge(3));
        System.out.println("doubleAge(6)=" + App.doubleAge(6));
        System.out.println("doubleAge(33)=" + App.doubleAge(33));
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
