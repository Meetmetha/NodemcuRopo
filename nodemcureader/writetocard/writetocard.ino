
  #include <SPI.h>
  #include <MFRC522.h>

//4 is 4123 & 5 is 3214
  #define SS_PIN          4
  #define RST_PIN         5

  MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
  MFRC522::StatusCode status; //variable to get card status

  byte buffer[18];  //data transfer buffer (16+2 bytes data+CRC)
  byte size = sizeof(buffer);

  uint8_t pageAddr = 0x05;  //In this example we will write/read 16 bytes (page 6,7,8 and 9).
                            //Ultraligth mem = 16 pages. 4 bytes per page.
                            //Pages 0 to 4 are for special functions.  
   byte buffr[] = {0x02,0x02,0x00,0x02};
   //writeBytesToBlock(pageAddr, buffr);

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println(F("Sketch has been started!"));
  //memcpy(buffer,"03020104",4);
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;
/*

  // Write data ***********************************************
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Ultralight_Write(pageAddr, buffr, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Read() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
  Serial.println(F("MIFARE_Ultralight_Write() OK "));
  Serial.println();*/



  // Read data ***************************************************
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  mfrc522.PICC_HaltA();

}
