program main;
var
  a: integer;

procedure inc1(var x: integer);
begin
  x := x + 1;
end;

begin
  a := 10;
  inc1(a + 1);
end.
