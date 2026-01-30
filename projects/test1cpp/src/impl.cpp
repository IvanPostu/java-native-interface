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
  jdouble *carr = new jdouble[len];

  env->GetDoubleArrayRegion(jArray, 0, len, carr);

  jdouble result = 0.0;
  for (int i = 0; i < len; i++) {
    result += *(carr + i);
  }

  delete[] carr;
  return result;
}

JNIEXPORT jdouble JNICALL Java_test1java_TestNative_multiply(JNIEnv *env,
                                                             jobject obj,
                                                             jdouble a,
                                                             jdouble b) {
  jdouble result = a * b;
  return result;
}

JNIEXPORT jobjectArray JNICALL Java_test1java_TestNative_ones(JNIEnv *env,
                                                              jobject obj,
                                                              jint rows,
                                                              jint cols) {

  jclass double_array_class = env->FindClass("[D");
  jobjectArray obj_array = env->NewObjectArray(rows, double_array_class, 0);

  for (int k = 0; k < rows; k++) {
    jdouble *carr = (jdouble *)malloc(sizeof(jdouble) * cols);
    jdoubleArray jarray = env->NewDoubleArray(cols);

    for (int i = 0; i < cols; i++) {
      *(carr + i) = 1.0;
    }
    env->SetDoubleArrayRegion(jarray, 0, cols, carr);
    env->SetObjectArrayElement(obj_array, k, jarray);

    free(carr);
  }
  return obj_array;
}

JNIEXPORT jdouble JNICALL Java_test1java_TestNative_max(JNIEnv *env,
                                                        jobject obj,
                                                        jobjectArray arr) {
  jdouble max_value = 0.0;
  jint rows = env->GetArrayLength(arr);

  for (int i = 0; i < rows; i++) {
    jdoubleArray jarray = (jdoubleArray)(env->GetObjectArrayElement(arr, i));
    jint columns = env->GetArrayLength(jarray);
    jdouble *carray = new jdouble[columns];
    env->GetDoubleArrayRegion(jarray, 0, columns, carray);
    for (int k = 0; k < columns; k++) {
      if (max_value < *(carray + k)) {
        max_value = *(carray + k);
      }
    }
    delete[] carray;
  }

  return max_value;
}
