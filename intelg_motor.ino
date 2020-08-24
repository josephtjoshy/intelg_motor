#include <WiFi.h>
#include "esp_wifi.h"

WiFiClient client;
bool simplea = false;
bool success = false;

//WiFiServer server(80);
const uint16_t port = 4010;
const char *host = "192.168.4.2"; // ip or dns
String line, dataProcess[50][11];
bool devConnected = false;
char tempReciveData[5000], recivedData[500][10];
int i, j, temp = 0, dataTemp = 0, spaces = 0, k, Rtemp = 0;
int hour[50], minutes[50], dmin[50], dsec[50], dmon[50], dtue[50], dwen[50], dthu[50], dfri[50], dsat[50], dsun[50], today;
int rOn = 0, rOf = 0, timeInHour = 0, timeInMin = 0,timingNo,timingTemp[50],MotorOnMin=0,MotorOnSec=0;
unsigned long old_time = 0,tempTime=0;
unsigned long new_time, add_time, diff_time;
float timeInSec = 0;

void CalTime()
{
	if (timeInSec > 59)
	{
		
		timeInMin++;
		MotorOnMin--;
	}	
	if (timeInHour > 23)
	{
		timeInHour = 0;
		today++;
	}
	if (today > 7)
	{
		today = 1;
	}
	if(MotorOnMin<1)
	{
		MotorOnMin=0;
	}
	if(MotorOnSec<1)
	{
		MotorOnSec=0;
	}
}

void WiFiEvent(WiFiEvent_t event)
{
	switch (event)
	{

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
			tcpip_adapter_sta_info_t station = adapter_sta_list.sta[0];
			Serial.println("device connected");
			host = ip4addr_ntoa(&(station.ip));
		}

		devConnected = true;

		break;

	case SYSTEM_EVENT_AP_STADISCONNECTED:
		//xEventGroupSetBits(event_group, STA_DISCONNECTED_BIT);
		new_time = millis();
		Serial.println("device disconnected");
		devConnected = false;

	default:
		Serial.println("default");
		break;
	}
}

void setup()
{
	// put your setup code here, to run o
	Serial.begin(115200);
	Serial.println("ESP32 Starting");
	WiFi.softAP("Intlq Motor", "123456789");
	WiFi.onEvent(WiFiEvent);
	Serial.print("Waiting for Connections...");
}

