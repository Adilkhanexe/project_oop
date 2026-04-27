#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <sstream>
#include <vector>

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

// Constructor
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	isDrawing = false;
	MyTool = new TPaintTool();
	NetManager = new TNetworkManager(this);
	NetManager->SetTool(MyTool);

	//Bitmap for PaintBox
	DrawingBitmap = new TBitmap();

	NetManager->OnConnect = OnNetworkConnect;
	NetManager->OnDisconnect = OnNetworkDisconnect;
	NetManager->OnReceive = OnNetworkReceive;

	EditHost->Text = "127.0.0.1";
	EditPort->Text = "8080";
}
// Destructor: Object Removal
__fastcall TForm1::~TForm1()
{
	if (MyTool) delete MyTool;
	if (NetManager) delete NetManager;
	if (DrawingBitmap) delete DrawingBitmap;
}
//Initialization when creating a window
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	MyTool->SetPenSize(3);
	MyTool->SetTool(TOOL_PEN);

	isDrawing = false;
	MemoLog->Lines->Add(L"The client is ready to work");
}
//Method for TPaintBox
void __fastcall TForm1::Image1Paint(TObject *Sender)
{
	//Force copy our Bitmap to the visible canvas (TPaintBox)
	Image1->Canvas->Draw(0, 0, DrawingBitmap);
}
// NETWORK LOGIC AND HANDLERS
void __fastcall TForm1::BtnConnectClick(TObject *Sender)
{
	if (NetManager->GetIdClient()->Connected())
	{
		NetManager->Disconnect();
	}
	else
	{
		std::string Host = AnsiString(EditHost->Text).c_str();
		int Port = 8080;
		try {
			Port = StrToInt(EditPort->Text);
		} catch(...) {
			Port = 8080;
		}

		MemoLog->Lines->Add(L"Connecting to " + EditHost->Text + L":" + IntToStr(Port) + L"...");

		bool success = NetManager->Connect(Host, Port);
		if (!success) {
			MemoLog->Lines->Add(L"Error: Unable to connect");
		}
	}
}

void __fastcall TForm1::BtnSendClick(TObject *Sender)
{
	if (!NetManager->GetIdClient()->Connected()) {
		MemoLog->Lines->Add(L"No connection to the server");
		return;
	}

	if (EditMessage->Text.IsEmpty()) return;

	std::string msg = AnsiString(EditMessage->Text).c_str();
	std::string command = "CHAT;" + msg;

	NetManager->Send(command);

	MemoLog->Lines->Add(L"Я: " + EditMessage->Text);
	EditMessage->Clear();
}

void __fastcall TForm1::OnNetworkConnect(TObject* Sender)
{
	BtnConnect->Caption = L"Disconnect";
	Panel1->Color = clMoneyGreen;
	MemoLog->Lines->Add(L"--- CONNECTION ESTABLISHED ---");
}

void __fastcall TForm1::OnNetworkDisconnect(TObject* Sender)
{
	BtnConnect->Caption = L"Connect";
	Panel1->Color = clBtnFace;
	MemoLog->Lines->Add(L"--- CONNECTION BROKEN ---");
}

void __fastcall TForm1::OnNetworkReceive(TObject* Sender, const std::string Data)
{
	ProcessReceivedCommand(Data);
}
// COMMANDS ANALYSIS
void __fastcall TForm1::ProcessReceivedCommand(const std::string Cmd)
{
	std::stringstream ss(Cmd);
	std::string segment;
	std::vector<std::string> parts;

	while (std::getline(ss, segment, ';')) {
		parts.push_back(segment);
	}

	if (parts.empty()) return;

	std::string commandType = parts[0];

	if (commandType == "CHAT" && parts.size() >= 2)
	{
		UnicodeString msgText = parts[1].c_str();
		MemoLog->Lines->Add(L"Interlocutor:" + msgText);
	}
	else if (commandType == "CLEAR")
	{
		DrawingBitmap->Canvas->Brush->Color = clWhite;
		DrawingBitmap->Canvas->FillRect(Rect(0, 0, Image1->Width, Image1->Height));
		Image1->Repaint();
		MemoLog->Lines->Add(L"Canvas cleaned remotely");
	}
	// --- 3. НАЧАЛО РИСОВАНИЯ ---
	else if (commandType == "START" && parts.size() >= 6)
	{
		int x = std::stoi(parts[1]);
		int y = std::stoi(parts[2]);
		int toolType = std::stoi(parts[3]);
		TColor color = (TColor)std::stoi(parts[4]);
		int size = std::stoi(parts[5]);

	NetManager->ApplyRemoteStart(x, y, toolType, color, size);

	}
	else if (commandType == "MOVE" && parts.size() >= 3)
	{
		int x = std::stoi(parts[1]);
		int y = std::stoi(parts[2]);

		MyTool->MouseMove(DrawingBitmap->Canvas, x, y);
		Image1->Repaint();
	}
	else if (commandType == "UP" && parts.size() >= 3)
	{
		int x = std::stoi(parts[1]);
		int y = std::stoi(parts[2]);

		MyTool->MouseUp(DrawingBitmap->Canvas, x, y);
		Image1->Repaint();
	}
}
// DRAWING LOGIC
void __fastcall TForm1::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	isDrawing = true;

	MyTool->MouseDown(X, Y);

	if (NetManager->GetIdClient()->Connected())
	{
		std::stringstream ss;
		ss << "START;" << X << ";" << Y << ";"
		   << (int)MyTool->GetCurrentTool() << ";"
		   << (int)MyTool->GetCurrentColor() << ";"
		   << MyTool->GetPenSize();

		NetManager->Send(ss.str());
	}
}

