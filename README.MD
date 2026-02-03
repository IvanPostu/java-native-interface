# Java Native Interface (JNI)

## Scripts

```sh
nix-shell --run bash

# build gsl (Gnu Scientific Library)

cd external/gsl/

git submodule init
git submodule update

mkdir build
cd build/
cmake .. -G"Unix Makefiles"
make

# build .h files based on java source files
bash "$PROJECT_ROOT/projects/test1cpp/scripts/build-java-native-class.sh" "$PROJECT_ROOT/projects/test1java/src" "$PROJECT_ROOT/projects/test1cpp/src/jni"

# build fortran obj file
gfortran -c "$PROJECT_ROOT/projects/test1cpp/src/zabs.f" -o "$PROJECT_ROOT/projects/test1cpp/fortran_obj/zabs.o"

g++ -fPIC -shared \
  -I"$PROJECT_ROOT/projects/test1cpp/src/jni" \
  -I"$PROJECT_ROOT/external/gsl/build" \
  -L"$PROJECT_ROOT/external/gsl/build" \
  "$PROJECT_ROOT/projects/test1cpp/src/impl.cpp" \
  "$PROJECT_ROOT/projects/test1cpp/fortran_obj/zabs.o" \
  -Wl,-Bstatic -lgsl -Wl,-Bdynamic \
  -o "$PROJECT_ROOT/projects/test1java/src/jni/libimpl.so"

bash "$PROJECT_ROOT/projects/test1cpp/scripts/build-jar.sh" "test1java.TestNative" "$PROJECT_ROOT/projects/test1java/src" "$PROJECT_ROOT/projects/test1java/build"

java -jar "$PROJECT_ROOT/projects/test1java/build/TestNative.jar"

# maven generate project
mvnw archetype:generate
# select: org.apache.maven.archetypes:maven-archetype-quickstart
# groupId: com.iv127.maven.demo2
# artifactId: maven-demo2

mvnw -f "$PROJECT_ROOT/projects/maven-demo2/pom.xml" package

mvnw -f "$PROJECT_ROOT/projects/maven-demo2/pom.xml" exec:java "-Dexec.mainClass=com.iv127.maven.demo2.App"

build_dir="$(mktemp -d)" \
  && javac \
  -d "$build_dir" \
  -h "$PROJECT_ROOT/projects/maven-demo2cpp/src/jni" \
  $(find "$PROJECT_ROOT/projects/maven-demo2/src/main" -type f -name "*.java") \
  && rm -rf "$build_dir"

g++ -fPIC -shared \
  -I"$PROJECT_ROOT/projects/maven-demo2cpp/src/jni" \
  "$PROJECT_ROOT/projects/maven-demo2cpp/src/impl.cpp" \
  -o "$PROJECT_ROOT/projects/maven-demo2/src/main/resources/native/libimpl.so"
```

## Core dump (caused by native code) - stacktrace

- example: `hs_err_pid19942.log`
- <img src="./_forReadme/image copy 3.png" width="500px" />

### Example of problem

```c
static auto Person_class = env->FindClass("test1java/Person");
// ...
jobject person = env->NewObject(Person_class, person_ctor_method_id, name);
// on the second call it crashes because class instance on the first call is associated with the instance

// The rule is to not cache jclass when creating objects
```

## General

- JNI - is a mechanism that provides the bridge to call functions from the native libraries
  - Windows - `.dll`, Linux - `.so`, Mac - `.dylib`

- E.g. of native libraries:
  - GNU Scientific library

## How to use native code in java?

- Define a class with native methods
- Compile with `javac`
- Generate header files with `javah`(java 8 and below) or `javac -h`(java 9 and above)
- Write C/C++ implementation for that header file
- Compile as dynamic library (Windows - `.dll`, Linux - `.so`, Mac - `.dylib`)
- Add the jvm argument `-Djava.library.path=path/to/library` when running java
- Load library:

  ```java
    static {
      System.loadLibrary("test");
    }
  ```

- Naming MacOS, when using `System.loadLibrary("libqwerty.dylib")`
  - drop `lib` prefix
  - drop `.dylib` suffix
  - Correct: `System.loadLibrary("qwerty")`

