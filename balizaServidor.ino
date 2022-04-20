#include <BLEDevice.h> ​
#include <BLEUtils.h> ​
#include <BLEScan.h> ​
#include <BLEAdvertisedDevice.h> ​
#include <WiFi.h> ​

int scanTime = 5; //In seconds ​
BLEScan* pBLEScan; ​

float A = -58; ​
float N = 3; ​
float distancia1=0; ​
float distancia2=0; ​

WiFiServer server(80); ​
const char* ssid = ""; ​
const char* password = ""; ​

TaskHandle_t Task1; ​
 ​
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks { ​
    void onResult(BLEAdvertisedDevice advertisedDevice) { ​
      if(advertisedDevice.getName()=="ESP32-1"){ ​
        int RSSIvar = advertisedDevice.getRSSI(); ​
        String aux=String(advertisedDevice.getRSSI()); ​
        float temp1 = A-RSSIvar; ​
        float temp2 = 10 * 3; ​
        float temp3= temp1/temp2; ​
        distancia1=pow(10,temp3); ​
      } ​

      if(advertisedDevice.getName()=="ESP32-2"){ ​
        float RSSIvar = advertisedDevice.getRSSI(); ​
        String aux=String(advertisedDevice.getRSSI()); ​
        float temp1 = A-RSSIvar; ​
        float temp2 = 10 * 3; ​
        float temp3 = temp1/temp2; ​
        distancia2=pow(10,temp3); ​
      } ​
    } ​
}; ​
void setup() { ​
  WiFi.begin(ssid, password); ​
  while (WiFi.status() != WL_CONNECTED) { ​
    delay(500); ​
  } ​
  // Arrancamos el servidor web ​
  server.begin(); ​
  delay(10000); ​
  //Serial.println(WiFi.localIP()); // Imprimimos la IP local ​
   ​
  BLEDevice::init(""); ​
  pBLEScan = BLEDevice::getScan(); //create new scan ​
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); ​
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster ​
  pBLEScan->setInterval(100); ​
  pBLEScan->setWindow(99);  // less or equal setInterval value ​

  xTaskCreatePinnedToCore( ​
                    Task1code,   /* Task function. */ ​
                    "Task1",     /* name of task. */ ​
                    10000,       /* Stack size of task */ ​
                    NULL,        /* parameter of the task */ ​
                    1,           /* priority of the task */ ​
                    &Task1,      /* Task handle to keep track of created task */ ​
                    0);          /* Núcleo 0 */                   ​
  delay(500); ​
} ​


void Task1code( void * pvParameters ){ ​
  //Serial.print("Task1 running on core "); ​
  //Serial.println(xPortGetCoreID()); ​
  for(;;){ ​
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false); ​
    pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory ​
    delay(2000);  ​
  }  ​
} 
