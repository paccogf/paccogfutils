//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : CalculusThread(0), TForm(Owner), CurrentFile(""), isInitializing(false)

{
WindowProc=SubClassWndProc;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::EnableReanalyze()
{
ReanalyzeButton->Enabled=true;
MainMenuReanalyze->Enabled=true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LocalInit()
{

DragAcceptFiles(this->Handle, TRUE);
if(ParamCount() > 0)
{
   if(FileExists(ParamStr(1)))
   {
       CurrentFile=ParamStr(1);
       EnableReanalyze();
       isInitializing=false;
       MainMenuReanalyzeClick(this);
   }
}
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::MainMenuExitClick(TObject *Sender)
{
MainForm->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::MainMenuOpenClick(TObject *Sender)
{

if(!OpenDialog->Execute()) return;

CurrentFile=OpenDialog->FileName;
EnableReanalyze();
MainMenuReanalyzeClick(this);
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::MainMenuReanalyzeClick(TObject *Sender)
{
int i;
int intervalo;
int intervalo_grafico;
int first_n_bytes;

// QUE ESTE VALOR SEA ENTERO SE COMPRUEBA YA SOLO, SI NO SALTA UNA EXCEPCION
intervalo = Edit1->Text.ToInt();
if(intervalo <= 0)
{
   MessageBox(0, "Rango del intervalo incorrecto", "Error", MB_ICONERROR);
   return;
}

// MIRAR INTERVALO REPRESENTACION GRAFICO
if(CheckBox2->Checked)
{
  intervalo_grafico = Edit2->Text.ToInt();
  if(intervalo_grafico <= 0)
  {
     MessageBox(0, "Valor invalido de intervalo grafico", "Error", MB_ICONERROR);
     return;
  }
}
else intervalo_grafico=-1;  // AUTOM�TICO


// COMPROBAR LIMITE SUPERIOR A CALCULAR DEL FICHERO
if(CheckBox1->Checked)
{
   first_n_bytes = Edit3->Text.ToInt();
   if(first_n_bytes <= intervalo)
   {
      MessageBox(0, "El tama�o m�ximo debe ser mayor o igual que el intervalo", "Error", MB_ICONERROR);
      return;
   }
}
else first_n_bytes=-1;    // CALCULAR TODO EL FICHERO
///

Caption="Grent - " + CurrentFile;

MainChart->Visible = false;
Series1->Clear();

ProgressDlg->ProgressBar->Position=0;

if(CalculusThread) delete CalculusThread;

CalculusThread = new TWorkThread(CurrentFile, intervalo, first_n_bytes, ProgressDlg);


// YA TENEMOS LOS RESULTADOS EN CalculusThread->Results;
if(ProgressDlg->InitCalculus() == mrOk)
{
   if(CalculusThread->nResults == -1) return;
   if(intervalo_grafico == -1) intervalo_grafico=CalculusThread->nResults/3000;
   if(intervalo_grafico == 0) intervalo_grafico+=1;

   ProgressDlg->Show();
   for(i=0; i< CalculusThread->nResults; i+=intervalo_grafico)
   {
      Series1->AddXY(i, CalculusThread->Results[i], "", clLime);
      ProgressDlg->SetProgress(100 * i / CalculusThread->nResults);
   }
   ProgressDlg->Hide();
}
MainChart->Visible = true;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Edit1KeyPress(TObject *Sender, char &Key)
{
if(Key >= '0' && Key <= '9') return ;
if(Key == 13)
{
   if(MainMenuReanalyze->Enabled) MainMenuReanalyzeClick(Sender);  // TECLA ENTER
   else MainMenuOpenClick(Sender);
   // CUANDO UN TEdit RECIBE UN CARACTER 13 PITA, ASI LO EVITAMO
   Key=0;
   return;
}
if(Key == 8) return;      // CARACTER BORRAR

Key=0;




}
//---------------------------------------------------------------------------



void __fastcall TMainForm::FormShow(TObject *Sender)
{
if(isInitializing) LocalInit();

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
if(CalculusThread) delete CalculusThread;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ToolButton4Click(TObject *Sender)
{
 Series1->Clear();
 if(CalculusThread) delete CalculusThread;
 CalculusThread=0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
isInitializing=true;        
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::MainToolBarDblClick(TObject *Sender)
{
ToolButton4->Visible=!ToolButton4->Visible;    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CheckBox1Click(TObject *Sender)
{
Edit3->Enabled=CheckBox1->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::CheckBox2Click(TObject *Sender)
{
Edit2->Enabled=CheckBox2->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MainChartZoom(TObject *Sender)
{
//MessageBox(0, "TMainForm::MainChartZoom", "Grent", MB_ICONINFORMATION);
MainChart->LeftAxis->Maximum;

MainChart->BottomAxis->Maximum;
MainChart->BottomAxis->Minimum;

}
//---------------------------------------------------------------------------





void __fastcall TMainForm::SubClassWndProc(Messages::TMessage &Message)
{
INT nCnt, nIndex, nSize;
LPTSTR  pszFileName;
HDROP   hDrop;

if (Message.Msg == WM_DROPFILES)
{
  hDrop = (HDROP)Message.WParam;
  nCnt  = DragQueryFile(hDrop, (UINT)-1, NULL, 0);

  for(nIndex = 0; nIndex < nCnt; ++nIndex)
  {
    if(0 == (nSize = DragQueryFile(hDrop, nIndex, NULL, 0)))
    continue;

    pszFileName = new TCHAR[++nSize];

    if(DragQueryFile(hDrop, nIndex, pszFileName, nSize))
    {

       if(FileExists(pszFileName))
       {
           CurrentFile=pszFileName;
           EnableReanalyze();
           MainMenuReanalyzeClick(this);
       }
    }

    delete [] pszFileName;
}
DragFinish(hDrop);
}
else
{
  WndProc(Message);
}

}
//---------------------------------------------------------------------------

