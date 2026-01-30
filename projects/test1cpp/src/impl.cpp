#include "./jni/test1java_TestNative.h"
#include "./jni/test1java_example1_Animal.h"
#include "./jni/test1java_example1_Dog.h"
#include <cstdio>
#include <cstdlib>

JNIEXPORT jstring JNICALL Java_test1java_example1_Animal_bark(JNIEnv *env,
                                                              jobject obj) {
  return env->NewStringUTF("Animal bark");
}

JNIEXPORT jstring JNICALL Java_test1java_example1_Dog_bark(JNIEnv *env,
                                                           jobject obj) {
  return env->NewStringUTF("Dog bark");
}

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

JNIEXPORT jstring JNICALL Java_test1java_TestNative_speak__(JNIEnv *env,
                                                            jobject obj) {
  const char *text = "Hello";
  jstring result = env->NewStringUTF(text);
  return result;
}

JNIEXPORT jstring JNICALL Java_test1java_TestNative_speak__Ljava_lang_String_2(
    JNIEnv *env, jobject obj, jstring str) {
  const char *text = env->GetStringUTFChars(str, 0);
  int len = snprintf(NULL, 0, "Hello %s", text) + 1;
  char *result = (char *)malloc(len);
  snprintf(result, len, "Hello %s", text);
  env->ReleaseStringUTFChars(str, text);
  jstring result1 = env->NewStringUTF(result);
  free(result);
  return result1;
}

JNIEXPORT void JNICALL Java_test1java_TestNative_nativePrintln(JNIEnv *env,
                                                               jobject obj,
                                                               jstring str) {
  const char *text = env->GetStringUTFChars(str, 0);
  printf("%s\n", text);
}

JNIEXPORT jstring JNICALL Java_test1java_TestNative_nativeConcat(JNIEnv *env,
                                                                 jobject obj,
                                                                 jstring str1,
                                                                 jstring str2) {
  const char *text1 = env->GetStringUTFChars(str1, 0);
  const char *text2 = env->GetStringUTFChars(str2, 0);
  int len = snprintf(NULL, 0, "%s%s", text1, text2) + 1;

  char *result = (char *)malloc(len);
  snprintf(result, len, "%s%s", text1, text2);
  env->ReleaseStringUTFChars(str1, text1);
  env->ReleaseStringUTFChars(str2, text2);
  jstring result1 = env->NewStringUTF(result);
  free(result);
  return result1;
}
