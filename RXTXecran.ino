/*********
modifié a partir des codes d'exemples et des explications trouvé a https://randomnerdtutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/
code d'envoie/reçeption de trame Lora avec rapport sur ecran et liaison COM

*********/

#include <SPI.h>  //bibliotheque I2C et SPI
#include <LoRa.h>  //bibliotheque LoRa https://github.com/sandeepmistry/arduino-LoRa
#include <Wire.h>  //définition des brochages
#include <Adafruit_SSD1306.h> //bibliotheque pour l'affichage

//definition des broches de la carte ESP
#define ss 5
#define rst 14
#define dio0 2

#define SCREEN_ADDRESS 0x3C // 0x3D ou 0x3C suivant soudure jumper sur carte
Adafruit_SSD1306 display(128, 64, &Wire, -1);
//sur esp32 SDA=21 SCL=22  voir https://randomnerdtutorials.com/esp32-esp8266-i2c-lcd-arduino-ide/
//sur esp8266 SDA=4 SCL=5


int counter = 0;
String LoRaData = "";
int LoraRSSI = 0;
int LoRaSNR = 0;

void setup() {
  Serial.begin(115200);

//initialise l'écran
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.clearDisplay();
  display.setCursor(0,0);

  display.print("init module RFM95");
  display.display(); 

  
  //configure le brochage du module lora RFM95
  LoRa.setPins(ss, rst, dio0);
Serial.println("init carte");
  display.println("init carte");
  display.display(); 

  
  //configure la fréquence du module https://www.thethingsnetwork.org/docs/lorawan/frequencies-by-country/
  //433E6 433Mhz 
  //866E6 866Mhz 
  //penser a adapter la taille de l'antenne: 86,3mm pour 868Mhz et 173,1mm pour 433Mhz
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
      display.print(".");
  display.display(); 

    delay(500);
  }

  // le sync word est une sorte d'identifiant de sous réseau, les deux appareils qui souhaitent communiquer doivent uttiliser les mêmes
  // valeurs de 0 à 0xFF
  LoRa.setSyncWord(0x89);
  Serial.println("carte démarré!");



  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Testeur");
  display.setCursor(0,16);
  display.print("LoRa: ok");
  display.display(); 

  
}

void loop() {
  delay(5000);
  
  //lit les pacquet eventuellement reçu
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("Reçoit paquet: ");

    // lit le paquet
    while (LoRa.available()) {
LoRaData = LoRa.readString();
      Serial.println(LoRaData); 
    }

    // lit information sur la qualité du signal
 LoraRSSI = LoRa.packetRssi(); //Received Signal Strength Indicator   https://www.thethingsnetwork.org/docs/lorawan/rssi-and-snr/
 LoRaSNR = LoRa.packetSnr(); //Signal-to-Noise Ratio
    Serial.print(" RSSI:");
    Serial.println(LoraRSSI);  
    Serial.print(" SNR:");
    Serial.println(LoRaSNR);   
  }


  Serial.print("envoie paquet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(counter);
  LoRa.endPacket();


  display.clearDisplay();
  display.setCursor(0,0);
  display.print("TX:");
  display.print(counter);
    display.setCursor(0,16);
   display.print("RX:");
  display.print(LoRaData);
    display.setCursor(0,32);
     display.print("RSSI:");
    display.println(LoraRSSI);
      display.setCursor(0,48);
   display.print("SNR:");
   display.println(LoRaSNR);   
  display.display(); 



  counter++;


  
}
