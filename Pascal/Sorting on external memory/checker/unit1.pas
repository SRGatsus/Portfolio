unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type picture=record
  Name: string[100];
    Fio: string[100];
    Pol: string[5];
    year  : string[4];
    city : string[100];
    Sport: string[100];
    number :string[11];
  end;

  link = ^picture;

type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TButton;
    Label1: TLabel;
    Label2: TLabel;
    labelResult: TLabel;
    labelPath: TLabel;
    Button2: TButton;
    OpenDialog: TOpenDialog;
    procedure Button1Click(Sender: TObject);
    procedure OpenDialogCanClose(Sender: TObject; var CanClose: Boolean);
    procedure Button2Click(Sender: TObject);
  private
    m_selected,m_result : boolean;

    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation


{$R *.lfm}
    function stringCompare(str1,str2:string):integer;
var i: integer;
len : integer;
begin
  if(length(str1)<length(str2))then
    len:=length(str1)
  else
  if(length(str1) >= length(str2))
  then
    len:=length(str2);

	for i:=1 to len do
	begin
		// ord of empty string = 0
		if( ord(str1[i]) < ord(str2[i])  )
		then begin
			stringCompare:=-1;
			exit;
		end else
		if( ord(str1[i]) > ord(str2[i])  )
		then begin
			stringCompare:=1;
			exit;
		end else
		if( ord(str1[i]) = ord(str2[i]) )
		then begin
			continue;
		end;

	end;

  if(length(str1) > length(str2)) then
  stringCompare:= 1 else
  if(length(str1) < length(str2)) then
  stringCompare:= -1 else
  stringCompare:= 0 ;



end;


    function filmCompare(ptr1,ptr2:pointer):integer;
    var f1,f2 : link;
    begin
    f1:= ptr1;
    f2:= ptr2;
    if( stringCompare(f1^.Name,f2^.Name) = 1 ) then
    filmCompare:=1 else
    if( stringCompare(f1^.Name,f2^.Name) = -1 ) then
    filmCompare:=-1 else
    if( stringCompare(f1^.Fio,f2^.Fio) = 1 ) then
    filmCompare:=1 else
    if( stringCompare(f1^.Fio,f2^.Fio) = -1 ) then
    filmCompare:=-1 else

    if( stringCompare(f1^.year,f2^.year) = 1 ) then
    filmCompare:=1 else
    if( stringCompare(f1^.year,f2^.year) = -1 ) then
    filmCompare:=-1 else


    if( stringCompare(f1^.Pol,f2^.Pol) = 1 ) then
    filmCompare:=1 else
    if( stringCompare(f1^.Pol,f2^.Pol) = -1 ) then
    filmCompare:=-1
    else
    if( stringCompare(f1^.city,f2^.city) = 1 ) then
    filmCompare:=1 else
    if( stringCompare(f1^.city,f2^.city) = -1 ) then
    filmCompare:=-1 else
    if( stringCompare(f1^.Sport,f2^.Sport) = 1 ) then
    filmCompare:=1 else
    if( stringCompare(f1^.Sport,f2^.Sport) = -1 ) then
    filmCompare:=-1 else


    filmCompare := 0;

        end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  m_selected := false;
  openDialog.Execute;

end;

procedure TForm1.OpenDialogCanClose(Sender: TObject;
  var CanClose: Boolean);
begin
  m_selected := true;
    labelPath.Caption := openDialog.FileName
end;

procedure TForm1.Button2Click(Sender: TObject);
var f : file of picture;
ptr1,ptr2 : link;
z:integer;
begin

  if(not m_selected)   then
  begin
  labelResult.Caption := 'Пожалуйста, выберите файл';
  exit;
  end;

  assignFile(f,openDialog.FileName);
  reset(f);

  new(ptr1);
  new(ptr2);
  m_result := true;
   z:=0 ;
  while {( not eof(f) )} z<(1250000-10) do
  begin
    read(f,ptr1^);
    read(f,ptr2^);

    if ( filmCompare(ptr1,ptr2) = 1 )
    then m_result := false;
     z:=z+1;
  end;

  closeFile(f);

  if(m_result) then
  labelResult.Caption := 'Отсортировано' else
  labelResult.Caption := 'Не отсортировано';



end;

end.

end.

