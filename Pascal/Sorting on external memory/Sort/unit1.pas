unit Unit1;

{$mode objfpc}{$H+}

interface

uses
   SysUtils, Variants, Classes, Graphics, Controls, Forms,
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
   type link = ^picture;
   cmpFunc = function (f1,f2:link):boolean;

   pictureFile = file of picture;
   pictureFileLink = ^pictureFile;

  { TForm1 }

  TForm1 = class(TForm)
    Label1: TLabel;
    OpenDialog1: TOpenDialog;
    OpenDialog2: TOpenDialog;
    srcBtn: TButton;
    Label2: TLabel;
    outBtn: TButton;
    srcFile: TOpenDialog;
    outFile: TOpenDialog;
    sortBtn: TButton;
    Memo1: TMemo;
    procedure srcBtnClick(Sender: TObject);
    procedure outBtnClick(Sender: TObject);
    procedure srcFileCanClose(Sender: TObject; var CanClose: Boolean);
    procedure outFileCanClose(Sender: TObject; var CanClose: Boolean);
    procedure sortBtnClick(Sender: TObject);
     procedure init();
   // function filmCompare(ptr1,ptr2:pointer):integer;
    function  getFileSize():integer;
    procedure readPart();
    procedure freeMemory();
    procedure saveArrayToFile(fileName:string);



    procedure filesArrayInit();
    procedure allocateMergeMemory();
    procedure freeMergeMemory();
    procedure updateFileData(index : integer );
    function  getMinElementIndex():integer;

  private


  public
       m_film : TList;
    m_fileArray : array of pictureFileLink;
    m_fileData  : array of link;

    m_arraySize        : integer;
    m_pictureReaded      : integer;
    m_filesCount       : integer;
    m_onePercent       : integer;
    m_openedFilesCount : integer;

    m_srcFile : file of picture;
    m_outFile : file of picture;

    m_filesDropped : integer;

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
if( ord(str1[i]) < ord(str2[i]) )
then begin
stringCompare:=-1;
exit;
end else
if( ord(str1[i]) > ord(str2[i]) )
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
//opening a file with records
procedure TForm1.srcBtnClick(Sender: TObject);
begin
  srcFile.Execute;
end;
//opening a file for output
procedure TForm1.outBtnClick(Sender: TObject);
begin
  outFile.Execute;
end;
//closing the dirictory
procedure TForm1.srcFileCanClose(Sender: TObject; var CanClose: Boolean);
begin
  Label1.Caption := srcFile.FileName;
end;
//closing the dirictory
procedure TForm1.outFileCanClose(Sender: TObject; var CanClose: Boolean);
begin
  Label2.Caption :=outFile.FileName;
end;
// main sort function
procedure TForm1.sortBtnClick(Sender: TObject);
var filesCount,i,index : integer   ;

begin
  init;

  // get file info,size of part and other...
  Memo1.Lines.Clear;
  Memo1.Lines.Add('Начало сортировки');

  m_arraySize := getFileSize;
  Memo1.Lines.Add('Размер файла :' + IntToStr(m_arraySize ));

  m_arraySize := (m_arraySize div 100 )*m_onePercent ;

  m_arraySize := m_arraySize div sizeof(picture);
  Memo1.Lines.Add('Записей в файле: ' + IntToStr(m_arraySize));



  // opening source file
  assignFile(m_srcFile,srcFile.FileName);
  reset(m_srcFile);

  // create list

  m_film := TList.Create;


  m_filesCount := 100 div m_onePercent;
  if(100 mod m_onePercent <> 0 ) then inc(m_filesCount);

  // divide file on chunks, all chunks reading,sorting and writing to temp file
  for i := 1 to m_filesCount do
    begin

      Memo1.Lines.Add('Чтение ' + IntToStr(i) + ' части');
      readPart();

      Memo1.Lines.Add('Сортировка  ' + IntToStr(i) + ' части');


      m_film.Sort(@filmCompare);



      Memo1.Lines.Add('Сохранение в : ' + IntToStr(i) + '.txt');
      saveArrayToFile(intToStr(i)+'.txt');


      Memo1.Lines.Add(' завершено' ) ;
      Memo1.Lines.Add('');
      // list clearing
      freememory();

    end;


  m_film.Destroy;
  closeFile(m_srcFile);




  // merge time
  Memo1.Lines.Add('Подготовка к слиянию');

  m_openedFilesCount := m_filesCount;
  // opening all files
  filesArrayInit();
  // create array for values,and read first value
  allocateMergeMemory();

  // opening output file
  AssignFile(m_outFile,outFile.FileName);
  rewrite(m_outFile);


 Memo1.Lines.Add('Начало слияния') ;
  m_filesDropped := 0;
  while(m_filesDropped <10) do
  begin
    // find lowest element in array of values
    index := getMinElementIndex();
    // write value to file
    write(m_outFile,m_fileData[index]^);
    // read next value from this file
    updateFileData(index);


  end;


  Memo1.Lines.Add('Завершено');
  // dispose filedata memory
  freeMergeMemory();
  // destroy m_fileArray ( after disposing in merge )
  setLength(m_fileArray,0);
  closeFile(m_outFile);






