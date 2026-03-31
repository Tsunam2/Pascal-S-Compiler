program main;
var
  a: integer;
  b: integer;

function dec1(p: integer): integer;
begin
  p := p - 1;
  dec1 := p;
end;

begin
  a := 10;
  b := dec1(a);
  write(b);
end.
