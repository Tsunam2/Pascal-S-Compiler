program procdemo;
var
    g : integer;

procedure bump;
var
    local : integer;
begin
    local := 1;
    g := g + local
end;

begin
    g := 0;
    bump;
    write(g)
end.
