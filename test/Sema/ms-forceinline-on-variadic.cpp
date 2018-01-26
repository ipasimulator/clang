// RUN: %clang_cc1 -emit-llvm -o - -triple i686-windows -verify -fms-extensions %s \
// RUN:            | FileCheck %s
//
// (instruction scheduling crash test)
// RUN: %clang_cc1 -S -o - -triple i686-windows -fms-extensions %s

inline __attribute__((always_inline)) void attribute_variadic(int f, ...) { } // expected-warning {{inlining attribute 'always_inline' ignored on variadic function}}

__forceinline inline void msvc_variadic(int f, ...) { // expected-warning {{inlining attribute '__forceinline' ignored on variadic function}}
// CHECK-DAG: define {{.*}} void [[MSVC_VARIADIC:@".*msvc_variadic.*"]](
    __builtin_va_list ap;
    __builtin_va_start(ap, f);
    __builtin_va_end(ap);
}

struct a {
    // members are, by default, thiscall; enforce it for the purposes of the test
    void __thiscall dispatcher();
};

void __thiscall a::dispatcher() {
    msvc_variadic(1, 2, 3);
// CHECK-DAG: define dso_local x86_thiscallcc void @"{{.*dispatcher.*}}"(
// CHECK-DAG: call void {{.*}} [[MSVC_VARIADIC]]
}

void t() {
    a{}.dispatcher();
}
