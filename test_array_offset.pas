program main;
var
  arr: array[1..3] of integer;
  x: integer;
begin
  arr[1] := 4;
  arr[3] := 8;
  x := arr[1] + arr[3];
  write(x);
end.
