

#ifndef util_fonc_h
#define util_fonc_h



#include "Arduino.h"

//https://www.redohm.fr/2019/07/arduino-tutoriel-la-memoire-eeprom/
// Pour inclure la librairie EEPROM dans un programme, on ajoutera
// au début du programme la ligne suivante
#include <EEPROM.h>




/*

  if (false)
  {
    outputTableDesCaracteres();
  }

*/
// Pour avoir une idées du jeux de caractéres disponible.
void outputTableDesCaracteres(){    
    Serial.println("      0  1  2  3  4  5  6  7  8  9");
    Serial.print("----------------------------------");
    // N.B. comme un byte se trouve codé sur 8 bits non signé (cf: <code>typedef uint8_t byte</code>) cela code/donne une valeur de 0 a 255
    //et si on test car < 256 là on créer une boucle inifinie car sur un byte a=255; a++; //donne (a==0) ...
    for (byte car = /*30*/15; car < /*127*/ 255; car++)
    {
      if (car % 10 == 0) // Le % ou 'modulo' donne le reste de la division entière.
      {
        Serial.println(""); // passe à la ligne
        Serial.print(car, DEC);
        if (car < 100)
        {
          Serial.print("  :"); // Besoin d'un peu d'espace...
        }
        else
        {
          Serial.print(" :");
        }
      }
      Serial.print(" ");
      Serial.print((char)car);
      Serial.print(" ");
    }
    Serial.println(""); // passe à la ligne
}


//



/*
requis : 
#include <EEPROM.h>

exemple d'usage :

  if (true)
  {
   outputEEPROM();

    //Met le programme en pause pendant la durée (en millisecondes)
    //spécifiée en tant que paramètre. (Il y a 1000 millisecondes
    //dans une seconde.)
    delay(200);
  }
*/
// Declaration de mes variables utile pour le programme
unsigned int adresse = 0;
int ligne = 0;
int compt = 0;
unsigned int compt1 = 19;

void outputEEPROM(){ 

    Serial.println(" ========================================================");
    Serial.println("    Affichage des adresses et des valeurs se trouvant    ");
    Serial.println("                     dans EEPROM                         ");
    Serial.println(" ========================================================");

    Serial.print(" EEPROM.length = ");
    Serial.print(EEPROM.length());
    Serial.println(" bytes.");

    Serial.println("    ");
    Serial.print("      ");

    // Preparation du comptage des colonnes de 0 a 9
    for (compt = 0; compt <= 9; compt++)
    {
      Serial.print(" ");
      Serial.print(compt);
      Serial.print("  ");
    }

    // Preparation du comptage des colonnes de 10 a 20
    for (compt = 10; compt <= 19; compt++)
    {
      Serial.print(" ");
      Serial.print(compt);
      Serial.print(" ");
    }

    // Position du numero 0
    Serial.println("");
    Serial.print("  ");
    Serial.print(adresse);
    Serial.print("   ");

    //Plutôt que de coder en dur la longueur, vous devriez utiliser
    //la fonction de longueur pré-fournie EEPROM.length().
    //Cela rendra votre code portable à tous les processeurs AVR.
    for (adresse = 0; adresse < /*= 511*/ EEPROM.length(); adresse++)
    {
      // commence à lire le premier octet (adresse 0) de la mémoire EEPROM
      // conversion au format int pour affichage valeur numérique
      Serial.print(int(EEPROM.read(adresse)));
      Serial.print(" ");
      //delay (100);
      if ((adresse == compt1) & (adresse <= 99))
      {
        Serial.println("");
        Serial.print(" ");
        Serial.print(adresse);
        Serial.print("   ");
        compt1 = compt1 + 20;
      }
      // affichage des adresses au-dessus de 99
      else if (adresse == compt1)
      {
        Serial.println("");
        Serial.print(" ");
        Serial.print(adresse);
        Serial.print("  ");
        compt1 = compt1 + 20;
      }
    }
    Serial.println("");
}




/*
requis : 
#include <EEPROM.h>

exemple d'usage :

*/
// la variable adresse possede la valeur de debut de memoire
unsigned int address = 0;
// Declaration de la variable value en un octet
// Un octet stocke un nombre non signé de 8 bits, compris entre 0 et 255.
byte value;

void serialPrintEEPROMUnderLOOP(){
   // lit un octet à partir de l'adresse actuelle de la mémoire EEPROM
    value = EEPROM.read(address);
    Serial.print(address);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.println();

    // on increment la variable adresse pour pouvoir lire l'ensemble
    // de l'espace mémoire
    address = address + 1;
    if (address == EEPROM.length())
    {
      address = 0;

      //
    }
}

#endif