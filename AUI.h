#include <unordered_map>
#include <string>

#include "SDutils.h"
#include "BTutils.h"
#include "WiFiutils.h"

#define MB_LOG_PREFIX "[TTS]: "

#include "url_encode.h"

enum TTSSource {
	GOOGLE_API,
	VOICERSS_API,
	OFFLINE_GENERATOR,
};

class AudioGenerator
{
private:
	std::unordered_map<std::string, std::string> _data_cache;
	std::unordered_map<std::string, std::string> _named_cache;
public:
	File generateAudio(const std::string& text, const std::string& cache_name="") {
		std::string filename = "/multibrailler_storage/material/audio/";
		
		if (filename.size() != 0) {
			filename += cache_name;
		} else {
			filename += "braille_";
		  	filename += std::to_string(std::hash<std::string>()(text));
		}

 		filename += "_cache.wav";

		if (_data_cache.count(text) > 0) {
			return SD.open(_data_cache[text].c_str());
		}


		if (wifi.run() == WL_CONNECTED) {
			File f = SD.open(filename.c_str(), "w", true);

			HTTPClient http;
			MB_LOGI("[HTTP] begin...");

			std::string tts_url = "http://api.voicerss.org/?key=800f26905d24467c99f8a7fbb3eb0ea2&hl=en-us&c=WAV&f=16khz_16bit_mono&src=Bismillah%%20And";
			tts_url += url_encode(text);
	        http.begin(tts_url.c_str());

			MB_LOGI("[HTTP] GET...");

        	int httpCode = http.GET();
        	if(httpCode > 0) {
        	    MB_LOGIF("[HTTP] GET... code: %d\n", httpCode);
        	    if(httpCode == HTTP_CODE_OK) {
        	        String payload = http.getString();
        	        f.write(reinterpret_cast<const uint8_t*>(payload.c_str()), payload.length());
					return f;
        	    }
        	} else {
        	    MB_LOGEF("GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
				return File(NULL);
        	}
        	http.end();
		} else {
			// TODO
		}
		return SD.open("/dev/null", "r");
	}
};

class AudioPlayer
{
private:
public:
	bool play(const File &audiofile) {
		
	}

	bool ready() {

	}
};

class TTS
{
private:
	AudioPlayer _target;
	AudioGenerator _engine;
public:
	bool vocalise(const std::string &text) {
		File f = _engine.generateAudio(text);
		_target.play(f);
		f.close();
	}
};

class AUI
{
private:
	TTS _tts;
public:
	void begin() {
		
	}
};
