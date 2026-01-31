#include "./jni/test1java_AiryFunction.h"
#include "./jni/test1java_TestNative.h"
#include "./jni/test1java_example1_Animal.h"
#include "./jni/test1java_example1_Dog.h"
#include <cstdio>
#include <cstdlib>
#include <gsl/gsl_mode.h>
#include <gsl/gsl_sf_airy.h>

static jfieldID PERSON_INSTANCE_AGE_FIELD_ID = NULL;

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

  // method caching gives 4x performance
  static jfieldID fieldId1 = env->GetStaticFieldID(
      person_class, "EXAMPLE_FIELD", "Ljava/lang/String;");
  jfieldID fieldId2 =
      env->GetStaticFieldID(person_class, "EXAMPLE_INT_FIELD", "I");
  env->SetStaticObjectField(person_class, fieldId1, exampleStaticValue);
  env->SetStaticIntField(person_class, fieldId2, staticIntValue);

  // method caching gives 4x performance
  static jfieldID localFieldId1 =
      env->GetFieldID(person_class, "name", "Ljava/lang/String;");
  jfieldID localFieldId2 = PERSON_INSTANCE_AGE_FIELD_ID != NULL
                               ? PERSON_INSTANCE_AGE_FIELD_ID
                               : env->GetFieldID(person_class, "age", "I");
  env->SetObjectField(obj, localFieldId1, name);
  env->SetIntField(obj, localFieldId2, age);
}

JNIEXPORT void JNICALL Java_test1java_TestNative_init(JNIEnv *env,
                                                      jclass TestNative) {
  jclass person_class = env->FindClass("test1java/Person");
  PERSON_INSTANCE_AGE_FIELD_ID = env->GetFieldID(person_class, "age", "I");
}

JNIEXPORT void JNICALL Java_test1java_TestNative_callPersonMethods(
    JNIEnv *env, jclass TestNative, jobject person) {

  jclass person_class = env->GetObjectClass(person);

  jmethodID getNameWithIntParamMethodId =
      env->GetMethodID(person_class, "getName", "(I)Ljava/lang/String;");
  jstring m1result =
      (jstring)(env->CallObjectMethod(person, getNameWithIntParamMethodId, 99));
  const char *m1resultCStr = env->GetStringUTFChars(m1result, 0);
  printf("m1result=%s\n", m1resultCStr);
  env->ReleaseStringUTFChars(m1result, m1resultCStr);

  jmethodID getExampleFieldWithIntParamMethodId = env->GetStaticMethodID(
      person_class, "getExampleField", "(I)Ljava/lang/String;");
  jstring m2result = (jstring)(env->CallStaticObjectMethod(
      person_class, getExampleFieldWithIntParamMethodId, 99));
  const char *m2resultCStr = env->GetStringUTFChars(m2result, 0);
  printf("m2result=%s\n", m2resultCStr);
  env->ReleaseStringUTFChars(m2result, m2resultCStr);
}

JNIEXPORT jlong JNICALL
Java_test1java_TestNative_freeMemory(JNIEnv *env, jclass TestNative) {
  jclass runtime_class = env->FindClass("java/lang/Runtime");
  jmethodID getRuntimeMethodID = env->GetStaticMethodID(
      runtime_class, "getRuntime", "()Ljava/lang/Runtime;");
  jobject runtime_object =
      env->CallStaticObjectMethod(runtime_class, getRuntimeMethodID);

  jmethodID freeMethodID = env->GetMethodID(runtime_class, "freeMemory", "()J");
  jlong value = env->CallLongMethod(runtime_object, freeMethodID);

  return value;
}

JNIEXPORT jobject JNICALL Java_test1java_TestNative_createPerson(
    JNIEnv *env, jclass TestNative, jstring name, jint age) {
  jclass person_class = env->FindClass("test1java/Person");
  jmethodID person_ctor_method_id =
      env->GetMethodID(person_class, "<init>", "(Ljava/lang/String;)V");

  jobject person = env->NewObject(person_class, person_ctor_method_id, name);

  jfieldID localFieldId2 = env->GetFieldID(person_class, "age", "I");
  env->SetIntField(person, localFieldId2, age);

  return person;
}

JNIEXPORT jdouble JNICALL Java_test1java_AiryFunction_ai__D(JNIEnv *env,
                                                            jclass AiryFunction,
                                                            jdouble x) {
  return gsl_sf_airy_Ai(x, GSL_PREC_DOUBLE);
}

JNIEXPORT jdouble JNICALL Java_test1java_AiryFunction_bi__D(JNIEnv *env,
                                                            jclass AiryFunction,
                                                            jdouble x) {
  return gsl_sf_airy_Bi(x, GSL_PREC_DOUBLE);
}

JNIEXPORT jdouble JNICALL Java_test1java_AiryFunction_ai__Ljava_lang_Double_2(
    JNIEnv *env, jclass AiryFunction, jobject x) {

  jclass jDoubleClass = env->FindClass("java/lang/Double");
  jmethodID getDoubleMedhodId =
      env->GetMethodID(jDoubleClass, "doubleValue", "()D");
  jdouble doubleValue = env->CallDoubleMethod(x, getDoubleMedhodId);
  return gsl_sf_airy_Ai(doubleValue, GSL_PREC_DOUBLE);
}

JNIEXPORT jdouble JNICALL Java_test1java_AiryFunction_bi__Ljava_lang_Double_2(
    JNIEnv *env, jclass AiryFunction, jobject x) {
  jclass jDoubleClass = env->GetObjectClass(x);
  jmethodID getDoubleMedhodId =
      env->GetMethodID(jDoubleClass, "doubleValue", "()D");
  jdouble doubleValue = env->CallDoubleMethod(x, getDoubleMedhodId);
  return gsl_sf_airy_Bi(doubleValue, GSL_PREC_DOUBLE);
}

JNIEXPORT jdoubleArray JNICALL
Java_test1java_AiryFunction_ai__Ljava_util_List_2(JNIEnv *env,
                                                  jclass AiryFunction,
                                                  jobject list) {

  static jclass list_class = env->FindClass("java/util/List");
  static jmethodID get_id =
      env->GetMethodID(list_class, "get", "(I)Ljava/lang/Object;");
  static jmethodID size_id = env->GetMethodID(list_class, "size", "()I");
  static jclass jDoubleClass = env->FindClass("java/lang/Double");
  static jmethodID getDoubleMedhodId =
      env->GetMethodID(jDoubleClass, "doubleValue", "()D");

  jint len = env->CallIntMethod(list, size_id);

  jdouble *arr = (jdouble *)malloc(sizeof(jdouble) * len);
  for (int i = 0; i < len; i++) {
    jobject doubleBoxed = env->CallObjectMethod(list, get_id, (jint)i);
    jdouble doubleValue = env->CallDoubleMethod(doubleBoxed, getDoubleMedhodId);
    jdouble calculated_value = gsl_sf_airy_Ai(doubleValue, GSL_PREC_DOUBLE);
    *(arr + i) = calculated_value;
  }

  jdoubleArray jarray = env->NewDoubleArray(len);
  env->SetDoubleArrayRegion(jarray, 0, len, arr);
  free(arr);
  return jarray;
}
