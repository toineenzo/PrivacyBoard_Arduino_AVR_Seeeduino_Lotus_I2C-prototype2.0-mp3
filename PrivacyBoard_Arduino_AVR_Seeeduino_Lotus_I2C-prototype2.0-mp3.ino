#include <LiquidCrystal_I2C.h>           //importeer LCD library
#include <Wire.h>                        //importeer de kabel library tussen de LCD en arduino (i2c converter)
LiquidCrystal_I2C lcd(0x27, 2, 16);      //geef aan wat voor een soort lcd het is
//=====================================================
#define button1 2                        //knop 1 zit op pin 2
#define button2 3                        //knop 1 zit op pin 3
#define buzzer 6                        //buzzer zit op pin 6

int spelmodus = 0;                        //ff wat variabele aanmakon
int dobbelgetal = 0;
int vraag = 0;
int vraagstelling = 0;
//=====================================================
#include <SoftwareSerial.h>
SoftwareSerial mp3(7, 8);

static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
//=====================================================
// Dit moet je doen voor deze functie om text op het lced schermpje te laten zien. vet handig.
void displayLCD( char* line1, char* line2) {    //De variabele doet het volgende
  lcd.clear();                                  //scherm leeghalen
  lcd.setCursor(0, 0);                          //cursor plaatsen op 1e blokje, 1e regel
  lcd.print(line1);                             //print 1e tekst op 1e regel
  lcd.setCursor(0, 1);                          //cursor plaatsen op 1e blokje, 2e regel
  lcd.print(line2);                             //print 2e tekst op 1e regel
}


void resetMP3() {
  Send_buf[0] = 0x7e;   // Open command
  Send_buf[1] = 0xff;   // Version
  Send_buf[2] = 0x06;   // Length
  Send_buf[3] = 0x0C;//yes
  Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(0x00 >> 8);  //datah
  Send_buf[6] = (int8_t)(0x00);       //datal
  Send_buf[7] = 0xef;   //
  for (uint8_t i = 0; i < 8; i++)  {
    mp3.write(Send_buf[i]) ;
  }
}

void setup() {
  mp3.begin(9600); //connect to mp3
  resetMP3(); //reset de mp3 speler, voor als er misschien nog muziek afspeelt ofzo vet irritant is dat als dat zo is
  //=====================================================
  pinMode(button1, INPUT);               //knop 1 is een input
  pinMode(buzzer, OUTPUT);               //Buzzer is een output
  //=====================================================
  lcd.init();                            //start lcd opnieuw op
  lcd.backlight();                       //zet backlight van lcd aan
  lcd.clear();                           //maak lcd scherm leeg
  lcd.begin(16, 2);                      //stel het aantal rijen en kolommen in op de lcd die je gaat gebruiken, in dit geval alles
  //=====================================================
  Send_buf[0] = 0x7e;   // Open command
  Send_buf[1] = 0xff;   // Version
  Send_buf[2] = 0x06;   // Length
  Send_buf[3] = 0x0F;//yes
  Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(0x0B01 >> 8);  //datah
  Send_buf[6] = (int8_t)(0x0B01);       //datal
  Send_buf[7] = 0xef;   //
  for (uint8_t i = 0; i < 8; i++)  {
    mp3.write(Send_buf[i]) ;
  }
  displayLCD("###Welkom bij###", "##PrivacyBoard##"); //titelscherm
  tone(buzzer, 3500);
  delay(300);
  noTone(buzzer);
  tone(buzzer, 4000);
  delay(350);
  noTone(buzzer);
  tone(buzzer, 4500);
  delay(250);
  noTone(buzzer);
  delay(1500);
  displayLCD("Gemaakt door:", "Boudewijn &Toine");
  delay(1500);
  displayLCD("Klas JDE-D04", "HBO CMD HU");
  delay(1500);
  displayLCD("Het spel waar je", "leert over je...");
  delay(2000);
  displayLCD("digital privacy!", "Wil je uitleg?");
  delay(2000);
  displayLCD("Knop 1 = Uitleg", "Knop 2=Sla over");
  lcd.blink();                                              //wait en show blink
}

