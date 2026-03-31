program main;
var
  a: array[0..2, 0..2] of integer;
  x: integer;
begin
  a[1, 2] := 7;
  x := a[1, 2];
  write(x);
end.
