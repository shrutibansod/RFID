/*
    A0= ouside green led
    A1= latch
    A2= inside green led
    A3= red led
    D8= internal switches
*/
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
#define Size            71   // 46//13

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

byte readCard[4];
char* myTags[Size] = {"86A62D1F", "92EACC79", "B2592289", "5B62A349", "9739A649", "358FA649", "E3E0A549", "9649049", "2D2FE63", "ACDCD63", "AACD63", "C554E63", "31B2D179", "7EF4DB79", "2B44D279", "E927CF79", "64E9D179", "768BCF79", "FE1CD179", "5C751979", "AD8A1779", "C692CF79", "A289DB79", "11C1DB79", "A38ADB79", "834ACD79", "5018CD79", "A510CD79", "B28D179", "B411D079", "B9A41679", "AD261879", "9116E63", "152CD63", "AE8F9049", "9DAB9049", "4B32D179", "7ADD179", "408ECE79", "5594CF79", "4CB7CF79", "3D45CE79", "64EAD63", "9839049", "D06ED63", "B43E9049", "7C9FD63", "B8EED66", "CB17EF66", "B2E199", "7B76ED66", "BBCDF866", "8B1DF466", "BB61599", "4A663", "6BB6FE66", "F21C1779", "A1A31979", "CD58CF79", "96BE1879", "53EE1679","4EA7DB79","B3C01879","D3D2CE79","BD2DCD79","3EDED179","11151D79","814FCC79","F13CE79","C7321C79","3310CD79"};
char* myNames[Size] = {"Shruti", "Shubham", "Aamdaar", "Shubham", "Sanket P", "Surabhi", "Sahil T", "Varun", "Krunal", "Shivam", "Richa", "Nishika", "Administrator", "Abhishek", "Simran", "Abhirav", "Amogh", "Anamay", "Anshul", "Chinmay", "Darshana", "Namrata", "Parth", "Pooja", "Pranay", "Prathamesh", "Romesh", "Rushabh", "Rutwik", "Arpit", "Shaunak", "Shripad", "Siddhesh", "Smruti", "Sumedh", "Sumeet", "Varad", "Yash", "Nupura", "Amit", "Sampada","Prajwal","Shubham","Yash","Himanshu","Abhiyash","Chinmay","Prathamesh","Juilee","Prof. H.M. Khare","Prof.G.N.Kotwal","Prof. Patwardhan","Dr.R.M.Jalnekar","ROBOSOFT","ROBOSOFT","Neha Khare","Rahul Bhide","Prof.N.B.Patki","Omkar","Aditya","Jairaj","Mayur","Ankur","ROBOSOFT","ROBOSOFT","Siddesh","Nikhil"};
String tagID = "";
boolean correctTag = false;



void setup() {
  Serial.begin(9600);
  SPI.begin();        // SPI bus
  mfrc522.PCD_Init(); //  MFRC522
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  pinMode(8, INPUT);
  digitalWrite(8, LOW);

  lcd.begin(16, 2);   // LCD screen
  lcd.print("SCAN NOW");
  Serial.println("SCAN NOW");
  delay(1000);
  printNormalModeMessage();

}

void loop() {
  if (digitalRead(8) == HIGH)
  {
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    printNormalModeMessage();
    delay(3000);
  }
  else
  {
    digitalWrite(A1, LOW);
  }
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    digitalWrite(A0, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    digitalWrite(A1, LOW);
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return;
  }
  tagID = "";
  // The MIFARE PICCs that we use have 4 byte UID
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  Serial.println(tagID);
  lcd.print(tagID);
  correctTag = false;

  for (int i = 0; i < Size; i++) {
    if (tagID == myTags[i]) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("----Welcome!----");
      lcd.setCursor(0, 1);
      lcd.print(myNames[i]);
      digitalWrite(A0, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
      digitalWrite(A1, HIGH);
      delay(3000);
      digitalWrite(A1, LOW);
      Serial.println(" Access Granted!");
      delay(1000);
      printNormalModeMessage();
      correctTag = true;
    }
  }
  if (correctTag == false) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Access Denied!");
    digitalWrite(A0, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, HIGH);
    digitalWrite(A1, LOW);
    //digitalWrite(6, LOW);
    delay(1000);
    printNormalModeMessage();
  }
}


void printNormalModeMessage() {
  lcd.clear();
  lcd.print("-Access Control-");
  lcd.setCursor(0, 1);
  lcd.print(" Scan Your Tag!");
}



