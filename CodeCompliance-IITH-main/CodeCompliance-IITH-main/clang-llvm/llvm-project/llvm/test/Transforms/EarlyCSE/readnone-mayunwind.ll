; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt -S -passes=early-cse -earlycse-debug-hash < %s | FileCheck %s

declare void @readnone_may_unwind() readnone

define void @f(ptr %ptr) {
; CHECK-LABEL: @f(
; CHECK-NEXT:    store i32 100, ptr [[PTR:%.*]], align 4
; CHECK-NEXT:    call void @readnone_may_unwind()
; CHECK-NEXT:    store i32 200, ptr [[PTR]], align 4
; CHECK-NEXT:    ret void
;

  store i32 100, ptr %ptr
  call void @readnone_may_unwind()
  store i32 200, ptr %ptr
  ret void
}