void loop()
{

	// put your main code here, to run repeatedly:

	if (devConnected == true)
	{
		client.connect(host, port);
		if (client.available())
		{
			temp = 0,spaces=0;
			line = client.readStringUntil('\r');
			memset(tempReciveData,0,sizeof tempReciveData); 
			memset(recivedData,0,sizeof recivedData); 
			Serial.println(line);
			for (i = 0; line[i] != '\0'; i++)
			{
			}
			for (j = 0; j < i; j++)
			{
				if (line[j] == ' ')
				{
					continue;
				}
				if (line[j] != '[' && line[j] != ']' && line[j] != '.' && line[j] != ',' && line[j]!='-' && line[j]!=':')
				{
					tempReciveData[temp] = line[j];
					//Serial.print(tempReciveData[temp]);
					temp++;
				}
				if (line[j] == '[' || line[j] == ']' || line[j] == '.' || line[j] == ',' || line[j]=='-' || line[j]==':')
				{
					tempReciveData[temp] = ' ';
					//Serial.print(tempReciveData[temp]);
					temp++;
				}
			}
			for (k = 0; k <= temp; k++)
			{
				if (tempReciveData[k] == ' ')
				{
					spaces++;
				}
			}
			//Serial.print("spaces:");
			//Serial.print(spaces);
			dataTemp = 0;
			for (k = 0; k < spaces; k++)
			{

				for (int l = 0; tempReciveData[dataTemp] != ' '; l++, dataTemp++)
				{
					recivedData[k][l] = tempReciveData[dataTemp];
				}
				if (tempReciveData[dataTemp] == ' ')
				{
					dataTemp++;
				}
				Serial.print(recivedData[k]);
				Serial.print(" ");
				Serial.print(k);
				Serial.print(" ");
			}
			Serial.println();

			k = k - 1;
			new_time = millis();

			if (recivedData[1][0] == 'R')
			{
				//Serial.println("Reapeated usage");
				rOn = atoi(recivedData[2]);
				rOf = atoi(recivedData[3]);
			}
			else if (recivedData[1][0] == 'l')
			{
				//Serial.println("hourwise usage");
				today = atoi(recivedData[k]);
				timeInSec=atoi(recivedData[k-2]);
				timeInMin=atoi(recivedData[k-3]);
				timeInHour=atoi(recivedData[k-4]);
				int rem1=timeInHour%10;
				timeInHour=timeInHour/10;
				int rem2=timeInHour%10;
				timeInHour=(rem2*10)+rem1;
				timingNo=(k-8)/11;
				for(int f=0,g=2;f<timingNo;f++,g+=11)
				{
					hour[f]=atoi(recivedData[g]);
					minutes[f]=atoi(recivedData[g+1]);
					dmin[f]=atoi(recivedData[g+2]);
					dsec[f]=atoi(recivedData[g+3]);
					dmon[f]=atoi(recivedData[g+4]);
					dtue[f]=atoi(recivedData[g+5]);
					dwen[f]=atoi(recivedData[g+6]);
					dthu[f]=atoi(recivedData[g+7]);
					dfri[f]=atoi(recivedData[g+8]);
					dsat[f]=atoi(recivedData[g+9]);
					dsun[f]=atoi(recivedData[g+10]);
					timingTemp[f]=1;
				}


				
			}

			//Serial.println(today);
			//Serial.print(line);
			success = true;
		}
	}
	//whiledisc:
	
	if (success == true)
	{
		if (recivedData[1][0] == 'R')
		{
			delay(100);
			diff_time = diff_time + (new_time - old_time);
			old_time = new_time;
			new_time = millis();
			if (diff_time > 1000)
			{
				Rtemp = Rtemp + (diff_time / 1000);
				diff_time = 0;
				if (Rtemp > ((rOn + rOf) - 1))
				{
					Rtemp = 0;
				}
			}
			if (Rtemp <= rOn)
			{
				Serial.print(Rtemp);
				Serial.println("Motor On");
			}
			if (Rtemp > rOn)
			{
				Serial.print(Rtemp);
				Serial.println("Motor off");
			}
		}
		else if (recivedData[1][0] == 'l')
		{
			
			delay(100);
			add_time = add_time + (new_time - old_time);
			tempTime=tempTime+(new_time-old_time);
			if((tempTime/1000)>=1)
			{
				MotorOnSec--;
				tempTime=0;
			}
			old_time = new_time;
			new_time = millis();
			Serial.print("add");
			Serial.println(add_time);
			timeInSec = add_time / 1000;
			timeInMin=(add_time/1000)/60;
			timeInHour=timeInHour+((add_time/1000)/3600);
			if(timeInHour%2==0)
			{
				add_time=0;
			}			
			CalTime();
			Serial.print(timeInHour);
			Serial.print(":");
			Serial.print(timeInMin);
			Serial.print(":");
			Serial.println(timeInSec);
			for(int f=0;f<timingNo;f++)
			{
				if(timeInHour>=hour[f] && timingTemp[f]==1)
				{
					if(timeInMin>minutes[f])
					{
						MotorOnMin=dmin[f];
						MotorOnSec=dsec[f];
						timingTemp[f]=0;
					}
				}
			}
			if(MotorOnSec>0 || MotorOnMin>0)
			{
				Serial.println("Motor On");
			}
			else
			{
				Serial.println("Motor off");
			}
			
		}
	}

	
	
}
