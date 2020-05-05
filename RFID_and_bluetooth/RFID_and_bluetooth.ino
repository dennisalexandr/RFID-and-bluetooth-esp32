#include "BluetoothSerial.h"
#include <SPI.h>
#include <MFRC522.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#define RST_PIN         22           // Configurable, see typical pin layout above
#define SS_PIN          5          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//setting timer kirim data
unsigned long previousMillis = 0;   
const long interval = 5000;

//data kirim
int count = 0;
//data terima
String pesan="";
void setup() {
  Serial.begin(115200);

  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  SPI.begin();
  mfrc522.PCD_Init();    
  Serial.println(F("Read personal data on a MIFARE PICC:"));  
}

void loop() {
  //cek data pada buffer bluetooth
  if (SerialBT.available()){
    char pesanMasuk = SerialBT.read();
    if (pesanMasuk != '\n'){
      pesan += String(pesanMasuk);}
    else{
      pesan = "";}
    Serial.write(pesanMasuk);  
  }
  //cek pesan yang diterima oleh ESP32
  if (pesan =="bacakartu"){
    //-------------------------------------------
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
      Serial.print("Kartu tidak terdeteksi\n");
      SerialBT.print("Kartu tidak terdeteksi\n");
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
    }   
    
    byte readCard[mfrc522.uid.size] ; // This is our byte array to store UID mind that there are 4 and 7 bytes long UID
    Serial.println("\nScanned PICC's UID:");
    SerialBT.println("\nScanned PICC's UID:");
     for (int i = 0; i < mfrc522.uid.size; i++) {
        readCard[i] = mfrc522.uid.uidByte[i];
        Serial.print(readCard[i], HEX);
        SerialBT.print(readCard[i], HEX);
      }
  }
}
