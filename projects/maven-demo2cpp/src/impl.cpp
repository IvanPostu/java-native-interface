#include "./jni/com_iv127_maven_demo2_App.h"
#include <cstdio>
#include <cstdlib>

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

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_App_printStringUTF(
    JNIEnv *env, jclass App_class, jstring jstr) {

  jboolean is_copy = 0;
  const char *cstr = env->GetStringUTFChars(jstr, &is_copy);
  printf("%s - is_copy=%d\n", cstr, is_copy);
  env->ReleaseStringUTFChars(jstr, cstr);
}

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_App_printStringCriticalUTF(
    JNIEnv *env, jclass App_class, jstring jstr) {

  jboolean is_copy = 0;
  const jchar *jvalue = env->GetStringCritical(jstr, &is_copy);
  jsize len = env->GetStringLength(jstr);

  for (int i = 0; i < len; i++) {
    putchar((char)*(jvalue + i));
  }

  printf(" - is_copy=%d\n", is_copy);
  env->ReleaseStringCritical(jstr, jvalue);
}

jint doubleAge(JNIEnv *env, jclass App, jint age);

JNIEXPORT void JNICALL
Java_com_iv127_maven_demo2_App_registerNatives(JNIEnv *env, jclass Appclass) {

  JNINativeMethod doubleAge_method;
  doubleAge_method.name = (char *)"doubleAge";
  doubleAge_method.signature = (char *)"(I)I";
  doubleAge_method.fnPtr = (void *)doubleAge;

  env->RegisterNatives(Appclass, &doubleAge_method, 1);
}

jint doubleAge(JNIEnv *env, jclass App, jint age) { return age * 2; }
jint tripleAge(JNIEnv *env, jclass App, jint age) { return age * 3; }

static JNINativeMethod methods[] = {
    {(char *)"tripleAge", (char *)"(I)I", (void *)tripleAge}};

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env;
  if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  }

  jclass cls = env->FindClass("com/iv127/maven/demo2/App");
  if (cls == NULL)
    return JNI_ERR;

  jint methods_len = sizeof(methods) / sizeof(methods[0]);
  if (env->RegisterNatives(cls, methods, methods_len) < 0)
    return JNI_ERR;

  return JNI_VERSION_1_6;
}
