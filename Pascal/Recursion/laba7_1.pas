unit Laba7_1;  
    
interface
uses wincrt,graph;  
var Gd,Gm:smallint;
procedure graph ;  
implementation
    
procedure graph ;
var Readk:char;
X1,A,R,Y1:real;
K:integer;

 procedure Draw(x, y, l, u : Real; t : Integer);
procedure Draw2(Var x, y: Real; l, u : Real; t : Integer);
begin
Draw(x, y, l, u, t);
x := x + l*cos(u);
y := y - l*sin(u);
end;
begin
if t > 0 then
begin
l := l/3;
Draw2(x, y, l, u, t-1);
Draw2(x, y, l, u+pi/3, t-1);
Draw2(x, y, l, u-pi/3, t-1);
Draw2(x, y, l, u, t-1);
end
else
Line(Round(x), Round(y), Round(x+cos(u)*l), Round(y-sin(u)*l))
end;

begin
 Gd := Detect;
 InitGraph(Gd, Gm, '');
 X1:=800;
 Y1:=600;
 A:=0;
 R:=200;
 K:=10;//глубина прорисовки
    repeat
   OutTextXY(0, 10, 'to increase/ decrease the depth of field- +/-');
  OutTextXY(0, 20, 'move up/down / left/right-arrows');
  OutTextXY(0, 30, 'zoom in / out - insert/delete');
Draw(10, 354, 400, pi/3, 4);
Draw(410, 354, 400, pi, 4);
Draw(210, 8, 400, -pi/3, 4);
    Readk:=wincrt.readkey;
    IF Readk=#0 then Readk:=wincrt.readkey;
    case Readk of
        #43:begin
            k:=K+1;
            if k>15 then k:=15;
            cleardevice;
            end;//Прибать глубину
        #45:begin
            k:=K-1;
            if k<0 then k:=0;
            cleardevice;
            end;//убавить глубину
        #72:begin
            Y1:=Y1-25;
            cleardevice;
            end;//вверх
        #80:begin
            Y1:=Y1+25;
            cleardevice;
            end;//вниз
        #77:begin
            X1:=X1+25;
            cleardevice;
            end;//вправо
        #75:begin
            X1:=X1-25;
            cleardevice;
            end;//влево
        #82:begin
            R:=R+25;
            if R>650 then R:=650;
            cleardevice;
            end;//увеличить масштаб
        #83:begin
            R:=R-25;
            if R<25 then R:=25;
            cleardevice;
            end;//уменьшить масштаб
    end;
    until Readk=#13;
end;
    
end.