void loop() {
  randomSeed(analogRead(0));                                //HOU DIE RANDOM FUNCTIE NOU VERDOMME RANDOM!!!!!!!!!!

  if (digitalRead(button1) == HIGH) {
    if (spelmodus == 0) {
      tone(buzzer, 2600);               //Laat piepje horen van 2600Hz
      delay(200);                       //voor 200ms
      noTone(buzzer);                   //zet dat stomme piepje uit pls
      lcd.noBlink();                     //set blinking cursor uit
      lcd.clear();                       //leeg scherm voordat de button 1 aan gaat
      spelmodus = 1;                     //Als knop wordt ingedrukt, zet hem aan
    }

    else {
      spelmodus == 0;                    //Als knop niet wordt ingedrukt, houd het uit
    }

    delay(250);                          //Even wachten totdat je verdergaat
  }


  if (digitalRead(button2) == HIGH) {    //doe skippen naar direct start spel
    if (spelmodus == 0) {
      tone(buzzer, 2600);
      delay(200);
      noTone(buzzer);
      lcd.noBlink();                     //set blinking cursor uit
      lcd.clear();                       //leeg scherm voordat de button 1 aan gaat
      spelmodus = 2;                     //ga naar dobbel/vraag scherm
    }

    else {
      spelmodus == 0;                    //Als knop niet wordt ingedrukt, houd het uit
    }
    delay(250);                          //Even wachten totdat je verdergaat
  }

  if (spelmodus == 1) {                                 //UITLEG SCHERM
    displayLCD("Max. 4 spelers.", "DOEL: als eerste");
    delay(3000);
    displayLCD("op vakje 63", "terecht komen.");
    delay(3000);
    displayLCD("==Blauwe vakje==", "Vraag (Knop 2)");
    delay(3000);
    displayLCD("===Rood vakje===", "Speciaal vakje");
    delay(3000);
    displayLCD("==Groen vakje==", "Dobbel nog eens!");
    delay(3000);
    tone(buzzer, 1000);
    delay(200);
    noTone(buzzer);
    tone(buzzer, 200);
    delay(500);
    noTone(buzzer);
    lcd.clear();
    spelmodus = 2;                        //ga naar dobbel/vraag scherm
  }

  if (spelmodus == 2) {                 // VRAAG DOBBEL MAINSCREEN - IDLE
    playMusic();

    lcd.setCursor(0, 0);
    lcd.print(F("Knop 1 = DOBBEL"));
    lcd.setCursor(0, 1);
    lcd.print(F("Knop 2 = VRAAG "));
    lcd.blink();
    while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
      //doe niks
    }
  }


  if (digitalRead(button1) == HIGH) {
    if (spelmodus == 2) {               //ja ga maar door als je vanuit het main menu komt (dobbel.vraag)
      lcd.noBlink();
      lcd.setCursor(0, 0);
      lcd.clear();
      dobbelgetal = random(1, 7);     //genereer dobbelgetal
      tone(buzzer, 1000);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "5456132156464948");     //coole dobbel effectjes
      tone(buzzer, 1200);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "6541986451986414");
      tone(buzzer, 1400);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "8997231987958676");
      tone(buzzer, 1600);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "1458922858882478");
      tone(buzzer, 1800);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "3298498577909878");
      tone(buzzer, 2000);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "2348089948784998");
      tone(buzzer, 2200);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "3457869832748758");
      tone(buzzer, 2400);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "2359847902000287");
      delay(100);
      tone(buzzer, 2600);
      delay(100);
      noTone(buzzer);
      displayLCD("Je gooide...", "2384576879035967");
      delay(100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Je gooide..."));
      lcd.setCursor(7, 1);                      //plaats getal in het midden van het scherm
      lcd.print(dobbelgetal);                 //dit heb je gegooid, laat zien
      lcd.setCursor(0, 0);
      tone(buzzer, 3000);
      delay(100);
      noTone(buzzer);
      delay(100);
      tone(buzzer, 4000);
      delay(100);
      noTone(buzzer);
      delay(1500);
      playMusic();

      spelmodus = 2;
    }

    else {
      spelmodus == 2;               //Als knop niet wordt ingedrukt, houd het uit
    }
    delay(250);                         //Even wachten totdat je verdergaat
  }



  if (digitalRead(button2) == HIGH) {       //geef vraag
    if (spelmodus == 2) {               //als je je in dobbel/vraag menu bevind, doe dan:....
      lcd.noBlink();
      tone(buzzer, 2600);
      delay(200);
      noTone(buzzer);
      spelmodus = 10;                //Ga naar vragencyclus

    } else {
      spelmodus == 2;               //Als knop niet wordt ingedrukt, houd het uit

    }
    delay(250);                         //Even wachten totdat je verdergaat
  }




  if (spelmodus == 10) {
    vraag = random(1, 11);


    //=================vraag 1================================

    if (vraag == 1) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0x0F;//yes
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0A01 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0A01);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 1;

      if (vraagstelling == 1) {
        displayLCD("Knop 1 = WAAR", "Knop 2 = ONWAAR");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0x0F;//yes
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0102 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0102);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }
          vraagFout();
          delay(10000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0x0F;//yes
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0102 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0102);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }
          vraagGoed();
          delay(10000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen
        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }



    //=====================vraag 2=================================================================


    if (vraag == 2) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0201 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0201);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 2;

      if (vraagstelling == 2) {
        displayLCD("Knop 1 = A", "Knop 2 = B");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0202 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0202);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(15000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0202 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0202);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(15000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }



    //=====================vraag 3=================================================================


    if (vraag == 3) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0301 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0301);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 3;

      if (vraagstelling == 3) {
        displayLCD("Knop 1 = WAAR", "Knop 2 = ONWAAR");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0302 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0302);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(12000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0302 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0302);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(12000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }




    //====================vraag 4==================================================================


    if (vraag == 4) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0401 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0401);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 4;

      if (vraagstelling == 4) {
        displayLCD("Knop 1 = WAAR", "Knop 2 = ONWAAR");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0402 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0402);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(12000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0402 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0402);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(12000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }




    //===================vraag 5===================================================================


    if (vraag == 5) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0501 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0501);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 5;

      if (vraagstelling == 5) {
        displayLCD("Knop 1 = WAAR", "Knop 2 = ONWAAR");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0502 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0502);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(15000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0502 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0502);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(15000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }





    //==============vraag 6========================================================================


    if (vraag == 6) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0601 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0601);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 6;

      if (vraagstelling == 6) {
        displayLCD("Knop 1 = WAAR", "Knop 2 = ONWAAR");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0602 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0602);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(15000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0602 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0602);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(15000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }




    //============vraag 7==========================================================================


    if (vraag == 7) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0701 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0701);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 7;

      if (vraagstelling == 7) {
        displayLCD("Knop 1 = WAAR", "Knop 2 = ONWAAR");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0702 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0702);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(10000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0702 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0702);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(10000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }





    //============vraag 8==========================================================================


    if (vraag == 8) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0801 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0801);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 8;

      if (vraagstelling == 8) {
        displayLCD("Knop 1 = MEER", "Knop 2 = MINDER");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0802 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0802);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(12000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0802 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0802);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(12000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }




    //==============vraag 9========================================================================


    if (vraag == 9) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0901 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0901);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 9;

      if (vraagstelling == 9) {
        displayLCD("Knop 1 = WAAR", "Knop 2 = ONWAAR");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0902 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0902);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(15000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0902 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0902);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(15000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }






    //================vraag 10======================================================================


    if (vraag == 10) {
      vraagGenerate();    //ff spannend maken
      Send_buf[0] = 0x7e;   // Open command
      Send_buf[1] = 0xff;   // Version
      Send_buf[2] = 0x06;   // Length
      Send_buf[3] = 0X0F;//
      Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
      Send_buf[5] = (int8_t)(0x0A01 >> 8);  //datah
      Send_buf[6] = (int8_t)(0x0A01);       //datal
      Send_buf[7] = 0xef;   //
      for (uint8_t i = 0; i < 8; i++)  {
        mp3.write(Send_buf[i]) ;
      }
      vraagstelling = 10;

      if (vraagstelling == 10) {
        displayLCD("Knop 1 = WAAR", "Knop 2 = ONWAAR");

        while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) { //Zolang de knoppen niet worden ingedrukt, wacht dan pls
          //doe niks
        }

        if (digitalRead(button1) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0A02 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0A02);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagFout();
          delay(12000);
          vraagBack();                                              //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        if (digitalRead(button2) == HIGH) {
          Send_buf[0] = 0x7e;   // Open command
          Send_buf[1] = 0xff;   // Version
          Send_buf[2] = 0x06;   // Length
          Send_buf[3] = 0X0F;//
          Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
          Send_buf[5] = (int8_t)(0x0A02 >> 8);  //datah
          Send_buf[6] = (int8_t)(0x0A02);       //datal
          Send_buf[7] = 0xef;   //
          for (uint8_t i = 0; i < 8; i++)  {
            mp3.write(Send_buf[i]) ;
          }

          vraagGoed();
          delay(12000);
          vraagBack();          //ga terug naar DOBBEL / VRAAG MENU en laat piepje horen

        }

        else {
          //idle
        }
        delay(250);                         //Even wachten totdat je verdergaat
      }
    }
    //====================einde vragen===========================================================


  }//end vragenmodus


} //end loop





