unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils,FileUtil, Forms, Controls, Graphics, Dialogs, ExtCtrls, StdCtrls,game, LCLType;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button3: TButton;
    Button2: TButton;
    Button4: TButton;
    Button5: TButton;
    CheckBox1: TCheckBox;
    CheckBox2: TCheckBox;
    GroupBox1: TGroupBox;
    GroupBox2: TGroupBox;
    Image1: TImage;
    Image2: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label8: TLabel;
    Label9: TLabel;
    Notebook1: TNotebook;
    Page1: TPage;
    Page2: TPage;
    Page3: TPage;
    RadioButton2: TRadioButton;
    RadioButton3: TRadioButton;
    Timer1: TTimer;
    Timer2: TTimer;
    Timer3: TTimer;

    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState);
    procedure FormPaint(Sender: TObject);
    procedure Timer1Timer(Sender: TObject);
    procedure NewGame;
  procedure MoveSnake;
  procedure Game;
   procedure GameOver;
    procedure Pause;
   procedure Timer2Timer(Sender: TObject);
   procedure Timer3Timer(Sender: TObject);
  private

  public

  end;

var
  Form1: TForm1;
 SnakeLen2,SnakeLen, Count : Integer;
  ATails : array[1..1000] of TTail;//тело первой змеи
  ATails2 : array[1..1000] of TTail2;//тело второй змеи
  AWall : array[1..100] of Twall;
  AApple : TApple;
  AHead : THead; //голова
  AHead2 : THead2; // голова второй змеи
  HeadPic,Head2Pic,ApplePic, TailPic,Tail2Pic,WallPic : TBitmap;
  dir,dir2 : String;
  flag,flag3,flag2:boolean;
  timerGame:integer;

implementation

{$R *.lfm}

{ TForm1 }

procedure TForm1.FormPaint(Sender: TObject);
var i : Integer;
  //bm:TBitMap;
  begin
    if (flag=true)  then
begin
{bm:=TBitMap.Create;
bm.Loadfromfile('grass2-min.bmp');
Form1.Canvas.Draw(0,0,bm);
bm.Free; }
     for i := 1 to SnakeLen do
    Canvas.Draw(ATails[i].getX,ATails[i].getY,Atails[i].getBitmap);

  Canvas.Draw(AApple.getX,AApple.getY,AApple.getBitmap);
  Canvas.Draw(AHead.getX,AHead.getY,AHead.getBitmap);
  if Checkbox2.Checked then
  for i:=1 to 30 do
    Canvas.Draw(AWall[i].getX,AWall[i].getY,AWall[i].getBitmap);
    if Checkbox1.Checked then
    begin
    for i := 1 to SnakeLen2 do
    Canvas.Draw(ATails2[i].getX,ATails2[i].getY,Atails2[i].getBitmap);
  Canvas.Draw(AHead2.getX,AHead2.getY,AHead2.getBitmap);
    end;

end;
  end;


procedure TForm1.Timer1Timer(Sender: TObject);
begin
 MoveSnake;
  Game;
  Label8.Caption := '1 player: '+IntToStr(SnakeLen)+'.';
  if CheckBox1.Checked then Label9.Caption := '2 player: '+IntToStr(SnakeLen2)+'.'
  else Label9.Caption :='';
  Refresh;
  flag3:=true;
  flag2:=true;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  image2.Picture.LoadFromFile('grass.bmp');
   image1.Picture.LoadFromFile('grass.bmp');
   Timer1.Enabled := false;
    flag:=false;
    Timer2.Enabled := false;
    Timer3.Enabled := false;
    Timer1.Enabled := false;
end;

procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word; Shift: TShiftState
  );
var z2,z3:char;
  i:integer;
begin
  begin
  if flag3=true  then begin
if (Key = VK_Left) and (dir <> 'Right') Then dir := 'Left';
  if (Key = VK_RIGHT) and (dir <> 'Left') Then dir := 'Right';
  if (Key = VK_DOWN) and (dir <> 'Up') Then dir := 'Down';
  if (Key = VK_UP) and (dir <> 'Down') Then dir := 'Up';
  end;
  //движение второй змеи
   if flag2=true then 
   begin
  if (Key = 65) and (dir2 <> 'Right') Then dir2 := 'Left';
  if (Key = 68) and (dir2 <> 'Left') Then dir2 := 'Right';
  if (Key = 83) and (dir2 <> 'Up') Then dir2 := 'Down';
  if (Key =87) and (dir2 <> 'Down') Then dir2 := 'Up';
  flag2:=false;
  end;
  flag3:=false;
  end;
  if (Key = 27) Then Pause ;
  end;


procedure TForm1.pause;
begin
   Timer2.Enabled := false;
   Timer3.Enabled := false;
   Timer1.Enabled := false;
   Notebook1.Visible:=true;

end;



procedure TForm1.Button2Click(Sender: TObject);
begin
  NoteBook1.PageIndex:=1;
end;

procedure TForm1.Button3Click(Sender: TObject);

