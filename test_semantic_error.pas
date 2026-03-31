program semantic_test;
var
  i, j: integer;
  r: real;
  b: boolean;

procedure test_proc(var x: integer; y: real);
begin
  x := x + 1;
end;

begin
  { 错误1：未声明的变量直接使用 }
  undeclared_var := 10;

  { 错误2：类型不匹配（把实数赋给整数） }
  i := 3.14;

  { 错误3：非法运算（对浮点数进行 mod 运算） }
  j := r mod 2;

  { 错误4：参数类型不匹配（给 var 参数传常量/实数） }
  test_proc(100, 2.5);
  
  { 错误5：参数数量不匹配 }
  test_proc(i);
end.