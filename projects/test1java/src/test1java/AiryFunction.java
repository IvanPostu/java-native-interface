package test1java;

import java.util.List;

public final class AiryFunction {
    private AiryFunction() {
    }

    public static native double ai(double x);

    public static native double bi(double x);

    public static native double ai(Double x);

    public static native double bi(Double x);

    public static native double[] ai(List<Double> numbers);

}