begin

 DoubleBuffered := True;
 Count := 29;
  Notebook1.Visible:=false;
 flag:=true;
 Color :=clGreen;
 NewGame;
end;

procedure TForm1.Button4Click(Sender: TObject);
begin
  Close;
end;

procedure TForm1.Button5Click(Sender: TObject);
begin
 NoteBook1.PageIndex:=0;
end;

procedure TForm1.Button6Click(Sender: TObject);
begin
NoteBook1.PageIndex:=0;
end;
 procedure TForm1.NewGame;
 var
 i,z:integer;
begin
  //длина змей
  SnakeLen := 2;
  SnakeLen2 := 2;
  //добавление первая змея
  ApplePic := TBitmap.Create;
  ApplePic.LoadFromFile('red.bmp');
  ApplePic.Transparent := True;
  WallPic := TBitmap.Create;
  WallPic.LoadFromFile('block.bmp');
  WallPic.Transparent := True;
  HeadPic := TBitmap.Create;
  HeadPic.LoadFromFile('yellow.bmp');
  TailPic := TBitmap.Create;
  TailPic.LoadFromFile('green.bmp');
  //вторая змея
   if  checkBox1.Checked  then
   begin
  Head2Pic := TBitmap.Create;
  Head2Pic.LoadFromFile('yellow2.bmp');
  Tail2Pic := TBitmap.Create;
  Tail2Pic.LoadFromFile('green2.bmp');
   end;

  Width := 24 * (Count+1);
  Height := Width;
 //спавен яблока с телом
  Randomize;
  AApple := TApple.Create(random(Count)*24,random(Count)*24,ApplePic);
  AHead := THead.Create(random(Count)*24,random(Count)*24,HeadPic);
  ATails[1] := TTail.Create(AHead.getX,AHead.getY-24,TailPic);
  ATails[2] := TTail.Create(AHead.getX,AHead.getY-48,TailPic);
  //cпавен второго тела
       if  checkBox1.Checked  then
   begin
   AHead2 := THead2.Create(random(Count)*24,random(Count)*24,Head2Pic);
   ATails2[1] := TTail2.Create(AHead2.getX,AHead2.getY-24,Tail2Pic);
   ATails2[2] := TTail2.Create(AHead2.getX,AHead2.getY-48,Tail2Pic);
   end;
  z:=0;
  if Checkbox2.Checked then
  for i:=1 to 30 do
  begin
 //спавен стен с условием что бы яблоко не было в стене
  Awall[i]:=Twall.Create(random(Count)*24,random(Count)*24,WallPic);
  if (AWall[i].getX = AApple.getX) and (AWall[i].gety = AApple.gety) then
     AApple := TApple.Create(random(Count)*24,random(Count)*24,ApplePic);
  end;
  dir := 'Down';
  dir2 := 'Down';

  Timer1.Enabled := True;
  Timer1.Interval := 200;
   if  RadioButton3.Checked  then
   begin
   //таймер игры
  Timer2.Enabled := True;
  Timer2.Interval := 300000+2000;
  timerGame:=300;
  end
   else
   timerGame:=0;
    Timer3.Enabled := True;
    Timer3.Interval :=1000;
   end;

  procedure TForm1.MoveSnake;
var i : Integer;
begin
   //перемешение тела первой змеии
  for i := SnakeLen downto 2 do
    begin
      ATails[i].setX(ATails[i-1].getX);
      ATails[i].setY(ATails[i-1].getY);
    end;

  ATails[1].setX(AHead.getX);
  ATails[1].setY(AHead.getY);

  If dir = 'Down' Then AHead.setY(AHead.getY + 24);
  If dir = 'Up' Then AHead.setY(AHead.getY - 24);
  If dir = 'Left' Then AHead.setX(AHead.getX - 24);
  If dir = 'Right' Then AHead.setX(AHead.getX + 24);
  //вторая змея
   if  checkBox1.Checked  then
  begin
    for i := SnakeLen2 downto 2 do
    begin
      ATails2[i].setX(ATails2[i-1].getX);
      ATails2[i].setY(ATails2[i-1].getY);
    end;
  ATails2[1].setX(AHead2.getX);
  ATails2[1].setY(AHead2.getY);
  If dir2 = 'Down' Then AHead2.setY(AHead2.getY + 24);
  If dir2 = 'Up' Then AHead2.setY(AHead2.getY - 24);
  If dir2 = 'Left' Then AHead2.setX(AHead2.getX - 24);
  If dir2 = 'Right' Then AHead2.setX(AHead2.getX + 24);
   end;
end;

