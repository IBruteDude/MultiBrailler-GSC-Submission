#include "MultiBraillerConfig.h"

#include <BluetoothSerial.h>

#define MB_LOG_PREFIX "[BT]: "

String btDeviceName = "MultiBrailler";

BluetoothSerial SerialBT;

#define BT_DISCOVER_TIME	10000

static bool btScanAsync = true;
static bool btScanSync = true;


void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
	MB_LOGIF("Found a device asynchronously: %s\n", pDevice->toString().c_str());
}

void bt_test(void) {
    SerialBT.begin(btDeviceName); // Bluetooth device name

    MB_LOGI("The device started, now you can pair it with bluetooth!");

    if (btScanAsync) {
        MB_LOGI("Starting discoverAsync...");
        if (SerialBT.discoverAsync(btAdvertisedDeviceFound)) {
            MB_LOGI("Findings will be reported in \"btAdvertisedDeviceFound\"");
            delay(BT_DISCOVER_TIME);
            MB_LOGI("Stopping discoverAsync... ");
            SerialBT.discoverAsyncStop();
            MB_LOGI("stopped");
        } else {
            MB_LOGE("Error on discoverAsync f.e. not workin after a \"connect\"");
        }
    }

    if (btScanSync) {
        MB_LOGI("Starting discover...");
        BTScanResults *pResults = SerialBT.discover(BT_DISCOVER_TIME);
        if (pResults)
            MB_LOGI(""), pResults->dump(&Serial);
        else
            MB_LOGE("Error on BT Scan, no result!");
    }
}

