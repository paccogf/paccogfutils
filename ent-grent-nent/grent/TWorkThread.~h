//---------------------------------------------------------------------------

#ifndef TWorkThreadH
#define TWorkThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "TWorkThread.h"
#include <math.h>
//---------------------------------------------------------------------------

#ifndef log2
#define log2(x) (log10(x)/log10(2))
#endif

#ifndef min
#define min(x, y) ((x<y)?x:y)
#endif


// LA FUNCION CALLBACK A IMPLEMENTAR POR EL CONTADOR DE PROGRESO ES ALGO ASI COMO:
// TProgressCounter.CallBack(int percent);
// typedef  TProgressDlg TProgressCounter;


//typedef TProgressCounter; // DECLARACION FORWARD

class TWorkThread : public TThread
{
private:
        double * __fastcall GetResult() { return result?result:0; }
        int __fastcall GetnResult() { return leidos; }
        void __fastcall SendProgress();
        void __fastcall FinalizeProgress();

        int currentIndex;
        TFileStream * fs;
        TProgressCounter *ProgressCounter;
        BYTE *buffer;
        double *result;
        int leidos;
        AnsiString FileName;
        int ENTORNO;
        int maxfilesize;

protected:
        void __fastcall Execute();

public:
        __fastcall TWorkThread(AnsiString filename, int entorno, int first_n_bytes, TProgressCounter *ProgressObj);
        __fastcall virtual ~TWorkThread();
        __property double * Results = {read=GetResult, nodefault};
        __property int  nResults = {read=GetnResult, nodefault};

};
//---------------------------------------------------------------------------
#endif
