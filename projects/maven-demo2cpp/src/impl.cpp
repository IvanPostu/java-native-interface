#include "./jni/com_iv127_maven_demo2_App.h"
#include <cstdio>
#include <cstdlib>

JNIEXPORT void JNICALL Java_com_iv127_maven_demo2_App_printHello(JNIEnv *env,
                                                                 jclass App) {
  printf("Hello\n");
}
