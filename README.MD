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
- Difference between `Eager Link` and `Lazy Link`
  - `Eager Link` - create bindings programmatically inside `JNI_OnLoad`, pros: fail fast, better performance and no need to generate header files for java classes
  - `Lazy Link` - generate header files for java classes and implement those functions, fail late (during call), slow because it does lookup of function symbols
- JNI allows working with any class inside classpath including libraries

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

## References

- <https://github.com/adamheinrich/native-utils/blob/master/src/main/java/cz/adamh/utils/NativeUtils.java>
- <https://www.reddit.com/r/vsCodium/comments/w9kaag/what_is_the_equivalent_of_the_ms_c_extension_with/>
- <https://www.gnu.org/software/gsl/#documentation>
- <https://ftp.gnu.org/gnu/gsl/>
- <https://www.w3schools.com/git/git_submodules.asp>
- <https://www.baeldung.com/maven-archetype>
- <https://maven.apache.org/guides/mini/guide-creating-archetypes.html>
- <https://www.geeksforgeeks.org/java/javap-tool-in-java-with-examples/>

## Decompile.class file using javah cli tool (from jdk)

- `javap -v $PROJECT_ROOT/projects/test1java/out/production/test1java/test1java/Person.class`

```sh
Classfile /home/iv127/Projects/java-native-interface/projects/test1java/out/production/test1java/test1java/Person.class
  Last modified Feb 1, 2026; size 1620 bytes
  SHA-256 checksum 48d49841946ad65d8dfa5ecbc8b45a97031c46863d64e72f9155406c1bd49d49
  Compiled from "Person.java"
public class test1java.Person
  minor version: 0
  major version: 65
  flags: (0x0021) ACC_PUBLIC, ACC_SUPER
  this_class: #8                          // test1java/Person
  super_class: #2                         // java/lang/Object
  interfaces: 0, fields: 4, methods: 6, attributes: 3
Constant pool:
   #1 = Methodref          #2.#3          // java/lang/Object."<init>":()V
   #2 = Class              #4             // java/lang/Object
   #3 = NameAndType        #5:#6          // "<init>":()V
   #4 = Utf8               java/lang/Object
   #5 = Utf8               <init>
   #6 = Utf8               ()V
   #7 = Fieldref           #8.#9          // test1java/Person.age:I
   #8 = Class              #10            // test1java/Person
   #9 = NameAndType        #11:#12        // age:I
  #10 = Utf8               test1java/Person
  #11 = Utf8               age
  #12 = Utf8               I
  #13 = Fieldref           #8.#14         // test1java/Person.name:Ljava/lang/String;
  #14 = NameAndType        #15:#16        // name:Ljava/lang/String;
  #15 = Utf8               name
  #16 = Utf8               Ljava/lang/String;
  #17 = InvokeDynamic      #0:#18         // #0:makeConcatWithConstants:(Ljava/lang/String;I)Ljava/lang/String;
  #18 = NameAndType        #19:#20        // makeConcatWithConstants:(Ljava/lang/String;I)Ljava/lang/String;
  #19 = Utf8               makeConcatWithConstants
  #20 = Utf8               (Ljava/lang/String;I)Ljava/lang/String;
  #21 = Fieldref           #8.#22         // test1java/Person.EXAMPLE_FIELD:Ljava/lang/String;
  #22 = NameAndType        #23:#16        // EXAMPLE_FIELD:Ljava/lang/String;
  #23 = Utf8               EXAMPLE_FIELD
  #24 = String             #25            // name=%s,age=%s,EXAMPLE_FIELD=%s,EXAMPLE_INT_FIELD=%s
  #25 = Utf8               name=%s,age=%s,EXAMPLE_FIELD=%s,EXAMPLE_INT_FIELD=%s
  #26 = Methodref          #27.#28        // java/lang/Integer.valueOf:(I)Ljava/lang/Integer;
  #27 = Class              #29            // java/lang/Integer
  #28 = NameAndType        #30:#31        // valueOf:(I)Ljava/lang/Integer;
  #29 = Utf8               java/lang/Integer
  #30 = Utf8               valueOf
  #31 = Utf8               (I)Ljava/lang/Integer;
  #32 = Fieldref           #8.#33         // test1java/Person.EXAMPLE_INT_FIELD:I
  #33 = NameAndType        #34:#12        // EXAMPLE_INT_FIELD:I
  #34 = Utf8               EXAMPLE_INT_FIELD
  #35 = Methodref          #36.#37        // java/lang/String.format:(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
  #36 = Class              #38            // java/lang/String
  #37 = NameAndType        #39:#40        // format:(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
  #38 = Utf8               java/lang/String
  #39 = Utf8               format
  #40 = Utf8               (Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
  #41 = String             #42            // example
  #42 = Utf8               example
  #43 = Utf8               (Ljava/lang/String;)V
  #44 = Utf8               Code
  #45 = Utf8               LineNumberTable
  #46 = Utf8               LocalVariableTable
  #47 = Utf8               this
  #48 = Utf8               Ltest1java/Person;
  #49 = Utf8               getName
  #50 = Utf8               (I)Ljava/lang/String;
  #51 = Utf8               i
  #52 = Utf8               ()Ljava/lang/String;
  #53 = Utf8               getExampleField
  #54 = Utf8               toString
  #55 = Utf8               <clinit>
  #56 = Utf8               SourceFile
  #57 = Utf8               Person.java
  #58 = Utf8               BootstrapMethods
  #59 = String             #60            // \u0001 \u0001
  #60 = Utf8               \u0001 \u0001
  #61 = MethodHandle       6:#62          // REF_invokeStatic java/lang/invoke/StringConcatFactory.makeConcatWithConstants:(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/invoke/CallSite;
  #62 = Methodref          #63.#64        // java/lang/invoke/StringConcatFactory.makeConcatWithConstants:(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/invoke/CallSite;
  #63 = Class              #65            // java/lang/invoke/StringConcatFactory
  #64 = NameAndType        #19:#66        // makeConcatWithConstants:(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/invoke/CallSite;
  #65 = Utf8               java/lang/invoke/StringConcatFactory
  #66 = Utf8               (Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/invoke/CallSite;
  #67 = Utf8               InnerClasses
  #68 = Class              #69            // java/lang/invoke/MethodHandles$Lookup
  #69 = Utf8               java/lang/invoke/MethodHandles$Lookup
  #70 = Class              #71            // java/lang/invoke/MethodHandles
  #71 = Utf8               java/lang/invoke/MethodHandles
  #72 = Utf8               Lookup
{
  static java.lang.String EXAMPLE_FIELD;
    descriptor: Ljava/lang/String;
    flags: (0x0008) ACC_STATIC

  static int EXAMPLE_INT_FIELD;
    descriptor: I
    flags: (0x0008) ACC_STATIC

  public test1java.Person(java.lang.String);
    descriptor: (Ljava/lang/String;)V
    flags: (0x0001) ACC_PUBLIC
    Code:
      stack=2, locals=2, args_size=2
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: aload_0
         5: bipush        33
         7: putfield      #7                  // Field age:I
        10: aload_0
        11: aload_1
        12: putfield      #13                 // Field name:Ljava/lang/String;
        15: return
      LineNumberTable:
        line 11: 0
        line 8: 4
        line 12: 10
        line 13: 15
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      16     0  this   Ltest1java/Person;
            0      16     1  name   Ljava/lang/String;

  public java.lang.String getName(int);
    descriptor: (I)Ljava/lang/String;
    flags: (0x0001) ACC_PUBLIC
    Code:
      stack=2, locals=2, args_size=2
         0: aload_0
         1: getfield      #13                 // Field name:Ljava/lang/String;
         4: iload_1
         5: invokedynamic #17,  0             // InvokeDynamic #0:makeConcatWithConstants:(Ljava/lang/String;I)Ljava/lang/String;
        10: areturn
      LineNumberTable:
        line 16: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      11     0  this   Ltest1java/Person;
            0      11     1     i   I

  public java.lang.String getName();
    descriptor: ()Ljava/lang/String;
    flags: (0x0001) ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: getfield      #13                 // Field name:Ljava/lang/String;
         4: areturn
      LineNumberTable:
        line 20: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0       5     0  this   Ltest1java/Person;

  public static java.lang.String getExampleField(int);
    descriptor: (I)Ljava/lang/String;
    flags: (0x0009) ACC_PUBLIC, ACC_STATIC
    Code:
      stack=2, locals=1, args_size=1
         0: getstatic     #21                 // Field EXAMPLE_FIELD:Ljava/lang/String;
         3: iload_0
         4: invokedynamic #17,  0             // InvokeDynamic #0:makeConcatWithConstants:(Ljava/lang/String;I)Ljava/lang/String;
         9: areturn
      LineNumberTable:
        line 24: 0
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      10     0     i   I

  public java.lang.String toString();
    descriptor: ()Ljava/lang/String;
    flags: (0x0001) ACC_PUBLIC
    Code:
      stack=5, locals=1, args_size=1
         0: ldc           #24                 // String name=%s,age=%s,EXAMPLE_FIELD=%s,EXAMPLE_INT_FIELD=%s
         2: iconst_4
         3: anewarray     #2                  // class java/lang/Object
         6: dup
         7: iconst_0
         8: aload_0
         9: getfield      #13                 // Field name:Ljava/lang/String;
        12: aastore
        13: dup
        14: iconst_1
        15: aload_0
        16: getfield      #7                  // Field age:I
        19: invokestatic  #26                 // Method java/lang/Integer.valueOf:(I)Ljava/lang/Integer;
        22: aastore
        23: dup
        24: iconst_2
        25: getstatic     #21                 // Field EXAMPLE_FIELD:Ljava/lang/String;
        28: aastore
        29: dup
        30: iconst_3
        31: getstatic     #32                 // Field EXAMPLE_INT_FIELD:I
        34: invokestatic  #26                 // Method java/lang/Integer.valueOf:(I)Ljava/lang/Integer;
        37: aastore
        38: invokestatic  #35                 // Method java/lang/String.format:(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;
        41: areturn
      LineNumberTable:
        line 29: 0
        line 30: 19
        line 29: 38
      LocalVariableTable:
        Start  Length  Slot  Name   Signature
            0      42     0  this   Ltest1java/Person;

  static {};
    descriptor: ()V
    flags: (0x0008) ACC_STATIC
    Code:
      stack=1, locals=0, args_size=0
         0: ldc           #41                 // String example
         2: putstatic     #21                 // Field EXAMPLE_FIELD:Ljava/lang/String;
         5: bipush        22
         7: putstatic     #32                 // Field EXAMPLE_INT_FIELD:I
        10: return
      LineNumberTable:
        line 5: 0
        line 6: 5
}
SourceFile: "Person.java"
BootstrapMethods:
  0: #61 REF_invokeStatic java/lang/invoke/StringConcatFactory.makeConcatWithConstants:(Ljava/lang/invoke/MethodHandles$Lookup;Ljava/lang/String;Ljava/lang/invoke/MethodType;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/invoke/CallSite;
    Method arguments:
      #59 \u0001 \u0001
InnerClasses:
  public static final #72= #68 of #70;    // Lookup=class java/lang/invoke/MethodHandles$Lookup of class java/lang/invoke/MethodHandles
```
