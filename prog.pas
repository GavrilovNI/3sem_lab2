program example;
const 
  d = 2.7;
Var i1, i2: integer;
  d1, d2: double;
  s1: string;
  b1: boolean;
const
  i = 13;
  b = false;
  s = 'Ok';
begin
  readln(i1, d1);
  i2:= i1 mod 2 + 2;
  if i1 div i2 > 4
  then begin
	write('in if');
	if b1 = true or not b = true
	then
		write('in 2if');
  end
  else
    begin
	write('out if');
    b1:= true;
    i2:= i;
    end;
  writeln('');
  writeln(s1);
  d1:= (d + i2 + i1 - d1) / 2 *d2;
  if (d1 > d2)
  then
    write('end.');
	
  write('end.2');
end.