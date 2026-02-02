#include "./jni/com_iv127_maven_demo2_App.h"
#include "./jni/com_iv127_maven_demo2_misc_Unsafe.h"
#include <cstdio>
#include <cstdlib>

JNIEXPORT jlong JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_allocateMemory(
    JNIEnv *env, jclass Unsafe_class, jlong bytes) {
  char *pointer = (char *)malloc(bytes * sizeof(char));
  return (jlong)pointer;
}

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_freeMemory(
    JNIEnv *env, jclass Unsafe_class, jlong pointer) {
  char *_pointer = (char *)pointer;
  free(_pointer);
}

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_putInt(
    JNIEnv *env, jobject, jlong address, jint x) {
  jint *pointer = (jint *)address;
  *pointer = x;
}

JNIEXPORT jint JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_getInt(
    JNIEnv *env, jobject, jlong address) {
  jint *pointer = (jint *)address;
  return *pointer;
}

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_putDouble(
    JNIEnv *env, jobject, jlong address, jdouble x) {
  jdouble *pointer = (jdouble *)address;
  *pointer = x;
}

JNIEXPORT jdouble JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_getDouble(
    JNIEnv *env, jobject, jlong address) {
  jdouble *pointer = (jdouble *)address;
  return *pointer;
}

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_putLong(
    JNIEnv *env, jobject, jlong address, jlong x) {
  jlong *pointer = (jlong *)address;
  *pointer = x;
}

JNIEXPORT jlong JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_getLong(
    JNIEnv *env, jobject, jlong address) {
  jlong *pointer = (jlong *)address;
  return *pointer;
}

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_putByte(
    JNIEnv *env, jobject, jlong address, jbyte x) {
  jbyte *pointer = (jbyte *)address;
  *pointer = x;
}

JNIEXPORT jbyte JNICALL Java_com_iv127_maven_demo2_misc_Unsafe_getByte(
    JNIEnv *env, jobject, jlong address) {
  jbyte *pointer = (jbyte *)address;
  return *pointer;
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

#ifndef PERSON_H_
#define PERSON_H_

#include <iostream>

class Person {
private:
  std::string name;
  int age;

public:
  Person(std::string name, int age);
  virtual ~Person() noexcept;

  std::string &getName();
  int getAge();
  void sayHi();
};

#endif

Person::Person(std::string name, int age) : name(name), age(age) {
  std::cout << "Person C++ instance created, name=" << name << " age=" << age
            << std::endl;
}

Person::~Person() noexcept {
  std::cout << "Person C++ instance destroyed, name=" << name << " age=" << age
            << std::endl;
}

std::string &Person::getName() { return this->name; }

int Person::getAge() { return this->age; }

void Person::sayHi() {
  std::cout << "C++ sayHi method call: name=" << name << " age=" << age
            << std::endl;
}

jlong person_allocate0(JNIEnv *env, jclass Person_class, jstring name,
                       jint age) {
  const char *name_chars = env->GetStringUTFChars(name, 0);
  Person *person = new Person(name_chars, age);
  env->ReleaseStringUTFChars(name, name_chars);
  return (jlong)person;
}

void person_free0(JNIEnv *env, jclass Person_class, jlong pointer) {
  Person *person = (Person *)pointer;
  delete person;
}

jstring person_get_name0(JNIEnv *env, jclass Person_class, jlong pointer) {
  Person *person = (Person *)pointer;
  return env->NewStringUTF(person->getName().c_str());
}

jint person_get_age0(JNIEnv *env, jclass Person_class, jlong pointer) {
  Person *person = (Person *)pointer;
  return person->getAge();
}

void person_say_hi0(JNIEnv *env, jclass Person_class, jlong pointer) {
  Person *person = (Person *)pointer;
  person->sayHi();
}

static JNINativeMethod methods[] = {
    {(char *)"tripleAge", (char *)"(I)I", (void *)tripleAge}};

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env;
  if (vm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
    return JNI_ERR;
  }
  {
    jclass app_class = env->FindClass("com/iv127/maven/demo2/App");
    if (app_class == NULL) {
      return JNI_ERR;
    }

    jint methods_len = sizeof(methods) / sizeof(methods[0]);
    if (env->RegisterNatives(app_class, methods, methods_len) < 0) {
      return JNI_ERR;
    }
  }

  jclass person_class = env->FindClass("com/iv127/maven/demo2/Person");
  {
    JNINativeMethod method;
    method.name = (char *)"allocate0";
    method.signature = (char *)"(Ljava/lang/String;I)J";
    method.fnPtr = (void *)person_allocate0;

    if (env->RegisterNatives(person_class, &method, 1) < 0) {
      return JNI_ERR;
    }
  }
  {
    JNINativeMethod method;
    method.name = (char *)"free0";
    method.signature = (char *)"(J)V";
    method.fnPtr = (void *)person_free0;

    if (env->RegisterNatives(person_class, &method, 1) < 0) {
      return JNI_ERR;
    }
  }
  {
    JNINativeMethod method;
    method.name = (char *)"getName0";
    method.signature = (char *)"(J)Ljava/lang/String;";
    method.fnPtr = (void *)person_get_name0;

    if (env->RegisterNatives(person_class, &method, 1) < 0) {
      return JNI_ERR;
    }
  }
  {
    JNINativeMethod method;
    method.name = (char *)"getAge0";
    method.signature = (char *)"(J)I";
    method.fnPtr = (void *)person_get_age0;

    if (env->RegisterNatives(person_class, &method, 1) < 0) {
      return JNI_ERR;
    }
  }
  {
    JNINativeMethod method;
    method.name = (char *)"sayHi0";
    method.signature = (char *)"(J)V";
    method.fnPtr = (void *)person_say_hi0;

    if (env->RegisterNatives(person_class, &method, 1) < 0) {
      return JNI_ERR;
    }
  }

  return JNI_VERSION_1_6;
}
