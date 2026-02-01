#include "./jni/com_iv127_maven_demo2_App.h"
#include <cstdio>
#include <cstdlib>

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_App_printHello(JNIEnv *env,
                                                                 jclass App) {
  printf("Hello\n");
}

JNIEXPORT void JNICALL
Java_com_iv127_maven_demo2_App_doubleArrayAddViaRegionAccessor(
    JNIEnv *env, jclass App, jdoubleArray java_array, jdouble x) {

  jclass double_array_class = env->FindClass("[D");
  jint len = env->GetArrayLength(java_array);
  jdouble *carr = (jdouble *)malloc(sizeof(jdouble) * len);
  (*env).GetDoubleArrayRegion(java_array, 0, len, carr);

  for (int i = 0; i < len; i++) {
    *(carr + i) = *(carr + i) + x;
  }
  (*env).SetDoubleArrayRegion(java_array, 0, len, carr);
  free(carr);
}

JNIEXPORT void JNICALL
Java_com_iv127_maven_demo2_App_doubleArrayAddViaElementsAccessor(
    JNIEnv *env, jclass App, jdoubleArray java_array, jdouble x) {

  jclass double_array_class = env->FindClass("[D");
  jint len = env->GetArrayLength(java_array);
  unsigned char is_copy = false;
  jdouble *current_arr = env->GetDoubleArrayElements(java_array, &is_copy);
  for (int i = 0; i < len; i++) {
    *(current_arr + i) = *(current_arr + i) + x;
  }
  env->ReleaseDoubleArrayElements(java_array, current_arr, 0);
}

JNIEXPORT void JNICALL
Java_com_iv127_maven_demo2_App_doubleArrayAddViaCriticalAccessor(
    JNIEnv *env, jclass App, jdoubleArray java_array, jdouble x) {

  jclass double_array_class = env->FindClass("[D");
  jint len = env->GetArrayLength(java_array);
  unsigned char is_copy = false;
  jdouble *current_arr =
      (jdouble *)env->GetPrimitiveArrayCritical(java_array, &is_copy);
  for (int i = 0; i < len; i++) {
    *(current_arr + i) = *(current_arr + i) + x;
  }
  env->ReleasePrimitiveArrayCritical(java_array, current_arr, 0);
}
