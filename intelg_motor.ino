#include<WiFi.h>

WiFiClient client;
//WiFiServer server(80);
const uint16_t port = 4010;
const char * host = "192.168.4.2"; // ip or dns
String line, dataProcess[50][11];
bool devConnected = false;
char tempReciveData[5000],recivedData[500][10];

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {

    case SYSTEM_EVENT_AP_START:
      Serial.println("Access point started\n");
      break;

    case SYSTEM_EVENT_AP_STACONNECTED:
      //xEventGroupSetBits(event_group, STA_CONNECTED_BIT);
      Serial.println("device connected");
      devConnected = true;

      break;

    case SYSTEM_EVENT_AP_STADISCONNECTED:
      //xEventGroupSetBits(event_group, STA_DISCONNECTED_BIT);
      Serial.println("device disconnected");
      devConnected = false;
      break;

    default:
      break;
  }
}

void setup() {
  // put your setup code here, to run o
  Serial.begin(115200);
  Serial.println("ESP32 Starting");
  WiFi.softAP("Intlq Motor", "123456789");
  WiFi.onEvent(WiFiEvent);
  Serial.print("Waiting for Connections...");


}

void loop() {
  // put your main code here, to run repeatedly:
  int i,j,temp=0,dataTemp=0,spaces=0;
  char loadchar;
  if (devConnected == true)
  {
    client.connect(host, port);
    if (client.available())
    {tempReciveData[5000]=' ',recivedData[500][10]=' ';
      line = client.readStringUntil('\r');
      for (i = 0; line[i] != '\0'; i++)
      {

      }
      for(j=0;j<i;j++)
      {
        if(line[j]==' ')
        {
          continue;
        }
        if(line[j]!='[' && line[j]!=']' && line[j]!='.' && line[j]!=',')
        {
          tempReciveData[temp]=line[j];
          //Serial.print(tempReciveData[temp]);
          temp++;
        }
        if(line[j]=='[' || line[j]==']' || line[j]=='.' || line[j]==',')
        {
          tempReciveData[temp]=' ';
          //Serial.print(tempReciveData[temp]);
          temp++;
        }
      }
      for(int k=0;k<=temp;k++)
      {
        if(tempReciveData[k]==' ')
        {
          spaces++;
        }
      }
      Serial.print("spaces:");
      Serial.print(spaces);
      dataTemp=0;
      for(int k=0;k<spaces;k++)
      {
    
        for(int l=0;tempReciveData[dataTemp]!=' ';l++,dataTemp++)
        {
          recivedData[k][l]=tempReciveData[dataTemp];
          
        }
        if(tempReciveData[dataTemp]==' ')
        {
          dataTemp++;
        }
        Serial.println(recivedData[k]);
      }
      Serial.println(line);
      
    }
  }

}