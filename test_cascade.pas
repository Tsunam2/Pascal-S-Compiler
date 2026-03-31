program cascade_test;
begin
  { 未声明变量 x，在之前的编译器中会报 3 次错，现在只会报 1 次！ }
  x := 1;
  x := x + 1;
  write(x);

  { 未声明过程 bad_proc，之前如果被调用多次会反复刷屏，现在也只会报 1 次！ }
  bad_proc();
  bad_proc();
end.