#include <vcl.h>
#pragma hdrstop

#include "NetworkManager.h"
#include <System.Threading.hpp>
#include <string>
#include <PaintTool.h>

__fastcall TNetworkManager::TNetworkManager(TComponent* Owner)
{
	IdClient = new TIdTCPClient(Owner);
    IdClient->OnConnected = IdClientConnected;
    IdClient->OnDisconnected = IdClientDisconnected;
    IdClient->Port = 8080;
}

__fastcall TNetworkManager::~TNetworkManager()
{
    if (IdClient)
    {
		IdClient->Disconnect();
        delete IdClient;
    }
}
// ПОТОК: Запуск чтения данных в отдельном потоке
void __fastcall TNetworkManager::IdClientConnected(TObject*AContext)
{
    TThread::Queue(NULL, [this]() {
		if (OnConnect) OnConnect(this);
    });

    // Запускаем бесконечный цикл чтения в отдельном потоке
    TTask::Run([this]()
    {
        try
        {
            while (IdClient->Connected())
            {
                // Читаем строку, явно преобразуя в UTF8String для совместимости с std::string
                std::string Data = UTF8String(IdClient->IOHandler->ReadLn()).c_str();

				// Оповещаем форму о полученных данных (возвращаемся в главный поток VCL)
				TThread::Queue(NULL, [this, Data]() {
                    if (OnReceive) {
                        OnReceive(this, Data);
                    }
                });
            }
        }
        catch (...)
		{
			if (IdClient->Connected()) {
                IdClient->Disconnect();
            }
        }
    });
}
// ПОТОК: Оповещение об отключении
void __fastcall TNetworkManager::IdClientDisconnected(TObject*AContext)
{
	// Оповещаем форму, что соединение разорвано
    TThread::Queue(NULL, [this]() {
        if (OnDisconnect) OnDisconnect(this);
    });
}
// УПРАВЛЕНИЕ: Подключение
bool TNetworkManager::Connect(const std::string Host, int Port)
{
    if (IdClient->Connected()) return true;

    try
    {
        IdClient->Host = Host.c_str();
        IdClient->Port = Port;
        IdClient->Connect();
        return true;
    }
    catch (...)
    {
		// Не удалось подключиться
        return false;
    }
}
// УПРАВЛЕНИЕ: Отключение
void TNetworkManager::Disconnect()
{
    if (IdClient->Connected()) {
        IdClient->Disconnect();
	}
}
// УПРАВЛЕНИЕ: Отправка данных
void TNetworkManager::Send(const std::string AData)
{
    if (!IdClient->Connected()) return;

    try
	{
        IdClient->IOHandler->WriteLn(AData.c_str());
    }
    catch (...)
	{
        Disconnect();
	}
}
// PaintTool
void __fastcall TNetworkManager::ApplyRemoteStart(int x, int y, int toolType, int color, int size)
{
	if (!FTool) return;
	FTool->SetTool((TToolType)toolType);
	FTool->SetColor((TColor)color);
	FTool->SetPenSize(size);
	FTool->MouseDown(x, y);
}
