#include <Arduino.h>
#include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------

#define Rssid "your_ssid"
#define Rpass "your_password"
#define MAX_CLIENTS 10
#define MAX_LINE_LENGTH 50

//ip address 
IPAddress IP(172,20,10,2);
IPAddress gateway(172,20,0,1);  
IPAddress subnet(255,255,240,0);
WiFiServer server(80);
//WiFiServer server(9001);
WiFiClient *clients[MAX_CLIENTS] = { NULL };
char inputs[MAX_CLIENTS][MAX_LINE_LENGTH] = { 0 };
# define MotorPin_1 D1
# define MotorPin_2 D2
# define MotorPin_3 D3
# define MotorPin_4 D4
int id;
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void SendToRelay(int duration)
{  
    Serial.println("\nIn motor control module\n");
    if(id==1321599)  
    {    
        digitalWrite(MotorPin_1, duration);  
        // Serial.println(MotorPin_1);
    }
}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
bool CheckForClient()
{
//    Serial.println("Waiting for USP client"); 
//    delay(500);
    WiFiClient newClient = server.available();  
    if (newClient) 
    {
      Serial.println("\nNew client");
//    Serial.println("\nNew client:" + String(newClient));
    for (int i=0 ; i<MAX_CLIENTS ; ++i) 
    {
        if (NULL == clients[i]) 
        {
            clients[i] = new WiFiClient(newClient);
            break;
        }
        }
    }
    for (int i=0 ; i<MAX_CLIENTS ; ++i)
    {
        /* If the client is in use, and has some data */
        if (NULL != clients[i] && clients[i]->available()) 
        {
            /* whole string */
            String newChar = clients[i]->readStringUntil('\r');
//            Serial.print("Received data: ");
//            Serial.println(newChar);
            id = (newChar.substring(0, newChar.length()-1).toInt());
            Serial.print("Receiver id: ");
            Serial.println(id);
            /* to motor */
            int duration = (newChar.substring(newChar.length()-1,newChar.length())).toInt();
            Serial.print("Motor input: ");
            Serial.println(duration);
            SendToRelay(!duration);
            /* clients cancellation */
            clients[i]->flush();
            clients[i]->stop();
            delete clients[i];
            clients[i] = NULL;  
            Serial.println("Back to wait for client");
        }   
    }
}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void setup() 
{
    Serial.begin(115200);
    delay(10);
    Serial.println("\n\nIrrigation Unit");
    Serial.print("Id: ");
    int IUid = ESP.getChipId();
    Serial.println(IUid);
    pinMode(MotorPin_1, OUTPUT);
    pinMode(MotorPin_2, OUTPUT);
    pinMode(MotorPin_3, OUTPUT);
    pinMode(MotorPin_4, OUTPUT);
    digitalWrite(MotorPin_1, 1);
    digitalWrite(MotorPin_2, 1);
    digitalWrite(MotorPin_3, 1);
    digitalWrite(MotorPin_4, 1);
    //WiFi.disconnect();
//    WiFi.config(IP,gateway,subnet);
//    WiFi.config();
    WiFi.begin(Rssid, Rpass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println(".");
    }
    Serial.print("Connected to: ");
    Serial.println(Rssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    
//     WiFi.mode(WIFI_AP_STA);
//     Serial.println("\nWIFI Mode : AccessPoint Station"); 
//    WiFi.softAP(ssid, password);
//     Serial.println("WIFI < " + String(ssid) + " > ... Started");
//     IPAddress IP = WiFi.softAPIP();
//     Serial.print("AccessPoint IP : ");
//     Serial.println(IP);
//     Serial.print("AccessPoint MC : ");
//     Serial.println(String(WiFi.softAPmacAddress()));
    
    server.begin();
    server.setNoDelay(true);
    Serial.println("Server Started\n");
}
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void loop() 
{
    CheckForClient();
}
//------------------------------------------------------------------------------------
