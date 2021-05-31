#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "nodemcurobot1.firebaseio.com"
#define FIREBASE_AUTH "dSJKXkHYMwlyBLlaCr0n88Z0vu53MUMqKUhJRoYR"
#define WIFI_SSID "AndroidAP"
#define WIFI_PASSWORD "00000000"

int target;
int direct;
int finaltarget;
#define RST_PIN         5           // Configurable, see typical pin layout above
#define SS_PIN          4          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;

void setup() {
    Serial.begin(115200); // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    
    //byte trailerBlock   = 7;
    uint8_t pageAddr;
    byte buffer[18];
    byte size = sizeof(buffer);
    MFRC522::StatusCode status;
    target = Firebase.getInt("target");// target table total 6 for now
    finaltarget = target;
    Serial.println(target);
    if(target<=3){
      pageAddr = 0x04;
      finaltarget = target;
    }
    else{
      pageAddr = 0x05;
      finaltarget = target - 4;
    }
    //data in 4 block is readed at once.
    
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(pageAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.println();
  direct=buffer[finaltarget];
  Serial.println("Direction");
  Serial.println(direct);
  Firebase.setInt("direct", direct);
  if(direct == 0)
  {
    delay(10000);
    Firebase.setInt("target", 0);
  }

   /*// Authenticate using key A
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Firebase.setInt("direct", 4);
    }
    // Read data from the block
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Firebase.setInt("direct", 4);
    }
    Serial.println("data Readen");
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();*/

    delay(2000);  
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}
