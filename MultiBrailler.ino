#define NDEBUG

#include "esp_http_client.h"

#include "WiFiutils.h"
#include "SDutils.h"

#define MB_LOG_PREFIX "[MAIN]: "

using namespace MultiBrailler;

#ifndef NDEBUG
File _logfile;
#endif

void setup() {
    Serial.begin(115200);

    sd_init();

#ifndef NDEBUG
    _logfile = SD.open(F("/multibrailler_storage/logs/log__" __TIME__ "_" __DATE__), "w", true);
#endif

    MB_LOGI("Wait for 5 seconds for wifi device");

    for(uint8_t t = 4; t > 0; t--) {
        MB_LOGIF("Wait for %d seconds...\n", t);
        Serial.flush();
        delay(1000);
    }

    wifi.addAP(SSID, PASS);

    if((wifi.run() == WL_CONNECTED)) {
	    HTTPClient http;
	    MB_LOGI("[HTTP] begin...");

	    String tts_url = F("http://api.voicerss.org/?key=800f26905d24467c99f8a7fbb3eb0ea2&hl=en-us&c=WAV&f=16khz_16bit_mono&src=Bismillah%%20And");
    
        http.begin(tts_url);

	    MB_LOGI("[HTTP] GET...");

        int httpCode = http.GET();

        if (httpCode > 0) {
            MB_LOGIF("[HTTP] GET... code: %d\n", httpCode);

            if (httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                
                size_t l = payload.length();
                const char *data = payload.c_str();

                MB_LOGIF("HTTP Payload length is %d bytes", l);

                for (size_t i = 0; i < l; i++)
                    Serial.print(payload[i]);
            }
        } else {
            MB_LOGEF("GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    } else {
        MB_LOGE("WIFI didn't connect");
    }
}




void loop() {
    delay(3000);
}


