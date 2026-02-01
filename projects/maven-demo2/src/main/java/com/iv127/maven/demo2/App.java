package com.iv127.maven.demo2;

/**
 * Hello world!
 */
public class App {

    static {
        NativeUtils.loadLibraryFromJar("/native/libimpl.so");
    }

    public static native void printHello();

    public static void main(String[] args) {
        System.out.println("Hello World!");
        App.printHello();
    }

}
