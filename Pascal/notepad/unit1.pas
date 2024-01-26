unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, Menus, StdCtrls,
  PrintersDlgs, Printers,ExtDlgs, ExtCtrls,LazUTF8,LCLProc,Printer4Lazarus;

type

  { TForm1 }

  TForm1 = class(TForm)
    Edit1: TEdit;
    Edit2: TEdit;
    FontDialog1: TFontDialog;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    MainMenu1: TMainMenu;
    Memo1: TMemo;
    MenuItem1: TMenuItem;
    Menu1Soxran: TMenuItem;
    Menu1Sozdat: TMenuItem;
    Menu1Otkr: TMenuItem;
    Menu1SoxranKak: TMenuItem;
    Menu1Exit: TMenuItem;
    Menu2Vurez: TMenuItem;
    Menu2Cop: TMenuItem;
    Menu2Vstav: TMenuItem;
    Menu3_1: TMenuItem;
    Menu3_2: TMenuItem;
    Menu1pejat: TMenuItem;
    MenuPravka: TMenuItem;
    MenuFormat: TMenuItem;
    OpenDialog1: TOpenDialog;
    PrintDialog1: TPrintDialog;
    SaveDialog1: TSaveDialog;
    procedure FormActivate(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: boolean);
    procedure FormCreate(Sender: TObject);
    procedure Label1Click(Sender: TObject);
    procedure Label3Click(Sender: TObject);
    procedure Label4Click(Sender: TObject);
    procedure Memo1Change(Sender: TObject);
    procedure Menu1ExitClick(Sender: TObject);
    procedure Menu1OtkrClick(Sender: TObject);
    procedure Menu1pejatClick(Sender: TObject);
    procedure Menu1SoxranClick(Sender: TObject);
    procedure Menu1SoxranKakClick(Sender: TObject);
    procedure Menu1SozdatClick(Sender: TObject);
    procedure Menu2CopClick(Sender: TObject);
    procedure Menu2VstavClick(Sender: TObject);
    procedure Menu2VurezClick(Sender: TObject);
    procedure Menu3_1Click(Sender: TObject);
    procedure Menu3_2Click(Sender: TObject);
    procedure MenuItem1Click(Sender: TObject);
    procedure Panel1Click(Sender: TObject);
  private

  public

  end;

var
  Form1: TForm1;
  FileWork:string;
  CH:boolean;

implementation

{$R *.lfm}
Procedure SaveAs;
Begin
 if Form1.SaveDialog1.Execute then
   begin
     Form1.Memo1.Lines.SaveToFile(Form1.SaveDialog1.FileName);
     FileWork:=Form1.SaveDialog1.FileName;
      ShowMessage(FileWork);
   end;
end;

{ TForm1 }

procedure TForm1.MenuItem1Click(Sender: TObject);
begin

end;

procedure TForm1.Panel1Click(Sender: TObject);
begin

end;

procedure TForm1.Menu3_1Click(Sender: TObject);
begin

  if Memo1.WordWrap then
    begin
      Memo1.WordWrap:=false;
      Memo1.ScrollBars:=ssBoth;
      Menu3_1.Checked:=False;
    end
  else
  begin
      Memo1.WordWrap:=true;
      Memo1.ScrollBars:=ssVertical;
      Menu3_1.Checked:=true;
  end;
end;

procedure TForm1.Menu3_2Click(Sender: TObject);
begin
   if FontDialog1.Execute then Memo1.Font:=FontDialog1.Font;
end;

procedure TForm1.Menu1ExitClick(Sender: TObject);
begin
  if CH=False then
   if MessageDlg('Подтверждение', 'Ой! Вы не сохранилифайл,файл будет утерен.Сохранить?',
                mtConfirmation, [mbYes, mbNo], 0) = mrYes then begin saveas; close end else  close
  else close;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin

end;

procedure TForm1.FormCloseQuery(Sender: TObject; var CanClose: boolean);
begin
  if CH=False then
   if MessageDlg('Подтверждение', 'Ой! Вы не сохранилифайл,файл будет утерен.Сохранить?',
                mtConfirmation, [mbYes, mbNo], 0) = mrYes then begin saveas; close end;

end;

procedure TForm1.FormActivate(Sender: TObject);
begin
   CH:=true;
end;


procedure TForm1.Label1Click(Sender: TObject);
begin

end;

procedure TForm1.Label3Click(Sender: TObject);
begin

end;

procedure TForm1.Label4Click(Sender: TObject);
begin
end;

procedure TForm1.Memo1Change(Sender: TObject);
var n,L:integer;
  stroka,S:string;
  P: PChar;
begin
  n:=Memo1.Lines.Count;
  str(n,stroka);
  Edit1.text:=stroka;
S := Memo1.Lines.Text;
L := Length(S);
if L = 0 then Exit;
SetLength(S, L + 1);
S[L + 1] := #0;
P := @S[1];
L := 0;
while P^ <> #0 do
begin
if (P^ <> #32) and (P^ <> #13) and (P^ <> #10) then L := L + 1;
P := P + UTF8CharacterLength(P);
end;
Edit2.Text := IntToStr(L);
CH:=false;
end;

procedure TForm1.Menu1OtkrClick(Sender: TObject);
begin
  if CH=false then if MessageDlg('Подтверждение', 'Ой! Вы не сохранилифайл,файл будет утерен.Сохранить?',
                mtConfirmation, [mbYes, mbNo], 0)=mrYes then SaveAs;
  if OpenDialog1.Execute then
  begin
    Memo1.Lines.LoadFromFile(OpenDialog1.FileName);
    FileWork:=OpenDialog1.FileName;
    CH:=true;
    end;
  end;

procedure TForm1.Menu1pejatClick(Sender: TObject);
var lh,y,i:integer;
begin
   if PrintDialog1.Execute then begin
  Printer.BeginDoc;
  Printer.Canvas.Font.Name:='Courier New';
  Printer.Canvas.Font.Size:=14;
  Printer.Canvas.Font.Color:=clBlack;
  lh:=round(1.2*abs(printer.Canvas.TextHeight('I')));
  for i:=0 to Memo1.Lines.Count-1 do begin
    if (i>0) and (i mod 40=0) then Printer.NewPage;
    Printer.Canvas.TextOut(10,3*lh+lh*(i mod 40),Memo1.Lines[i]);
  end;
  Printer.EndDoc;
  ShowMessage('Печать завершена');
end;
   end;

procedure TForm1.Menu1SoxranClick(Sender: TObject);
begin
  if FileWork='' then SaveAs else Memo1.Lines.SaveToFile(Filework);
  CH:=true;
end;

procedure TForm1.Menu1SoxranKakClick(Sender: TObject);
begin
  SaveAs;
  CH:=true;
end;

procedure TForm1.Menu1SozdatClick(Sender: TObject);
begin
   if CH=false then if MessageDlg('Подтверждение', 'Ой! Вы не сохранилифайл,файл будет утерен.Сохранить?',
                mtConfirmation, [mbYes, mbNo], 0)=mrYes then SaveAs;
   Memo1.clear;
   FileWork:=' ';
   CH:=true;
end;

procedure TForm1.Menu2CopClick(Sender: TObject);
begin
  Memo1.CopyToClipboard;
end;



procedure TForm1.Menu2VstavClick(Sender: TObject);
begin
  Memo1.PasteFromClipboard;
end;

procedure TForm1.Menu2VurezClick(Sender: TObject);
begin
   Memo1.CutToClipboard;
end;

end.
