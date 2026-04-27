#ifndef Unit1H
#define Unit1H

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <System.ImageList.hpp>
#include <IdBaseComponent.hpp>
#include <string>
#include <vector>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.Imaging.pngimage.hpp>

#include "PaintTool.h"
#include "NetworkManager.h"
class TForm1 : public TForm
{  	// IDE-managed Components
__published:
	TPanel *Panel1;

	TSpeedButton *BtnPen;
	TSpeedButton *BtnLine;
	TSpeedButton *BtnEraser;
	TSpeedButton *BtnColor;
	TSpeedButton *BtnClear;
	TTrackBar *TrkPenSize;
	TColorDialog *ColorDialog1;
	TImageList *ImageList1;

	TPaintBox *Image1;

	TEdit *EditHost;
	TEdit *EditPort;
	TButton *BtnConnect;
	TEdit *EditMessage;
	TButton *BtnSend;
	TMemo *MemoLog;
	TButton *BtnExit;
	TIdTCPClient *IdTCPClient1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BtnPenClick(TObject *Sender);
	void __fastcall BtnLineClick(TObject *Sender);
	void __fastcall BtnEraserClick(TObject *Sender);
	void __fastcall BtnColorClick(TObject *Sender);
	void __fastcall BtnClearClick(TObject *Sender);
	void __fastcall TrkPenSizeChange(TObject *Sender);
	void __fastcall BtnExitClick(TObject *Sender);
	void __fastcall BtnConnectClick(TObject *Sender);
	void __fastcall BtnSendClick(TObject *Sender);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall Image1Paint(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);


private:
	TPaintTool *MyTool;
	TNetworkManager *NetManager;
	bool isDrawing;
	TBitmap *DrawingBitmap;

	void __fastcall OnNetworkConnect(TObject* Sender);
	void __fastcall OnNetworkDisconnect(TObject* Sender);
	void __fastcall OnNetworkReceive(TObject* Sender, const std::string Data);

	void __fastcall ProcessReceivedCommand(const std::string Cmd);

public:
	__fastcall TForm1(TComponent* Owner);
	__fastcall ~TForm1();
};
extern PACKAGE TForm1 *Form1;
#endif