## Set Up C++ Compiler

- Mac - XCode or command line tools
- Linux - g++ or clang
- Windows - MinGW-64bit

## General types of parameters

- `JNIEnv` pointer to JNIEnv object used to call java classes and their instance variables
- Primitives: `jint`, `jdouble`, `jboolean`, `jchar`, ... -> equivalent C/C++ primitive types `typedef`
- Classes: `jstring`, `jclass` is used for reflection
- reference to **this** - `jobject`

## Did you know?

- What is the downside of using JNI?
  - It ties the program to the specific platform / platform independence is loosed
- What is the general format of header functions for native methods?
  - `Java_<package_name>_<Class_name>_<method_name>`
  - `java.lang.Math.sin` -> `Java_java_lang_Math_sin`
- Priority of java's stdout and native's stdout
  - java has priority
- What does **dereference** means?
  - accessing the value that a pointer points to.
- Signature of the `String` type
  - `Ljava/lang/String;`
- Difference between C/C++ char and java's char
  - C/C++ char takes 1 byte
  - Java char takes 2 bytes - `unsigned short`
- What is string internalization
  - A mechanism that allows java to put strings in a pool of reusable objects, e.g. `new String("abc").intern()`
- If env method returns a pointer to the native heap, it MUST be released manually, e.g.
  - `const char *str1 = (env)->GetStringUTFChars(param1, 0);`
  - `(env)->ReleaseStringUTFChars(param1, str1);`
- Where is the metadata of Java classes stored in memory?
  - In the Metaspace (formerly known as PermGen).
- What is the difference between java heap and native heap in the context of JNI?
  - java heap is managed by GC while native heap is managed manually
- Java requires `field.setAccessible(true)` in order to access a private field
  - JNI uses unprotected approach, it can easily access private fields
- Why on header file generation/name generation based on class name `$` is replaced with 00024?
  - because $'s representation in utf16 is 00024
- Java can generate header files based on .class files
- If we call `GetDoubleArrayElements/GetPrimitiveArrayCritical` we need to call release function in the end of operation  `ReleaseDoubleArrayElements/ReleasePrimitiveArrayCritical`
  - after calling `GetDoubleArrayElements/GetPrimitiveArrayCritical` GC will ignore these objects, it will consider them after calling  `Release...`
  - shouldn't call any arbitrary functions between get and release
- Difference between array manipulations: region, elements and critical
  - region - manually create buffer in native code and call get/set region for that buffer
  - elements - (GC ignores until release) returns pointer of array type, values are changed on release
  - critical - (GC ignores until release) returns opaque pointer, values are changed directly
- Difference between `Eager Link` (RegisterNatives manual, explicit method binding) and `Lazy Link` (Dynamic symbol lookup (default JNI behavior))
  - `Eager Link` - create bindings programmatically inside `JNI_OnLoad`, pros: fail fast, better performance and no need to generate header files for java classes
  - `Lazy Link` - generate header files for java classes and implement those functions, fail late (during call), slow because it does lookup of function symbols
    - JVM loads your native library, JVM looks for exported C symbols whose names follow the JNI naming convention If it finds them → native method is linked, If not → UnsatisfiedLinkError
- JNI allows working with any class inside classpath including libraries
- What is name mangling
  - Transformation of function name by C++ compiler from `int add(int a, int b);` -> `_Z3addii` or

    ```c++
    namespace math {
        class Calc {
        public:
            int add(int, int);  // _ZN4math4Calc3addEii
        };
    }
    ```

    - C language doesn't do mangling because function by default is unique
    - to disable mangling in C++, do: `extern "C" int add(int a, int b);` or `extern "C" {functions}`
- To remember `Get*ArrayCritical` / `Release*ArrayCritical` is for for direct access (when performance is important)
- Local reference inside native code. E.g. array of objects that we iterate and call `getObjectArrayElement`, if it is too big `NewLocalRef/DeleteLocalRef` can help
- Diff between Global Reference and Weak Global reference
  - Weak Global Reference can be garbage collected while simple Global Reference should be manually released
