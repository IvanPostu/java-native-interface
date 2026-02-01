package com.iv127.maven.demo2;

import java.util.Arrays;

public class App {

    static {
        NativeUtils.loadLibraryFromJar("/native/libimpl.so");
    }

    public static native void printHello();

    public static native void doubleArrayAddViaRegionAccessor(double[] arr, double x);

    public static native void doubleArrayAddViaElementsAccessor(double[] arr, double x);

    public static native void doubleArrayAddViaCriticalAccessor(double[] arr, double x);

    public static void main(String[] args) {
        demo1();
    }

    private static void demo1() {
        System.out.println("Hello World!");
        App.printHello();

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
