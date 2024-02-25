#ifndef MB_CONFIG
#define MB_CONFIG

#define MB_VERSION "0.0.1"

#define MB_LOG_PREFIX "[SETUP]: "

#ifdef NDEBUG
	#define LOG_STREAM Serial
#else
	#include "SD.h"
	extern File _logfile;
	#define LOG_STREAM _logfile
#endif

#define MB_LOGI(message) \
	LOG_STREAM.println(F(MB_LOG_PREFIX "[INFO]: " message))
#define MB_LOGE(message) \
	LOG_STREAM.println(F(MB_LOG_PREFIX "[ERROR]: " message))

#define MB_LOGIF(format, ...) \
	LOG_STREAM.printf(MB_LOG_PREFIX "[INFO]: " format "\n", __VA_ARGS__)
#define MB_LOGEF(format, ...) \
	LOG_STREAM.printf(MB_LOG_PREFIX "[ERROR]: " format "\n", __VA_ARGS__)

#endif // MB_CONFIG