- Diff between NewObject, NewObjectA and NewObjectV
  - NewObject - constructor arguments directly as variadic parameters, i.e. `...`
  - NewObjectA - Arguments are passed as an array of jvalue (a union)  i.e. `const jvalue *args`
  - NewObjectV - Arguments come from a va_list `va_list args`
- In between `GetStringCritical` and `ReleaseStringCritical` MUSTN't call any JNI function
- `GetObjectArrayElement` creates a local reference, in native code all local references are deleted when this native function finishes execution, there is a memory limit for them so `DeleteLocalRef` can help
- `ExceptionOccurred` returns null in native code if there is no exception, it can be used directly in if clause
- `JNIEnv` is local to a thread (internally stores data in thread local), it **MUSTN'T** be used by another thread

## Eclipse setup - cdt (C Development Tool) & (Enterprise Java Plugin)

- Download (<https://www.eclipse.org/downloads/>)
- Unarchive
- Run installer
- Eclipse with C/C++ development tools
- Open eclipse
- Help -> Eclipse Marketplace -> install: Enterprise Java Plugin
  - <img src="./_forReadme/image.png" width="500px" />

### In nix shell

```sh
nix-shell ~/Desktop/docubase-documents/Udemy/java-native-interface-in-depth/app.nix --run bash

which g++

~/IDEAs/eclipse-cdt/eclipse/eclipse
```

### Env variable required by eclipse

See `app.nix`

```sh
export PATH=/nix/store/abcd1234-gcc-13.2.0/bin:$PATH
export CC=/nix/store/abcd1234-gcc-13.2.0/bin/gcc
export CXX=/nix/store/abcd1234-gcc-13.2.0/bin/g++
export C_INCLUDE_PATH=/nix/store/abcd1234-glibc-2.39/include
export CPLUS_INCLUDE_PATH=/nix/store/abcd1234-gcc-13.2.0/include/c++/13.2.0:/nix/store/abcd1234-gcc-13.2.0/include/c++/13.2.0/x86_64-unknown-linux-gnu
```

### Project 1

- `nix-shell ~/Desktop/docubase-documents/Udemy/java-native-interface-in-depth/app.nix --run bash`
- `cd Udemy/java-native-interface-in-depth`

## VSCodium setup

- <img src="./_forReadme/image copy.png" width="500px" />
- add `clangd` language server - part of `clang-tools` package

## IntelliJ

- jclasslib - plugin for showing java bytecode of class
- basically does exactly what `javap` do
- <img src="./_forReadme/image copy 2.png" width="500px" />

## Windows setup

1. Install `MinGW`
1. Set `MINGW_HOME` env. variable to:
1. <img src="./_forReadme/image copy 4.png" width="500px" />
1. 64 bit java can work with only 64 bit .dll

## Decompile.class file using javap cli tool (from jdk)

- `javap -v $PROJECT_ROOT/projects/test1java/out/production/test1java/test1java/Person.class`

see [javap Output](./_forReadme/javap_out.txt)

## Performance advices

- Not cache method, field and class IDs
- Trigger array copies
- Reaching back instead of passing parameters
- Using many local references without informing JVM
- **Very good performance advices can be found in IMB reference**

## References

- <https://github.com/adamheinrich/native-utils/blob/master/src/main/java/cz/adamh/utils/NativeUtils.java>
- <https://www.reddit.com/r/vsCodium/comments/w9kaag/what_is_the_equivalent_of_the_ms_c_extension_with/>
- <https://www.gnu.org/software/gsl/#documentation>
- <https://ftp.gnu.org/gnu/gsl/>
- <https://www.w3schools.com/git/git_submodules.asp>
- <https://www.baeldung.com/maven-archetype>
- <https://maven.apache.org/guides/mini/guide-creating-archetypes.html>
- <https://www.geeksforgeeks.org/java/javap-tool-in-java-with-examples/>
- <https://docs.oracle.com/javase/8/docs/technotes/guides/jni/>
- <https://developer.ibm.com/articles/j-jni/>
