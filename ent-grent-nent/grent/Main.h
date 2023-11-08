//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ToolWin.hpp>
#include "TProgressDlg.h"
#include "TWorkThread.h"
#include <DB.hpp>
#include <DbChart.hpp>
#include <DBClient.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <Chartfx3.hpp>
#include <OleCtrls.hpp>

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TStatusBar *MainStatusBar;
        TToolBar *MainToolBar;
        TToolButton *ToolButton1;
        TToolButton *OpenFileButton;
        TToolButton *ReanalyzeButton;
        TMainMenu *MainMenu;
        TMenuItem *MainMenuArchive;
        TMenuItem *MainMenuOpen;
        TMenuItem *N1;
        TMenuItem *MainMenuExit;
        TOpenDialog *OpenDialog;
        TImageList *MainImageList;
        TChart *MainChart;
        TToolButton *ToolButton2;
        TEdit *Edit1;
        TMenuItem *MainMenuReanalyze;
        TLabel *Label1;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TEdit *Edit2;
        TLabel *valores;
        TLabel *Label3;
        TCheckBox *CheckBox1;
        TEdit *Edit3;
        TCheckBox *CheckBox2;
        TAreaSeries *Series1;
        TToolButton *ToolButton3;

        void __fastcall MainMenuExitClick(TObject *Sender);
        void __fastcall MainMenuOpenClick(TObject *Sender);
        void __fastcall MainMenuReanalyzeClick(TObject *Sender);
        void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
        void __fastcall EnableReanalyze();
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall ToolButton4Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall MainToolBarDblClick(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall CheckBox2Click(TObject *Sender);
        void __fastcall MainChartZoom(TObject *Sender);
        void __fastcall MainChartUndoZoom(TObject *Sender);
        void __fastcall MainChartScroll(TObject *Sender);


private:	// User declarations
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
        void __fastcall TMainForm::SubClassWndProc(Messages::TMessage &Message);
        void __fastcall LocalInit();
        void __fastcall UpdateChart(double *results, int from, int to);
        TWorkThread *CalculusThread;
        AnsiString CurrentFile;
        bool isInitializing;

};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------

#define MAX(a, b)  ((a>b)?a:b)
#define MIN(a, b)  ((a<b)?a:b)


#endif
