//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TProgressDlg.h"
#include "Main.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProgressDlg *ProgressDlg;
//---------------------------------------------------------------------------
__fastcall TProgressDlg::TProgressDlg(TComponent* Owner)
        : TForm(Owner)
{
ProgressDlg->ProgressBar->Min=0;
ProgressDlg->ProgressBar->Max=100;

}

//---------------------------------------------------------------------------

int __fastcall TProgressDlg::InitCalculus()
{
ProgressDlg->ProgressBar->Min=0;
ProgressDlg->ProgressBar->Max=100;

ModalResult=mrOk;        // SI TODO VA BIEN NO LO CAMBIAMOS
//MessageBox(0, AnsiString("TProgressDlg::FormShow ModalResult= " + AnsiString(ModalResult)).c_str(), "", MB_ICONINFORMATION);
Caption="Progreso 0%";
MainForm->CalculusThread->Resume();
return ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TProgressDlg::SetProgress(int progress)
{
ProgressBar->Position=progress;
Caption="Progreso " + AnsiString(progress) + AnsiString('%');
}

//---------------------------------------------------------------------------

void __fastcall TProgressDlg::EndProgress()
{
Button1->Click();   // CERRAR DIALOGO MODAL CON modalResult=OK

}


void __fastcall TProgressDlg::BitBtn1Click(TObject *Sender)
{
//MessageBox(0, "que coño hacemos aqui", "", 0);
ModalResult=mrCancel;  // ASI SE ENTERA LA FORMA PRINCIPAL DE QUE PULSAMOS CANCEL
MainForm->CalculusThread->Terminate();
}



