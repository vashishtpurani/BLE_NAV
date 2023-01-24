//for wifi
#include <WiFi.h>
#include <WebSocketClient.h>

//for Ble
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define c "client1"
#define b0 "p0gU"
#define b1 "p1gU"
#define b2 "p2gU"
#define b3 "p3gU"
#define LED_BUILTIN 2

// //wifi credentials
// const char* ssid     = "Adorer21";
// const char* password = "Kaizoku_0u_Ni_orewa_Naru";
// char path[] = "/";
// char ip[] = "192.168.1.7";
// int port = 7000 ;


// const char* ssid     = "JAY WIFI";
// const char* password = "jay123456";
// char path[] = "/";
// char ip[] = "192.168.1.8";
// int port = 7000 ;
// int Flag = 0;

const char* ssid     = "Rajesh";
const char* password = "Rp_9898195848?";
char path[] = "/";
char ip[] = "192.168.1.10";
int port = 7000 ;
int Flag = 0;

// const char* ssid     = "erenYeager";
// const char* password = "8490995868";
// char path[] = "/";
// char ip[] = "192.168.170.137";
// int port = 7000 ;
// int Flag = 0;


int r0=-1,r1=-1,r2=-1,r3=-1;

String s;

WebSocketClient webSocketClient;
WiFiClient client;
BLEScan *pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    // Serial.println(advertisedDevice.toString().c_str());
    if(client.connected()){
      Flag = 1;
    }
    if (advertisedDevice.getName() == b0 && Flag == 1)
    {
      r0 = advertisedDevice.getRSSI();
      // Serial.println("{\"beacon\" :"+String(b0)+",\"rssi\" :"+advertisedDevice.getRSSI()+"}");
      //webSocketClient.sendData("{\"beacon\" : \""+String(b0)+"\",\"rssi\" :"+advertisedDevice.getRSSI()+"}");
      // s.concat("{\"beacon\" : \""+String(b0)+"\",\"rssi\" : "+advertisedDevice.getRSSI()+"},");            
    }
    if (advertisedDevice.getName() == b1 && Flag == 1)
    {
      r1 = advertisedDevice.getRSSI();      
      // Serial.println("{\"beacon\" :"+String(b1)+",\"rssi\" :"+advertisedDevice.getRSSI()+"}");
      //webSocketClient.sendData("{\"beacon\" : \""+String(b1)+"\",\"rssi\" :"+advertisedDevice.getRSSI()+"}");     
      // s.concat("{\"beacon\" : \""+String(b1)+"\",\"rssi\" : "+advertisedDevice.getRSSI()+"},");       
    }        
    if (advertisedDevice.getName() == b2 && Flag == 1)
    {
      r2 = advertisedDevice.getRSSI();      
      // Serial.println("{\"beacon\" :"+String(b2)+",\"rssi\" :"+advertisedDevice.getRSSI()+"}");
      //webSocketClient.sendData("{\"beacon\" : \""+String(b2)+"\",\"rssi\" :"+advertisedDevice.getRSSI()+"}");      
      // s.concat("[{\"beacon\" : \""+String(b2)+"\",\"rssi\" : "+advertisedDevice.getRSSI()+"},");                        
    }  
    
    if (advertisedDevice.getName() == b3 && Flag == 1)
    {
      r3 = advertisedDevice.getRSSI();      
      // Serial.println("{\"beacon\" :"+String(b3)+",\"rssi\" :"+advertisedDevice.getRSSI()+"}");
      //webSocketClient.sendData("{\"beacon\" : \""+String(b3)+"\",\"rssi\" :"+advertisedDevice.getRSSI()+"}");
      // s.concat("{\"beacon\" : \""+String(b3)+"\",\"rssi\" : "+advertisedDevice.getRSSI()+"}");      
    }
    // if(client.connected()){
    //   webSocketClient.sendData("["+"{"+"\"beacon\""+":"+String(b0)+","+"\"rssi\""+":"+String(r0)"}"+","+"{"+"\"beacon\""+":"+String(b1)+","+"\"rssi\""+":"+String(r1)"}"+",""{"+"\"beacon\""+":"+String(b2)+","+"\"rssi\""+":"+String(r2)"}"+",""{"+"\"beacon\""+":"+String(b3)+","+"\"rssi\""+":"+String(r3)"}"+","+"]");            
    // }
    // Serial.print("[ { \"beacon\" :" + String(b0) + ", \"rssi\" :" +String(r0) + "},{ \"beacon\" :"+ String(b1) + ", \"rssi\" :" + String(r1) + "},{\"beacon\" :" + String(b2) + ", \"rssi\" :" + String(r2) + "},{ \"beacon\"\ :" + String(b3) + ", \"rssi\" :" + String(r3) + "}]");    
  }
};

void blink(int count){
    for(int i = 0; i < count ; i++ ) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
    delay(1000);
};


void setup() {
  Serial.begin(9600);
  while(!Serial);
  //wifi setup
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("Esp32's new assigned IP address: ");
  Serial.println(WiFi.localIP());

  delay(5000);


  if (client.connect(ip, port)) {
    Serial.println("Connected");
  } else {
      Serial.println("");
      Serial.println("ErrorCode:server connection failed, check credentials!!!");
    while(1) {
      blink(3);
    }
  }


  webSocketClient.path = path;
  webSocketClient.host = "${ip}:${port}";
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while(1) {
      blink(2);
    }
  }
  webSocketClient.sendData("hola amigos p1");

  //ble setup
  Serial.println("Scanning for bles'...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(1000);
  pBLEScan->setWindow(1000); // less or equal setInterval value

}

void loop() {
  BLEScanResults foundDevices = pBLEScan->start(1, false);
  s = "{\"client_1\" : [ {\"beacon\" : \""+String(b0)+"\",\"rssi\" : "+String(r0)+"},{\"beacon\" : \""+String(b1)+"\",\"rssi\" : "+String(r1)+"},{\"beacon\" : \""+String(b2)+"\",\"rssi\" : "+String(r2)+"},{\"beacon\" : \""+String(b3)+"\",\"rssi\" : "+String(r3)+"}]}";  
  webSocketClient.sendData(s);
  Serial.println(s);
  s.clear();
  delay(500);
  String data;
    
  if (client.connected()) {
    webSocketClient.getData(data);
    if (data.length() > 0) {
      if(data == "on") {
        digitalWrite(LED_BUILTIN, HIGH);
        webSocketClient.sendData("The blue led is on");
      } else if ( data == "off" ){
        digitalWrite(LED_BUILTIN, LOW);
        webSocketClient.sendData("The blue led is off");
      } else {
          webSocketClient.sendData("Could not understand command try {on, off}");
      }
      data = "";
    }
    
  } else {
    Serial.println("Client disconnected.");
    while(!client.connect(ip, port)) {
      blink(3);
    }
    webSocketClient.path = path;
    webSocketClient.host = "${ip}:${port}";
    while (!webSocketClient.handshake(client)) {
      blink(2);
    }
  }
  
  // wait to fully let the client disconnect
//  delay(3000);
  
}