program funcdemo;
var
    result : integer;

function addone : integer;
begin
    addone := 1 + 2
end;

begin
    result := addone + 3;
    write(result)
end.
