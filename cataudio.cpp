#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <FS.h>
#include <SD.h>

typedef struct {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
} WAVHeader;

#define WHS sizeof(WAVHeader)

void concatenate_audiofiles(const char** infiles, const char* outfile)
{
    if (infiles == NULL || infiles[0] == NULL)
        return;

    File f = SD.open(infiles[0], "rb");
    File out = SD.open(outfile, "wb", true);

    WAVHeader h1, h2;

    f.read((uint8_t*)&h1, WHS);
    out.write((uint8_t*)&h1, WHS);

    uint8_t buf[1024];
    size_t bytesRead;

    while ((bytesRead = f.read(buf, sizeof(buf))) > 0)
       out.write(buf, bytesRead);

    f.close();

    for (int i = 1; infiles[i] != NULL; i++) {
        f = SD.open(infiles[i], "rb");
        f.read((uint8_t*)&h2, WHS);
        if (
            h1.audioFormat != h2.audioFormat ||
            h1.numChannels != h2.numChannels ||
            h1.sampleRate != h2.sampleRate ||
            h1.bitsPerSample != h2.bitsPerSample
        ) {
            Serial.println("Error: Audio files are not in the same format.");
            f.close();
            out.close();
            return;
        }
        while ((bytesRead = f.read(buf, sizeof(buf))) > 0)
           out.write(buf, bytesRead);
        f.close();
    }

    out.close();

    Serial.println("Audio files concatenated successfully.");
}

int main() {
    const char* infiles[] = { "audio1.wav", "audio2.wav" };
    const char* outfile = "concatenated.wav";

    concatenate_audiofiles(infiles, outfile);

    return 0;
}
