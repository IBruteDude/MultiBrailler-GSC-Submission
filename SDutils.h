#include <vector>

#include <SD.h>

#include "MultiBraillerConfig.h"

#define MB_LOG_PREFIX "[SD]: "

namespace MultiBrailler
{


bool sd_init(void) {
    if (!SD.begin()) {
        MB_LOGE("Card Mount Failed");
        return false;
    }

    MB_LOGI("Started Successfully\n\n\n");
    return true;
}


bool sd_test(void) {
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        MB_LOGE("No SD card attached");
        return false;
    }

    MB_LOGI("SD Card Type: ");
    switch (cardType)
    {
    case CARD_MMC:
        Serial.println("MMC");
    case CARD_SD:
        Serial.println("SDSC");
    case CARD_SDHC:
        Serial.println("SDHC");
    default:
        Serial.println("UNKNOWN");
    }

    MB_LOGIF("SD Card Size: %u bytes or %u MB", (unsigned long long)SD.cardSize(), (unsigned long long)SD.cardSize() / 1024 / 1024);
    return true;
}


inline bool check_directory(const char *dirname)
{
    if (!SD.exists(dirname)) {
        if (!SD.mkdir(dirname)) {
            MB_LOGEF("Failed to create directory %s", dirname);
            return false;
        };
    } else {
        File d = SD.open(dirname);
        if (!d.isDirectory()) {
            String newname = dirname;

            newname += "__oldfile";
            if (SD.rename(dirname, newname)) {
                MB_LOGEF("Failed to create directory %s", dirname);
                d.close();
                return false;
            }
        }
        d.close();

        if (!SD.mkdir(dirname)) {
            MB_LOGEF("Failed to create directory %s", dirname);
            return false;
        };
    }
    return true;
}


inline bool check_file(const char *filename)
{
    File f;

    if (!SD.exists(filename)) {
        f = SD.open(filename, "r", true);
        if (!f) {
            MB_LOGEF("Failed to create file %s", filename);
            return false;
        };
    } else {
        f = SD.open(filename);
        if (f.isDirectory()) {
            String newname = filename;

            newname += "__olddir";
            if (SD.rename(filename, newname.c_str())) {
                MB_LOGEF("Failed to create file %s", newname);
                f.close();
                return false;
            }
            f.close();

            f = SD.open(filename, "r", true);
            if (!f) {
                MB_LOGEF("Failed to create file %s", filename);
                return false;
            };
        }
    }
    f.close();
    return true;
}


bool storage_check() {
    if (!check_directory("/multibrailler_storage"))
        return false;

    if (!check_file("/multibrailler_storage/version")) {
        File f = SD.open("/multibrailler_storage/version", "w", true);

        f.printf("%s\n", MB_VERSION);
        if (f.getWriteError() != 0) {
            MB_LOGEF("error writing to file:\n%s", "/multibrailler_storage/version");
            return false;
        }
    }

    if (!check_directory("/multibrailler_storage/cached_words"))
        return false;

    if (!check_file("/multibrailler_storage/cached_words/index.txt"))
        return false;

    if (!check_directory("/multibrailler_storage/cached_labels"))
        return false;

    if (!check_directory("/multibrailler_storage/cached_labels/options"))
        return false;

    if (!check_directory("/multibrailler_storage/cached_labels/titles"))
        return false;

    if (!check_directory("/multibrailler_storage/material"))
        return false;

    if (!check_directory("/multibrailler_storage/material/text"))
        return false;

    if (!check_file("/multibrailler_storage/material/text/index.txt"))
        return false;

    if (!check_directory("/multibrailler_storage/material/audio"))
        return false;

    if (!check_file("/multibrailler_storage/material/audio/index.txt"))
        return false;

    return true;
}


} // MultiBrailler
