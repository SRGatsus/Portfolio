unit Unit1;
   {$mode objfpc}{$H+}
interface

uses
SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, MaskEdit;


type picture=record
  Name: string[100];
  Fio: string[100];
  Pol: string[5];
  year  : string[4];
  city : string[100];
  Sport: string[100];
  number :string[11];
  end;



type

  { TForm1 }

  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    Label1: TLabel;
    Label2: TLabel;
    Memo1: TMemo;
    OpenDialog: TOpenDialog;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure OpenDialogCanClose(Sender: TObject; var CanClose: boolean);
    function generatePicture: picture;
    procedure initLists();
    procedure removeLists();
  private
    { Private declarations }


  public

    { Public declarations }
  end;

var
  Form1: TForm1;
    m_check : boolean;
    NameList,FioList,countryList,sportList ,PolList: TStringList;
    z:integer;
implementation

{$R *.lfm}
procedure TForm1.Button1Click(Sender: TObject);
begin
  OpenDialog.Execute;
end;


procedure TForm1.Button2Click(Sender: TObject);
var f : file of picture;
i,z: longint;
pic : picture;
begin
  initLists();

  if(not m_check) then
  begin
  Memo1.Lines.Add('Пожалуйста, выберите файл!');
  exit;
  end;

  m_check := false;
  Memo1.Lines.Add('Начало генерации');
  AssignFile(f,openDialog.FileName);
  rewrite(f);
  z:=2500000;
  for i:=0 to z do
  begin
    pic := generatePicture();
    write(f,pic);

  end;

  closeFile(f);

  Memo1.Lines.Add('Генерация завершена');


  removeLists();

end;







procedure TForm1.OpenDialogCanClose(Sender: TObject; var CanClose: boolean);
begin
m_check := true;
  label1.Caption := openDialog.FileName;
end;


function TForm1.generatePicture: picture;
var res: picture;
z:integer;
begin
  z:=random(2);
  res.Pol:=PolList[z];
  case z of
  1: begin  res.Name := NameList[ random(8) ]; end;
  0:begin   res.Name := NameList[ random(8)+9 ];  end;
  end;
  res.year := inttostr(1940+random(80));
  res.Fio :=FioList[ random(FioList.Count-1) ];
  res.city := countryList[ random(CountryList.Count-1)  ];
  res.Sport := SportList[Random(SportList.Count-1)];
  res.number := inttostr(10000000000+Random(89999999999));

  generatePicture := res;
end;




procedure TForm1.initLists();
begin
  NameList := TStringList.Create;
  FioList := TStringList.Create;
  countryList := TStringList.Create;
  SportList := TStringList.Create;
  PolList := TStringList.Create;
  //Пол
  PolList.Add('Women');
   PolList.Add('Men  ');
  //Мужсике Фамилии 0-27
  FioList.Add('Shishkin         ');
  FioList.Add('Petrov-Vodkin    ');
  FioList.Add('Repin            ');
  FioList.Add('Benoit           ');
  FioList.Add('Leonardo da Vinci');
  FioList.Add('Rafael           ');
  FioList.Add('Aivazovsky       ');
  FioList.Add('Kuindzhi         ');
  FioList.Add('Botticelli       ');
  FioList.Add('Rubens           ');
  FioList.Add('Gauguin          ');
  FioList.Add('Velazquez        ');
  FioList.Add('Yuryevich        ');
  FioList.Add('Caravagg         ');
  FioList.Add('Aleksandrovich   ');
  FioList.Add('Levitan          ');
  FioList.Add('Malevich         ');
  FioList.Add('Degas            ');
  FioList.Add('Brullov          ');
  FioList.Add('Goya             ');
  FioList.Add('Delacroix        ');
  FioList.Add('Buonarotti       ');
  FioList.Add('Durer            ');
  FioList.Add('Bilibin          ');
  FioList.Add('Vrubel           ');
  FioList.Add('Kustodiev        ');
  FioList.Add('Rylov            ');
  FioList.Add('Serebryakova     ');


  //Мужские имена 0-8
  NameList.Add('Anatoliy ');
  NameList.Add('Anton    ');
  NameList.Add('Vladimir ');
  NameList.Add('Igor     ');
  NameList.Add('Dmitriy  ');
  NameList.Add('Oleg     ');
  NameList.Add('Sergey   ');
  NameList.Add('Fedor    ');
  NameList.Add('Kirill   ');
  //Женские Имена 9-17
  NameList.Add('Anzhela  ');
  NameList.Add('Galina   ');
  NameList.Add('Inna     ');
  NameList.Add('Kristina ');
  NameList.Add('Oksana   ');
  NameList.Add('Regina   ');
  NameList.Add('Snezhana ');
  NameList.Add('Larisa   ');
  NameList.Add('Kira     ');


  countryList.Add('Kirov           ');
  countryList.Add('Moscow          ');
  countryList.Add('Saint-Petersburg');
  countryList.Add('Yekaterinburg   ');
  countryList.Add('Nizhny Novgorod ');
  countryList.Add('Kazan           ');
  countryList.Add('Paris           ');
  countryList.Add('London          ');
  countryList.Add('Pavlovsk        ');


  sportList.Add('baseball           ');
  sportList.Add('basketball         ');
  sportList.Add('football           ');
  sportList.Add('volleybal          ');
  sportList.Add('cricket            ');
  sportList.Add('dog racing         ');
  sportList.Add('horse racing       ');
  sportList.Add('horseback riding   ');
  sportList.Add('rowing             ');
  sportList.Add('cross country race ');
  sportList.Add('waterskiing        ');
  sportList.Add('alpine skiing      ');
  sportList.Add('ice hockey         ');
  sportList.Add('BASE jumping       ');
  sportList.Add('rock climbing      ');
  sportList.Add('skysurfing         ');
  sportList.Add('bowling            ');
  sportList.Add('draught            ');
  sportList.Add('car racing         ');
  sportList.Add('handball           ');
  sportList.Add('mountaineering     ');
  sportList.Add('weightlifting      ');



end;




procedure TForm1.removeLists;
begin
  NameList.Free;
  FioList.Free;
  countryList.Free;
  sportList.Free;
  PolList.Free;
end;

end.
