#include <vcl.h>
#pragma hdrstop

#include "Server1.h"
#include <IdContext.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPServer.hpp>

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;

__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
/*
 METHOD: BroadcastCommand
 PURPOSE: Relay received drawing/text data to all connected clients
 MECHANISM: Uses Contexts->LockList to ensure thread-safety during iteration
 */
void __fastcall TForm2::BroadcastCommand(const std::string &Command, TIdContext *SenderContext)
{
	UnicodeString UCommand = Command.c_str();
	// THREAD-SAFETY: Syncing with UI thread for logging purposes
	TThread::Queue(NULL, [this, UCommand]() {
		MemoLog->Lines->Add(L"Broadcasting: " + UCommand);
	});
	// RESOURCE MANAGEMENT: Lock the list of clients to prevent mid-broadcast disconnections
	TIdContextList *List = IdTCPServer1->Contexts->LockList();

	__try {
		SendCommandToAll(List, UCommand);
	}
	__finally {
	// IMPORTANT: Always unlock the list to prevent server deadlock
		IdTCPServer1->Contexts->UnlockList();
	}
}
/*
 EVENT: IdTCPServer1Execute
 PURPOSE: Main execution loop for each client connection thread
 */
void __fastcall TForm2::IdTCPServer1Execute(TIdContext *Context)
{
	try {
	// NON-BLOCKING CHECK: Process data only if the buffer is not empty
		if (Context->Connection->Connected() && !Context->Connection->IOHandler->InputBufferIsEmpty())
		{
		// NETWORK I/O: Reading line-terminated command string
            UnicodeString ReceivedData = Context->Connection->IOHandler->ReadLn();
            std::string ReceivedCommand = AnsiString(ReceivedData).c_str();

            TThread::Queue(NULL, [this, ReceivedData]() {
                MemoLog->Lines->Add(L"Received: " + ReceivedData);
			});
			// LOGIC: Distribute the received coordinates/message to all other peers
            BroadcastCommand(ReceivedCommand, Context);
        }
	} catch (Exception &E) {
	// EXCEPTION HANDLING: Clean disconnect on socket errors
		Context->Connection->Disconnect();
    }
}
/*
 UI EVENT: BtnStartClick
 PURPOSE: Toggle Server state (Listen / Stop)
 */
void __fastcall TForm2::BtnStartClick(TObject *Sender)
{
	if (IdTCPServer1->Active) {
		IdTCPServer1->Active = false;
		MemoLog->Lines->Add(L"Server stopped.");
		BtnStart->Caption = L"Start Server";
	} else {
		try {
		// PORT CONFIGURATION: Default port binding for the listener
            if (IdTCPServer1->DefaultPort == 0) {
                IdTCPServer1->DefaultPort = 8080;
            }
			IdTCPServer1->Active = true;
			MemoLog->Lines->Add(L"Server started on port " + IntToStr(IdTCPServer1->DefaultPort) + L".");
			BtnStart->Caption = L"Stop Server";
		}
		catch (Exception &E) {
			MemoLog->Lines->Add(L"Error starting server: " + E.Message);
		}
	}
}
/*
 METHOD: SendCommandToAll
 PURPOSE: Low-level data transmission to the client list
 */
void __fastcall TForm2::SendCommandToAll(TIdContextList *List, const UnicodeString &UCommand)
{
// ITERATION: Walk through the thread-safe list of active connections
    for (int i = 0; i < List->Count; i++)
	{
		TIdContext* Context = static_cast<TIdContext*>(List->Items[i]);

        if (Context && Context->Connection)
        {
			try {
			// TRANSMISSION: Send the command followed by a CRLF terminator
                Context->Connection->IOHandler->WriteLn(UCommand);
			} catch (Exception &E) {
            // FAIL-SAFE: Disconnect client if transmission fails
				Context->Connection->Disconnect();
            }
        }
    }
}

