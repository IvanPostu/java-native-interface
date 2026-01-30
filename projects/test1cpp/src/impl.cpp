#include "./jni/test1java_TestNative.h"
#include "./jni/test1java_example1_Animal.h"
#include "./jni/test1java_example1_Dog.h"

JNIEXPORT jboolean JNICALL Java_test1java_TestNative_isOdd(JNIEnv *env,
                                                           jobject obj,
                                                           jint num) {
  jboolean isOdd = num % 2 == 0;
  return isOdd;
}