void vraagGoed() {                          //Dit doe je als je de vraag goed hebt beantwoord
  displayLCD("Je antwoord is...", "");
  tone(buzzer, 3000);
  delay(100);
  tone(buzzer, 3500);
  delay(100);
  tone(buzzer, 4000);
  delay(100);
  tone(buzzer, 4500);
  delay(100);
  tone(buzzer, 5000);
  delay(500);
  noTone(buzzer);
  delay(100);
  displayLCD("Je antwoord is...", "GOED");
  tone(buzzer, 3000);
  delay(100);
  noTone(buzzer);
  tone(buzzer, 4000);
  delay(400);
  noTone(buzzer);
  delay(1000);
}


void vraagFout() {
  displayLCD("Je antwoord is...", "");      //Dit doe je als je de vraag fout hebt beantwoord :(((((((((((
  tone(buzzer, 3000);
  delay(100);
  tone(buzzer, 3500);
  delay(100);
  tone(buzzer, 4000);
  delay(100);
  tone(buzzer, 4500);
  delay(100);
  tone(buzzer, 5000);
  delay(500);
  noTone(buzzer);
  delay(100);
  displayLCD("Je antwoord is...", "FOUT");
  tone(buzzer, 1000);
  delay(1000);
  noTone(buzzer);
  delay(500);
  displayLCD("Ga weer terug", "> vorige positie");
  delay(2000);
}

