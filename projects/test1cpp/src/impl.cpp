#include "./jni/test1java_TestNative.h"
#include "./jni/test1java_example1_Animal.h"
#include "./jni/test1java_example1_Dog.h"
#include <cstdlib>

JNIEXPORT jboolean JNICALL Java_test1java_TestNative_isOdd(JNIEnv *env,
                                                           jobject obj,
                                                           jint num) {
  jboolean isOdd = num % 2 == 0;
  return isOdd;
}

JNIEXPORT jdoubleArray JNICALL Java_test1java_TestNative_linspace(
    JNIEnv *env, jobject onj, jdouble start, jdouble end, jint num) {

  jdouble *carr = (jdouble *)malloc(sizeof(jdouble) * num);
  jdouble dx = (end - start) / (num - 1.0);
  for (int i = 0; i < num; i++) {
    *(carr + i) = start + i * dx;
  }

  jdoubleArray jarr = env->NewDoubleArray(num);
  env->SetDoubleArrayRegion(jarr, 0, num, carr);
  free(carr);

  return jarr;
}

JNIEXPORT jdouble JNICALL Java_test1java_TestNative_sumOfDoubleArray(
    JNIEnv *env, jobject obj, jdoubleArray jArray) {

  jint len = env->GetArrayLength(jArray);
  jdouble *carr = (jdouble *)malloc(sizeof(jdouble) * len);

  env->GetDoubleArrayRegion(jArray, 0, len, carr);

  jdouble result = 0.0;
  for (int i = 0; i < len; i++) {
    result += *(carr + i);
  }

  free(carr);
  return result;
}

JNIEXPORT jdouble JNICALL Java_test1java_TestNative_multiply(JNIEnv *env,
                                                             jobject obj,
                                                             jdouble a,
                                                             jdouble b) {
  jdouble result = a * b;
  return result;
}
