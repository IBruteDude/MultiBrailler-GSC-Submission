#include <WiFiMulti.h>
#include <HTTPClient.h>


#define SSID "KokoSoso"
#define PASS "84840990"


WiFiMulti wifi;
String tts_url = "http://api.voicerss.org/?key=800f26905d24467c99f8a7fbb3eb0ea2&hl=en-us&c=WAV&f=16khz_16bit_mono&src=Bismillah";


void wifi_test(void) {
    if((wifi.run() == WL_CONNECTED)) {
        HTTPClient http;

        Serial.print("[HTTP] begin...\n");

        http.begin(tts_url); //HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] Response code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.write(payload.c_str(), payload.length());
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
}



