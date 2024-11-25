; test.ll
define dso_local i32 @main() #0 {
entry:
  ret i32 0
}

define dso_local void @hello() #0 {
entry:
  ret void
}