void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	if (!isDrawing) return;

	MyTool->MouseMove(DrawingBitmap->Canvas, X, Y);
	Image1->Repaint();

	if (NetManager->GetIdClient()->Connected())
	{
		std::stringstream ss;
		ss << "MOVE;" << X << ";" << Y;
		NetManager->Send(ss.str());
	}
}

void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (!isDrawing) return;
	MyTool->MouseUp(DrawingBitmap->Canvas, X, Y);
	isDrawing = false;
	Image1->Repaint();
	if (NetManager->GetIdClient()->Connected())
	{
		std::stringstream ss;
		ss << "UP;" << X << ";" << Y;
		NetManager->Send(ss.str());
	}
}
// TOOLS
void __fastcall TForm1::BtnPenClick(TObject *Sender) { MyTool->SetTool(TOOL_PEN); }
void __fastcall TForm1::BtnLineClick(TObject *Sender) { MyTool->SetTool(TOOL_LINE); }
void __fastcall TForm1::BtnEraserClick(TObject *Sender) { MyTool->SetTool(TOOL_ERASER); }

void __fastcall TForm1::BtnColorClick(TObject *Sender)
{
	if (ColorDialog1->Execute()) {
		MyTool->SetColor(ColorDialog1->Color);
	}
}

void __fastcall TForm1::BtnClearClick(TObject *Sender)
{
	DrawingBitmap->Canvas->Brush->Color = clWhite;
	DrawingBitmap->Canvas->FillRect(Rect(0, 0, Image1->Width, Image1->Height));
	Image1->Repaint();

	if (NetManager->GetIdClient()->Connected()) {
		NetManager->Send("CLEAR");
		MemoLog->Lines->Add(L"The canvas is cleaned.");
	}
}

void __fastcall TForm1::TrkPenSizeChange(TObject *Sender)
{
	MyTool->SetPenSize(TrkPenSize->Position);
}

void __fastcall TForm1::BtnExitClick(TObject *Sender)
{
	Close();
}
void __fastcall TForm1::FormShow(TObject *Sender)
{
DrawingBitmap->Width = Image1->Width;
    DrawingBitmap->Height = Image1->Height;

    DrawingBitmap->Canvas->Brush->Color = clWhite;
    DrawingBitmap->Canvas->FillRect(Rect(0, 0, Image1->Width, Image1->Height));

	Image1->Repaint();
}
void __fastcall TForm1::FormResize(TObject *Sender)
{
    // Проверяем, чтобы битмап не стерся при изменении размера
    if (DrawingBitmap->Width < Image1->Width || DrawingBitmap->Height < Image1->Height) {
        TBitmap *Temp = new TBitmap();
        Temp->Assign(DrawingBitmap);
        DrawingBitmap->SetSize(Image1->Width, Image1->Height);
        DrawingBitmap->Canvas->Brush->Color = clWhite;
        DrawingBitmap->Canvas->FillRect(Rect(0,0, DrawingBitmap->Width, DrawingBitmap->Height));
        DrawingBitmap->Canvas->Draw(0, 0, Temp); // Возвращаем старый рисунок
        delete Temp;
    }
}
