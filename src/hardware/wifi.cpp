#include "hardware/wifi.h"
#include <WiFi.h>
#include "hardware/display.h"
#include <ESPAsyncWebServer.h>
#include "html/config.h"
#include <LittleFS.h>
#include "math/config.h"
#include "math/time.h"
#include <Arduino.h>
#include "hardware/display.h"
#include "display/qr.h"
#include "display/text.h"
#include "display/alignment.h"
#include <WiFiMulti.h>
#include <vector>
#include "hardware/rtc.h"
#include <WatchyRTC.h>
#include <HTTPClient.h>

static AsyncWebServer server(80);
WiFiMulti wifimulti;

File file;
size_t cur_pos = 0;
const String charset = "abcdefghijklmnopqrstovwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0124589";

void setupAP()
{
    JSONVar cfg = parseCategory("AP Settings", config);
    Serial.println(JSON.typeof(cfg));
    Serial.println(JSON.typeof(cfg["Password"]));
    String password;
    RecursiveMultipleChoiceReturn r = parseRecursiveMultipleChoice("Password", cfg);
    if (r.type == "custom")
    {
        password = parseText("Password", r.value);
    }
    else
    {
        password = "";
        int length = 32;
        for (int i = 0; i < length; i++)
        {
            password += charset[random(0, charset.length())];
        }
    }
    String ssid = parseText("SSID", cfg);
    bool hidden = parseCheckbox("Hidden", cfg);
    String hiddenstr = hidden ? ";H:true" : "";

    if (ssid == "")
    {
        ssid = "WatchyAP";
    }

    dp.setFullWindow();
    dp.fillScreen(bg_color);

    drawQr("WIFI:T:WPA;S:" + ssid + ";P:" + password + hiddenstr + ";;", 0, 0, 200, 200, TD_CENTER_CENTER);

    dp.display(false);

    dp.epd2.setBusyCallback(0);
    WiFi.persistent(false); // Don’t write to NVS
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.setSleep(false);
    if (WiFi.softAP(ssid, password, 1, hidden))
    {
        Serial.println("AP Started");
    }
    else
    {
        Serial.println("AP Failed");
    }
    IPAddress IP = WiFi.softAPIP();
    Serial.print("IP Address: ");
    Serial.println(IP);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/html", index_html); });
    server.on("/config_post", HTTP_POST, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "OK"); },
              nullptr, // Do not need this callback for chunk processing
              [](AsyncWebServerRequest *request, const uint8_t *data, size_t len, size_t index, size_t total)
              {
        if(index==0){
            if (cur_pos!=0){
                file.close();
                cur_pos=0;
            }
            file = LittleFS.open("/config.json", "w");
        }
        if (cur_pos==index){
            file.write(data, len);
            cur_pos+=len;
            if (cur_pos==total){
                file.close();
                cur_pos=0;
            }
        } });
    server.on("/config_get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
    File file = LittleFS.open("/config.json", "r");
    if (!file||!file.available())
    {
      request->send(404, "text/plain", "File Not Found");
      return;
    }
    size_t length=file.size();
    request->send("text/plain",length,[file=std::move(file),length](uint8_t* buffer, size_t maxLen, size_t index) mutable -> size_t {
      size_t l=file.readBytes((char*)buffer, maxLen);
      if(index+l==length){
        file.close();
      }
      return l;
    }); });
    server.begin();
}
void stopAP()
{
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
    dp.epd2.setBusyCallback(WatchyDisplay::busyCallback);
    server.end();
    file.close();
}
void connectWifi()
{
    JSONVar cfg = parseCategory("WiFi Settings", config);
    Serial.println(JSON.stringify(cfg));
    WiFi.mode(WIFI_STA);
    //  <experimental>
    WiFi.setSleep(false);                // Disable power save
    WiFi.setTxPower(WIFI_POWER_19_5dBm); // Set the highest transmit power
    WiFi.setAutoReconnect(true);         // Automatically reconnect to Wi-Fi
    //  </experimental>
    std::vector<JSONVar> networks = parseList("APs", cfg);
    for (int i = 0; i < networks.size(); i++)
    {
        String ssid = parseText("SSID", networks[i]);
        String password = parseText("Password", networks[i]);
        Serial.print("Adding AP: ");
        Serial.println(ssid);
        wifimulti.addAP(ssid.c_str(), password.c_str());
    }
}
void waitForConnection()
{
    Serial.print("Connecting to WiFi");
    int count = 0;
    while (wifimulti.run() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        if (count++ > 5)
        {
            Serial.println("Failed to connect to WiFi");
            return;
        }
    }
    Serial.println();
    Serial.print("Connected to WiFi. IP: ");
    Serial.println(WiFi.localIP());
}
void disconnectWifi()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}
RTC_SLOW_ATTR SimpleTime lastNtpUpdate;
void updateRtc()
{
    SimpleTime bevore=simpleNow();
    configTime(0, 0, "pool.ntp.org");

    // Wait until time is retrieved
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }

    // Print debug info
    Serial.printf("NTP Time: %02d:%02d:%02d %04d-%02d-%02d\n",
                  timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
                  timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);

    // Convert to RTC_DATA_STRUCT
    tmElements_t rtcData;
    rtcData.Hour = timeinfo.tm_hour;
    rtcData.Minute = timeinfo.tm_min;
    rtcData.Second = timeinfo.tm_sec;
    rtcData.Day = timeinfo.tm_mday;
    rtcData.Month = timeinfo.tm_mon + 1;
    rtcData.Year = timeinfo.tm_year + 1900 - 1970;

    // Set Watchy RTC
    RTC.set(rtcData);
    SimpleTime after=simpleNow();
    int diff=(after.hour*3600+after.minute*60+after.second)-(bevore.hour*3600+bevore.minute*60+bevore.second);
    int diff2=(bevore.hour*3600+bevore.minute*60+bevore.second)-(lastNtpUpdate.hour*3600+lastNtpUpdate.minute*60+lastNtpUpdate.second);
    lastNtpUpdate=after;
    dp.setFullWindow();
    dp.fillScreen(bg_color);
    textBox("RTC update\nsuccess!\ncorrected by\n"+String(diff)+"s\nin\n"+String(diff2)+"s\nthat's\n"+String(float(diff)/float(diff2))+"s/s", 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, TD_CENTER_CENTER, __null, 2, fg_color);
    dp.display(false);
}
bool updateTimeT()
{
    JSONVar cfg = parseCategory("timetable", config);
    String url = parseText("url", cfg);
    Serial.println("1");
    if (url == "")
    return false;
    WiFiClientSecure *client = new WiFiClientSecure;
    client->setInsecure();
    Serial.println("2");
    if (!client)
    return false;
    client->setInsecure();
    HTTPClient https;
    Serial.println("3");
    if (!https.begin(*client, url))
    return false;
    Serial.println("4");
    if (https.GET() != HTTP_CODE_OK)
    return false;
    File file = LittleFS.open("/timet", "w", true);
    Serial.println("5");
    if (!file)
    return false;
    Serial.println("6");
    https.writeToStream(&file);
    https.end();
    return true;
}
String https(String url)
{
    WiFiClientSecure *client = new WiFiClientSecure;
    if (client)
    {
        client->setInsecure();
        HTTPClient https;
        if (https.begin(*client, url))
        {
            int httpCode = https.GET();
            if (httpCode > 0)
            {
                if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
                {
                    return https.getString();
                }
            }
            else
            {
                // Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
            }
            https.end();
        }
    }
    else
    {
        // Serial.printf("[HTTPS] Unable to connect\n");
    }
    return "";
}