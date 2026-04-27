#ifndef Server1H
#define Server1H

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdContext.hpp>

class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TIdTCPServer *IdTCPServer1;
	TMemo *MemoLog;
	TButton *BtnStart;
	void __fastcall IdTCPServer1Execute(TIdContext *Context);
	void __fastcall BtnStartClick(TObject *Sender);
private:  void __fastcall BroadcastCommand(const std::string &Command, TIdContext *SenderContext = NULL);
void __fastcall SendCommandToAll(TIdContextList *List, const UnicodeString &UCommand);	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
extern PACKAGE TForm2 *Form2;
#endif
