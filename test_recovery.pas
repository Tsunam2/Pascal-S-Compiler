program recoverytest;
const
  pi = 3;
  bad_const @@@ ;  { 错误1：乱码常量声明 }
  e = 2;
var
  a: integer;
  bad_var $$$ ;    { 错误2：乱码变量声明 }
  b: integer;
begin
  a := 1;
  a := +++ 2;      { 错误3：非法的表达式 }
  b := 2;
  write(b)
end.