package com.iv127.maven.demo2.misc;

public final class Unsafe {

    public static final Unsafe theUnsafe = new Unsafe();

    private Unsafe() {
    }

    public static native long allocateMemory(long bytes);

    public static native void freeMemory(long address);

    public native void putInt(long address, int x);

    public native int getInt(long address);

    public native void putDouble(long address, double x);

    public native double getDouble(long address);

    public native void putLong(long address, long x);

    public native long getLong(long address);

    public native void putByte(long address, byte x);

    public native byte getByte(long address);

}
