#ifndef NetworkManagerH
#define NetworkManagerH

#include <System.Classes.hpp>
#include <IdTCPClient.hpp>
#include <IdBaseComponent.hpp>
#include <IdContext.hpp>
#include <string>
#include <PaintTool.h>
class TPaintTool;

// Определение типа для событий: OnConnect, OnDisconnect
typedef void __fastcall (__closure *TNetworkEvent)(TObject* Sender);

// Определение типа для события получения данных
typedef void __fastcall (__closure *TDataEvent)(TObject* Sender, const std::string Data);

class TNetworkManager : public TObject
{
private:
    TIdTCPClient *IdClient;
	void __fastcall IdClientConnected(TObject* Sender);
	void __fastcall IdClientDisconnected(TObject* Sender);
	 TPaintTool* FTool;

public:
	void __fastcall SetTool(TPaintTool* ATool) { FTool = ATool; }
	void __fastcall ApplyRemoteStart(int x, int y, int toolType, int color, int size);
    __fastcall TNetworkManager(TComponent* Owner);
	__fastcall ~TNetworkManager();

    TNetworkEvent OnConnect;
    TNetworkEvent OnDisconnect;
    TDataEvent OnReceive;

    TIdTCPClient* GetIdClient() const { return IdClient; }

    bool Connect(const std::string Host, int Port);
    void Disconnect();
    void Send(const std::string AData);
};
#endif
