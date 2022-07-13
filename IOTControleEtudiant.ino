#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(0,1,2,5,8,6);
#include <Servo.h>  // on inclut la bibliothèque pour piloter un servomoteur
Servo monServo;  // on crée l'objet monServo



// ecrire ici le numero UID du bon badge
const byte bonUID[4] = {19, 65, 219, 11};
const int pinRST = 9;  // pin RST du module RC522
const int pinSDA = 10; // pin SDA du module RC522
MFRC522 rfid(pinSDA, pinRST);


void setup() {
  // put your setup code here, to run once:
SPI.begin();
monServo.attach(6);
  rfid.PCD_Init();
  lcd.begin (16,2);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
   pinMode(7, OUTPUT);
   Serial.begin(9600);
   //lcd.setCursor(5,0);
   lcd.print("bonjour");

}

void loop() {
  // put your main code here, to run repeatedly:
  int refus = 0; // quand cette variable n'est pas nulle, c'est que le code est refusé
  if (rfid.PICC_IsNewCardPresent())  // on a dédecté un tag
  {
    if (rfid.PICC_ReadCardSerial())  // on a lu avec succès son contenu
    {
      for (byte i = 0; i < rfid.uid.size; i++) // comparaison avec le bon UID
      {
        if (rfid.uid.uidByte[i] != bonUID[i]) {
          refus++;
        }
      }
      if (refus == 0) // UID accepté
      {
        // allume LED verte et active relais pendant 3 secondes
        digitalWrite(3, HIGH);
        digitalWrite(7, LOW);
        //delay(1000);
        digitalWrite(4, LOW);
        Serial.println("carte accepter" );
    for (int position = 0; position <=180; position ++){ // on crée une variable position qui prend des valeurs entre 0 à 180 degrés
        monServo.write(position);  // le bras du servomoteur prend la position de la variable position
        delay(15);  // on attend 15 millisecondes
    }

    for (int position = 180; position >=0; position --){ // cette fois la variable position passe de 180 à 0°
        monServo.write(position);  // le bras du servomoteur prend la position de la variable position
        delay(15);  // le bras du servomoteur prend la position de la variable position
    }
        
      }
            else   {  // UID refusé
              Serial.println("carte non valide");
        // allume LED rouge et active buzzer pendant 3 secondes
        digitalWrite(4, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(3, LOW);
        
      }
    }
  }


}