void vraagBack() {
  lcd.clear();
  tone(buzzer, 2600);
  delay(500);
  noTone(buzzer);
  playMusic();

  vraag = 0;
  vraagstelling = 0; //RESET ALLE VARIABELE
  spelmodus = 2;
}

void vraagGenerate() {
  Send_buf[0] = 0x7e;   // Open command
  Send_buf[1] = 0xff;   // Version
  Send_buf[2] = 0x06;   // Length
  Send_buf[3] = 0x0F;//yes
  Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(0x0D01 >> 8);  //datah
  Send_buf[6] = (int8_t)(0x0D01);       //datal
  Send_buf[7] = 0xef;   //
  for (uint8_t i = 0; i < 8; i++)  {
    mp3.write(Send_buf[i]) ;
  }
  displayLCD("Je vraag is...", "(doe oortjes in)");  //SPANNEND AFWACHTEN todat je vraag wordt laten zien
  tone(buzzer, 3000);
  delay(200);
  tone(buzzer, 3500);
  delay(200);
  tone(buzzer, 4000);
  delay(200);
  tone(buzzer, 4500);
  delay(200);
  tone(buzzer, 5000);
  delay(500);
  noTone(buzzer);
  delay(3000);

}


void playMusic() {
  Send_buf[0] = 0x7e;   // Open command
  Send_buf[1] = 0xff;   // Version
  Send_buf[2] = 0x06;   // Length
  Send_buf[3] = 0x0F;//yes
  Send_buf[4] = 0x00;   // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(0x0E01 >> 8);  //datah
  Send_buf[6] = (int8_t)(0x0E01);       //datal
  Send_buf[7] = 0xef;   //
  for (uint8_t i = 0; i < 8; i++)  {
    mp3.write(Send_buf[i]) ;
  }
}