end;

function TForm1.getFileSize: integer;
var f : file of byte;
begin
  AssignFile(f,srcFile.FileName);
  reset(f);
  getFileSize:= FileSize(f);
  closeFile(f);
end;

procedure TForm1.init;
begin
  m_onePercent :=10;
end;

procedure TForm1.readPart();
var ptr:link;
begin
  m_pictureReaded := 0  ;

  while( ( m_pictureReaded < m_arraySize) AND (NOT EOF(m_srcFile) ) ) do
  begin
    ptr := nil;
    new(ptr);
    read( m_srcFile , ptr^ );
    m_film.Add(ptr);
    inc(m_pictureReaded);
  end;
end;

procedure TForm1.freeMemory;
var i : integer;
ptr : link ;
begin
  for i:= 0 to m_film.Count-1 do
  begin
      ptr:=m_film[i];
      dispose(ptr);
      m_film[i]:=nil;
  end;
  m_film.Clear;
end;            ///

procedure TForm1.saveArrayToFile(fileName:string);
  var f : file of picture;
  ptr : link;
  i:integer;
begin
  AssignFile(f,filename);
  rewrite(f);

  for i:= 0 to m_film.Count-1 do
  begin
  ptr := m_film[i];
  write(f,ptr^);
  end;

  closeFile(f);

end;

procedure TForm1.allocateMergeMemory;
var i:integer;
ptr : link;
begin
  setlength(m_fileData,m_filesCount);

  // allocate memory for all elements
  for i:=0 to m_filesCount-1 do
  begin
      ptr:=nil;
      new(ptr);
      m_fileData[i]:=ptr;
  end;


  // write first elements..
  for i:=0 to m_filesCount-1 do
  begin
    read(m_fileArray[i]^,m_fileData[i]^);
  end;

end;

procedure TForm1.filesArrayInit;

var i:integer;
ptr : pictureFileLink;

begin
  setlength(m_fileArray,m_filesCount);
  for i:= 0 to m_filesCount-1 do
  begin
    ptr:=nil;
    new(ptr);
    AssignFile(ptr^,inttostr(i+1)+'.txt');
    reset(ptr^);
    m_fileArray[i] := ptr;



  end;

end;

procedure TForm1.freeMergeMemory;
var ptr : link;
i:integer;
begin
  for i:= 0 to m_openedFilesCount-1 do
  begin
    ptr:= m_fileData[i];
    dispose(ptr);
    m_fileData[i]:= nil;

  end;
  setlength(m_fileData,0);


end;

function TForm1.getMinElementIndex():integer;
var i,index:integer;
element : link;
begin


  for i:=0 to m_filesCount-1 do
  begin
      if(m_fileArray[i] <> nil)then
      begin
        element := m_fileData[i];
        index := i;
        break;
      end;

  end;

  for i:=0 to m_filesCount-1 do
  begin
    if ( m_fileArray[i] = nil) then continue;
    if ( filmCompare( m_fileData[i],element) = -1) then
      begin
      element := m_fileData[i];
      index := i;
    end;

  end;

  getMinElementIndex := index;

end;

procedure TForm1.updateFileData(index: integer);
var i : integer;
swap : link;
begin
  if( NOT ( EOF( m_fileArray[index]^ ) ) )  then
    read(m_fileArray[index]^,m_fileData[index]^)
  else begin
    Memo1.Lines.Add('DROP:' + inttostr(index)) ;
    closeFile(m_fileArray[index]^);
    dispose(m_fileArray[index]);
    m_fileArray[index]:=nil;

    inc(m_filesDropped);


  end;
end;

end.