procedure TForm1.Game;
var i,z : Integer;
begin
  //первая змея
  if (AHead.getX = AApple.getX) and (AHead.getY = AApple.getY) Then
     begin
     //сьедание яблока
       Randomize;
       AApple.setX(random(Count)*24);
       AApple.setY(random(Count)*24);

       while  AApple.gety<30 do
       AApple.setY(random(Count)*24);
       if Checkbox2.Checked then
       for i:=1 to 30 do
       begin
       if (AWall[i].getX = AApple.getX) and (AWall[i].gety = AApple.gety) then
       begin
       AApple.setX(random(Count)*24);
       AApple.setY(random(Count)*24);
       while  AApple.gety<20 do
       AApple.setY(random(Count)*24);
       end;
       end;
       SnakeLen := SnakeLen + 1;
       ATails[SnakeLen] := TTail.Create(ATails[SnakeLen-1].getX,ATails[SnakeLen-1].getY,TailPic);
       if Timer1.Interval > 55 Then Timer1.Interval := Timer1.Interval - 5;
     end;
  //вторая змея
  if  checkBox1.Checked  then
    if (AHead2.getX = AApple.getX) and (AHead2.getY = AApple.getY) Then
     begin
     //сьедание яблока
       Randomize;
       AApple.setX(random(Count)*24);
       AApple.setY(random(Count)*24);

       while  AApple.gety<20 do
       AApple.setY(random(Count)*24);
       if Checkbox2.Checked then
       for i:=1 to 30 do
       begin
       if (AWall[i].getX = AApple.getX) and (AWall[i].gety = AApple.gety) then
       begin
       AApple.setX(random(Count)*24);
       AApple.setY(random(Count)*24);
       while  AApple.gety<20 do
       AApple.setY(random(Count)*24);
       end;
       end;
       SnakeLen2 := SnakeLen2 + 1;
       ATails2[SnakeLen] := TTail2.Create(ATails2[SnakeLen-1].getX,ATails2[SnakeLen-1].getY,Tail2Pic);
       if Timer1.Interval > 55 Then Timer1.Interval := Timer1.Interval - 5;
     end;
  //первая змея
  for i := 1 to SnakeLen do
  begin
     if (AHead.getX = ATails[i].getX) and (AHead.getY = ATails[i].getY) Then
        GameOver;
     if Checkbox2.Checked then
     for z:=1 to 30 do
       begin
       if (AWall[z].getX = AHead.getX) and (AWall[z].gety = AHead.gety) then begin  gameover; end;
       end;
  end;
  //вторая змея
  if  checkBox1.Checked  then
    for i := 1 to SnakeLen2 do
  begin
     if (AHead2.getX = ATails2[i].getX) and (AHead2.getY = ATails2[i].getY) Then
        GameOver;
     if Checkbox2.Checked then
     for z:=1 to 30 do
       begin
       if (AWall[z].getX = AHead2.getX) and (AWall[z].gety = AHead2.gety) then begin  gameover; end;
       end;
  end;
  //первая змея
  if AHead.getX < 0 Then GameOver;
  if AHead.getX > (Count) * 24 Then GameOver;
  if AHead.getY < 16 Then GameOver;
  if AHead.getY > (Count) * 24 Then GameOver;
  //вторая змея
  if  checkBox1.Checked  then
  begin
  if AHead2.getX < 0 Then GameOver;
  if AHead2.getX > (Count) * 24 Then GameOver;
  if AHead2.getY < 16 Then GameOver;
  if AHead2.getY > (Count) * 24 Then GameOver;
  end;
end;

procedure TForm1.GameOver;
var i,z,y : Integer;
begin
  Timer1.Enabled := False;
  Timer2.Enabled := False;
  Timer3.Enabled := False;
  for i := 1 to SnakeLen do
    ATails[i].Free;
  z:= SnakeLen;
  SnakeLen := 2;
  if Checkbox1.Checked then
  for i := 1 to SnakeLen2 do
    ATails2[i].Free;
  Y:=SnakeLen2;
  SnakeLen2:= 2;
  if Checkbox1.Checked then
  if MessageDlg('Игра окончена', 'Cчет 1 игрока: '+inttostr(z)+'. Cчет 2 игрока: '+inttostr(Y)+' Попробывать еще раз?',
                mtConfirmation, [mbYes, mbNo], 0) = mrYes then NewGame
  else Pause
  else
  if MessageDlg('Игра окончена', 'Ваш счет: '+inttostr(z)+'. Попробывать еще раз?',
                mtConfirmation, [mbYes, mbNo], 0) = mrYes then NewGame
  else
     Pause;
end;

procedure TForm1.Timer2Timer(Sender: TObject);
begin
 gameover
end;
//таймер игр
procedure TForm1.Timer3Timer(Sender: TObject);
var
  z,z1,z2:integer;
begin
  if  RadioButton3.Checked then
     begin
       z:= timerGame mod 1000;//секунд всего
       z1:= z mod 60;
       z2:=z div 60;
  Label7.Caption:='Время осталось:'+inttostr(z2)+'минут, '+inttostr(z1)+'секунд';
     timerGame:=timerGame-1;

     end;
  if RadioButton2.Checked then
     begin
       z:= timerGame mod 1000;//секунд всего
       z1:= z mod 60;
       z2:=z div 60;
       Label7.Caption:='Время прошло:'+inttostr(z2)+' минут '+inttostr(z1)+'секунд';
     timerGame:=timerGame+1;
     end;
end;


end.

