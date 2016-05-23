// #include "Communication.h"
#include "CommunicationFuncs.h"
#include <Adafruit_WINC1500.h>

Adafruit_WINC1500 WiFi(WINC_CS, WINC_IRQ, WINC_RST);
IPAddress server(IP1, IP2, IP3, IP4);

void Setup_Wifi_and_Server(Adafruit_WINC1500Client& client)
{
    ConnectWifi();
    ConnectServer(client);
}

void PrintWifiStatus()
{
    Serial.print("status: ");
    if (WiFi.status() == WL_NO_SHIELD)
        Serial.println("No Sheild");
    else if(WiFi.status() == WL_IDLE_STATUS)
        Serial.println("Idle");
    else if (WiFi.status() == WL_NO_SSID_AVAIL)
        Serial.println ("No SSID Avail");
    else if (WiFi.status() == WL_SCAN_COMPLETED)
        Serial.println("Scan Completed");
    else if (WiFi.status() == WL_CONNECTED)
        Serial.println("Connected");
    else if (WiFi.status() == WL_CONNECT_FAILED)
        Serial.println("Connect Failed");
    else if (WiFi.status() == WL_CONNECTION_LOST)
        Serial.println("Connection Lost");
    else if (WiFi.status() == WL_DISCONNECTED)
        Serial.println("Disconnected");
}

void ConnectWifi()
{
    // todo... do we really need 2 statues???
    int status;
    // -----Wifi Setup-----
    Serial.println("Setting up Wifi");

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println(F("WiFi shield not present"));
      // don't continue:
      while (true);
    }

    //---attempt to connect to Wifi network---
    while (WiFi.status() != WL_CONNECTED) {
        PrintWifiStatus();
      Serial.print(F("Attempting to connect to SSID: "));
      Serial.println(ssid);
      // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);

      // wait 15 seconds for connection:
      uint8_t timeout = 15;
      while (timeout && (WiFi.status() != WL_CONNECTED)) {
        timeout--;
        delay(1000);
      }
    }
    Serial.println(F("Connected to wifi"));
}

void ConnectServer(Adafruit_WINC1500Client& client)
{

    Serial.println(F("\nStarting connection to Server..."));
    // if you get a connection, report back via serial
    Serial.println("====================");
    Serial.println(server);
    Serial.println(port);
    Serial.println("====================");
    while(!client)
    {
        if(client.connect(server, port)){
            Serial.println(F("connected to Server"));
            break;
        }
        else{
            Serial.println("Could not connect to Server");
        }
        Serial.println("Attempting to reconnect");
    }   
}

bool GetWifiConnected()
{
     if(WiFi.status() != WL_CONNECTED)
        return false;
    return true;
}

//update checks the currents statuses of the connections, either wifi, server or message
bool WaitForUpdate(Adafruit_WINC1500Client& client)
{
    Serial.println("Waiting for message");
    while(!client.available()){

    //checks if wifi is connected, if not it reconnects
    if (WiFi.status() != WL_CONNECTED) {
        Setup_Wifi_and_Server(client);
    }

    //if server is not connected it tries to reconnected
    while(!client)
    {
        Serial.println("We got disconnected from Server:");
        Serial.println("Attempting to reconnect");
        if(client.connect(server, port)){ //checks that server connected
            Serial.println(F("connected to Server"));

        }
        else{
            Serial.println("Could not connect to Server");
        }
    }
    }
    return true;
}
//whoe ever calls this function has to check that client has data for us
// aka... call wait for update first..
Packet GetMessage(Adafruit_WINC1500Client& client)
{
    char c;
#ifdef DEBUG
    Serial.println(F("Start messaging block"));
#endif

    String received = "";

    // if there are incoming bytes available
    // from the server, read them and print them:
    //  Serial.println(client.available());
    while (client.available()) {    //read incoming message
#ifdef DEBUG
        Serial.print("available: ");Serial.println(client.available());
#endif
        c = client.read();            //reads message one character at a time
        //    Serial.print("read:");
        //    Serial.println(c);
        received += c;                //takes each character to build into received

#ifdef DEBUG
        Serial.print(F("[")); Serial.print(c); Serial.print(F("]"));
#endif
    }

#ifdef DEBUG
    Serial.println("");
    Serial.print("Received: ");
    Serial.println(received);
    Serial.print("Length: ");
    Serial.println(received.length());
#endif
    char message[received.length() + 5]; //+5???????????
//    char coor[coordinates.length() + 5];
    //            char Sendme[100];
    //    Serial.println("mess recieved");

    received.toCharArray(message, received.length() + 5); //takes received and turns it into a char array message
//    coordinates.toCharArray(coor, coordinates.length() + 5);
    Packet msg(message);    //initializes MessageBlock which will break up the message into its components
#ifdef DEBUG
    Serial.println("");
    Serial.print("Received: ");
    Serial.println(received);
    Serial.print("Length: ");
    Serial.println(received.length());
#endif
    Serial.print("received: ");
    Serial.println(msg.Message);
    return msg;

}
void Send(const char Sendme[], Adafruit_WINC1500Client& client)
{
//    Sendme.Write(Response);
    Serial.print("Message we are sending: ");
    Serial.println(Sendme);
    client.print(Sendme);         //sending the message
}

