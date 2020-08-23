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
int hour[50], minutes[50], dmin[50], dsec[50], dmon[50], dtue[50], dwen[50], sthu[50], sfri[50], dsat[50], dsun[50], today;
int rOn = 0, rOf = 0, timeInSec = 0, timeInHour = 0, timeInMin = 0;
unsigned long old_time = 0;
unsigned long new_time, add_time, diff_time;

void CalTime()
{
	if (timeInSec > 59)
	{
		new_time = 0;
		add_time = 0;
		timeInMin++;
	}
	if (timeInMin > 59)
	{
		timeInMin = 0;
		timeInHour++;
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
			tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];
			Serial.println("device connected");
			host = ip4addr_ntoa(&(station.ip));
		}

		devConnected = true;

		break;

	case SYSTEM_EVENT_AP_STADISCONNECTED:
		//xEventGroupSetBits(event_group, STA_DISCONNECTED_BIT);
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
			temp = 0;
			line = client.readStringUntil('\r');
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
				if (line[j] != '[' && line[j] != ']' && line[j] != '.' && line[j] != ',')
				{
					tempReciveData[temp] = line[j];
					//Serial.print(tempReciveData[temp]);
					temp++;
				}
				if (line[j] == '[' || line[j] == ']' || line[j] == '.' || line[j] == ',')
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
			}

			//Serial.println(today);
			//Serial.print(line);
			success = true;
		}
	}
	//whiledisc:
	/*
	if (success == true)
	{
		if (recivedData[1][0] == 'R')
		{
			delay(100);
			diff_time = diff_time + (new_time - old_time);
			old_time = new_time;

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
			add_time = add_time + (new_time - old_time);
			timeInSec = add_time / 1000;
			CalTime();
		}
	}

	new_time = millis();
	*/
}
