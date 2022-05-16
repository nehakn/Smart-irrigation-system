//for sending user alerts
//const char* GScriptId = "https://script.google.com/macros/s/AKfycbxa6cXfRKpnWZgjLJsb4FR_fDpIMIbr5Waqie_kEzVli5NUNqc/exec";
const char* GScriptId = "https://script.google.com/macros/s/AKfycbxA6KOk9gM-e_fxvCZpM0rYyyjVYGlanNstu8CDz_OFittLbY4/exec";
const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";
const int httpsPort =  443;
HTTPSRedirect alertClient(httpsPort);

void sendValueToGoogle()
//void sendValueToGoogle(String value)
{
  if (!alertClient.connected())
  {
    Serial.println("Connecting to client"); 
    alertClient.connect(host, httpsPort);
  }
  Serial.println("Client connected");
//  alertClient.printRedir(GScriptId, host, googleRedirHost);
  if(!alertClient.printRedir(GScriptId, host, googleRedirHost))
  {
    Serial.println("Trying to write");
  }
  Serial.println("Data written");
}


