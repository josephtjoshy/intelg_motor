#include<WiFi.h>
#include "esp_wifi.h"
//#include<ip4_addr.h>
#include "lwip/opt.h"
#include "lwip/def.h"


WiFiClient client;
bool simplea=false;
 

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
      
      delay(300);
      wifi_sta_list_t wifi_sta_list;
      tcpip_adapter_sta_list_t adapter_sta_list;
      memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
      memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
      
      esp_wifi_ap_get_sta_list(&wifi_sta_list);
      tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);
      for (int i = 0; i < adapter_sta_list.num; i++) 
      {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
        Serial.println("device connected");
        host=ip4addr_ntoa(&(station.ip)); 
      }
      
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
