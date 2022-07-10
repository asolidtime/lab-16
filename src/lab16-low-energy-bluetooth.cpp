/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/maxwell/CTD-IoT/d9/lab16-low-energy-bluetooth/src/lab16-low-energy-bluetooth.ino"
/*
 * Project lab16-low-energy-bluetooth
 * Description:
 * Author:
 * Date:
 */
#include "oled-wing-adafruit.h"
void setup();
void loop();
#line 8 "/home/maxwell/CTD-IoT/d9/lab16-low-energy-bluetooth/src/lab16-low-energy-bluetooth.ino"
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);
const size_t UART_TX_BUF_SIZE = 20;
unsigned long long lastTime = 0;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

OledWingAdafruit display;
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  if (String(data[0]).equals("49")) { // i could probably ditch string conversion but this works and honestly I'm tired of doing this lab lol
    display.fillScreen(WHITE);
    display.setCursor(0, 0);
    display.setTextColor(BLACK);
    display.setTextSize(4);
    display.println("Hell");
    display.display();
  } else if (String(data[0]).equals("48")) {
    BLE.disconnect();
  }
}

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  BLE.on();
  display.setup();
  //in setup
  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);

  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  data.appendLocalName("maximus");  //max 8 characters 
  BLE.advertise(&data);
  uint8_t txBuf[10]; // an array of 10 bytes we can send
  txCharacteristic.setValue(txBuf, 10);  // here we are sending all 10 bytes
  
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  display.loop();
  if (millis() - 1000 >= lastTime) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    if (BLE.connected()) {
      RGB.color(RGB_COLOR_BLUE);
      display.println("connected");
    	uint8_t txBuf[UART_TX_BUF_SIZE];
      String message = "Hello!\n";
      message.toCharArray((char *)txBuf, message.length() + 1);
      txCharacteristic.setValue(txBuf, message.length() + 1); 
    } else {
      RGB.color(RGB_COLOR_YELLOW);
      display.println("advertising");
    }
    display.display();
    lastTime = millis();
  }
}