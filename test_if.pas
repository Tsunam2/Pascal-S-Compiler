program branchdemo;
const
    threshold = 10;
var
    x : integer;
begin
    read(x);
    if x > threshold then
    begin
        write(x)
    end
    else
    begin
        x := threshold;
        write(x)
    end
end.
