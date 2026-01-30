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

JNIEXPORT void JNICALL Java_test1java_TestNative_printPerson(
    JNIEnv *env, jobject TestNativeClass, jobject person) {

  jclass person_class = env->FindClass("test1java/Person");
  jfieldID fieldId1 =
      env->GetFieldID(person_class, "name", "Ljava/lang/String;");
  jfieldID fieldId2 = env->GetFieldID(person_class, "age", "I");

  jobject name_obj = env->GetObjectField(person, fieldId1);
  jstring name = (jstring)name_obj;
  jint age = env->GetIntField(person, fieldId2);

  const char *name_arr = env->GetStringUTFChars(name, 0);

  printf("name=%s, age=%d\n", name_arr, age);
  env->ReleaseStringUTFChars(name, name_arr);
}

JNIEXPORT void JNICALL
Java_test1java_TestNative_printStaticPerson(JNIEnv *env, jclass TestNative) {

  jclass person_class = env->FindClass("test1java/Person");
  jfieldID fieldId1 = env->GetStaticFieldID(person_class, "EXAMPLE_FIELD",
                                            "Ljava/lang/String;");
  jfieldID fieldId2 =
      env->GetStaticFieldID(person_class, "EXAMPLE_INT_FIELD", "I");

  jobject EXAMPLE_FIELD = env->GetStaticObjectField(person_class, fieldId1);
  jint EXAMPLE_INT_FIELD = env->GetStaticIntField(person_class, fieldId2);
  jstring exampleFieldStr = (jstring)EXAMPLE_FIELD;

  const char *exampleFieldCStr = env->GetStringUTFChars(exampleFieldStr, 0);

  printf("EXAMPLE_FIELD=%s, EXAMPLE_INT_FIELD=%d\n", exampleFieldCStr,
         EXAMPLE_INT_FIELD);

  env->ReleaseStringUTFChars(exampleFieldStr, exampleFieldCStr);
}

JNIEXPORT void JNICALL Java_test1java_TestNative_setPerson(
    JNIEnv *env, jclass TestNative, jobject obj, jstring name, jint age,
    jstring exampleStaticValue, jint staticIntValue) {

  jclass person_class = env->FindClass("test1java/Person");

  jfieldID fieldId1 = env->GetStaticFieldID(person_class, "EXAMPLE_FIELD",
                                            "Ljava/lang/String;");
  jfieldID fieldId2 =
      env->GetStaticFieldID(person_class, "EXAMPLE_INT_FIELD", "I");
  env->SetStaticObjectField(person_class, fieldId1, exampleStaticValue);
  env->SetStaticIntField(person_class, fieldId2, staticIntValue);

  jfieldID localFieldId1 =
      env->GetFieldID(person_class, "name", "Ljava/lang/String;");
  jfieldID localFieldId2 = env->GetFieldID(person_class, "age", "I");
  env->SetObjectField(obj, localFieldId1, name);
  env->SetIntField(obj, localFieldId2, age);
}
