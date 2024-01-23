; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --version 2
; RUN: opt -S -passes=inline %s | FileCheck %s
; RUN: opt -S -passes='cgscc(inline)' %s | FileCheck %s
; RUN: opt -S -passes='module-inline' %s | FileCheck %s

declare void @foo()
declare void @bar()

define void @callee(ptr %arg) {
; CHECK-LABEL: define void @callee
; CHECK-SAME: (ptr [[ARG:%.*]]) {
; CHECK-NEXT:    [[CMP:%.*]] = icmp eq ptr [[ARG]], null
; CHECK-NEXT:    br i1 [[CMP]], label [[EXPENSIVE:%.*]], label [[DONE:%.*]]
; CHECK:       expensive:
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    ret void
; CHECK:       done:
; CHECK-NEXT:    call void @bar()
; CHECK-NEXT:    ret void
;
  %cmp = icmp eq ptr %arg, null
  br i1 %cmp, label %expensive, label %done

; This block is designed to be too expensive to inline.  We can only inline
; callee if this block is known to be dead.
expensive:
  call void @foo()
  call void @foo()
  call void @foo()
  call void @foo()
  call void @foo()
  call void @foo()
  call void @foo()
  call void @foo()
  call void @foo()
  call void @foo()
  ret void

done:
  call void @bar()
  ret void
}

; Positive test - arg is known non null
define void @caller(ptr nonnull %arg) {
; CHECK-LABEL: define void @caller
; CHECK-SAME: (ptr nonnull [[ARG:%.*]]) {
; CHECK-NEXT:    call void @bar()
; CHECK-NEXT:    ret void
;
  call void @callee(ptr nonnull %arg)
  ret void
}

; Negative test - arg is not known to be non null
define void @caller2(ptr %arg) {
; CHECK-LABEL: define void @caller2
; CHECK-SAME: (ptr [[ARG:%.*]]) {
; CHECK-NEXT:    call void @callee(ptr [[ARG]])
; CHECK-NEXT:    ret void
;
  call void @callee(ptr %arg)
  ret void
}

define void @caller3(ptr %arg) {
; CHECK-LABEL: define void @caller3
; CHECK-SAME: (ptr [[ARG:%.*]]) {
; CHECK-NEXT:    [[CMP_I:%.*]] = icmp eq ptr [[ARG]], null
; CHECK-NEXT:    br i1 [[CMP_I]], label [[EXPENSIVE_I:%.*]], label [[DONE_I:%.*]]
; CHECK:       expensive.i:
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    call void @foo()
; CHECK-NEXT:    br label [[CALLEE_EXIT:%.*]]
; CHECK:       done.i:
; CHECK-NEXT:    call void @bar()
; CHECK-NEXT:    br label [[CALLEE_EXIT]]
; CHECK:       callee.exit:
; CHECK-NEXT:    ret void
;
  call void @callee(ptr nonnull %arg)
  ret void
}

define void @caller4(ptr dereferenceable(8) %arg) {
; CHECK-LABEL: define void @caller4
; CHECK-SAME: (ptr dereferenceable(8) [[ARG:%.*]]) {
; CHECK-NEXT:    call void @callee(ptr dereferenceable(8) [[ARG]])
; CHECK-NEXT:    ret void
;
  call void @callee(ptr dereferenceable(8) %arg)
  ret void
}

define void @caller5(ptr dereferenceable(8) %arg) {
; CHECK-LABEL: define void @caller5
; CHECK-SAME: (ptr dereferenceable(8) [[ARG:%.*]]) {
; CHECK-NEXT:    call void @callee(ptr [[ARG]])
; CHECK-NEXT:    ret void
;
  call void @callee(ptr %arg)
  ret void
}

define void @caller6(ptr %arg) {
; CHECK-LABEL: define void @caller6
; CHECK-SAME: (ptr [[ARG:%.*]]) {
; CHECK-NEXT:    call void @callee(ptr dereferenceable(8) [[ARG]])
; CHECK-NEXT:    ret void
;
  call void @callee(ptr dereferenceable(8) %arg)
  ret void
}
