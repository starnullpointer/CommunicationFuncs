#ifndef COMMUNICATION_H
#define COMMUNICATION_H
//#include "../messageblock/messageblock.h"
#include "../messageblock/packet.h"
//#include "../Cstring/Cstring.h"
#include <Adafruit_WINC1500Client.h>

enum CommunicationStatus{ComSucess,ComWifiDisconnect,ComServerDisconnect,BadPacket,ComError};

//maybe update??????????
// void Init();
void Setup_Wifi_and_Server(Adafruit_WINC1500Client& client);
void PrintWifiStatus();
void ConnectWifi();
void ConnectServer(Adafruit_WINC1500Client& client);
bool WaitForUpdate(Adafruit_WINC1500Client& client);
Packet GetMessage(Adafruit_WINC1500Client& client);
bool GetWifiConnected();
//    void Write(Packet Writeme);
void Send(const char Sendme[], Adafruit_WINC1500Client& client);
//member variables
// IPAddress server;
// Adafruit_WINC1500Client client;
//Adafruit_WINC1500 WiFi;

#endif // COMMUNICATION_H
