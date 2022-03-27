/* Mesure de température et moteur et parser serie. (todo EEPROM)
 *  
 *  Auteur : PPAC pour fred347 ( https://www.lesimprimantes3d.fr/forum/topic/44540-demande-daide-pour-projet/ )
 *  Etat : version 0.2 beta
 *  
 * Type de carte : Arduino UNO 
 * 
 * ===    
 * // Pour la mesure de température
 * Composants :
 * un arduino UNO
 * une resistance 10K Ohms (code couleur :  Maron = 1 , Noir = 0 , Orange = 3 (comme en position 3 c'est pour dire x10^3) = 10 x 10^3 = 10000 Ohms = 10K Ohms)
 * une Platine d'expérimentation (breadboard)
 *  (ici 5 + 2 ) Câbles de raccord dupont femmel ( jumper wire )
 * une thermistance NTC 100K  avec un connecteur dupont femmel 2p (cf  extrait marlin)
 * un ? pour connecter notre connecteur dupont femmel de la thermistance a la platine d'expérimentation.
 * 
 * deux LED ( une rouge et une verte)
 * des resistances 220 Ohm pour proteger les leds
 * 
 * 
 * 
 * Montage / Schematics: (il faudrais une bonne description du shema ... mais là en mode texte ...)
 * Je renvoie a https://create.arduino.cc/projecthub/Marcazzan_M/how-easy-is-it-to-use-a-thermistor-e39321?ref=search&ref_id=NTC%20Thermistor&offset=1#schematics
 * Sauf que l'on utilisera une thermistance différente.
 * Et que l'on va ajouter une LED + resitance pour la led sur la pin ? 
 * 
 * // Pour le control de la chauffe ( control d'un MOSFET)
 * 
 * 
 * // Pour le controlleur de moteur pas a pas  stepper 
 *  un 
 * 
 * 
 * Pour déterminer les bon paramétres de la thermistance ... ? une doc ?
 * Là je vais me baser sur les fichiers des tables des thermistance du firmware marlin (v2.0.9.2)
 * Marlin/Marlin/src/module/thermistor/
 * ex : fichier "thermistor_1.h" dans les commentaire du fichier 
 * // R25 = 100 kOhm, beta25 = 4092 K, 4.7 kOhm pull-up, bed thermistor
 * 
 * TODO / A FAIRE
 * Pour ne pas avoir a modifier et televerser le sketch si on veux modifier la valeur de température cible
 * Une température cible modifiable via la connexion serie et qui sera sauvé dans l'eeprom 
 * il va faloir mettre en place une methode de modification et de sauvegarde dans l'eeprom
 * 
 * Une autre pin pour controler un MOSFET ou un Relais pour la chauffe
 * ?PWM
 * 
 * La gestion d'un controlleur de moteur pas a pas (stepper) pour activer un moteur
 * ? gestion de la vitesse ??? via la connexion serie définition d'une vitesse ou via un potard donc une entré de mesure analogique pour ajuster ?
 * ? 
 * 
*/

//
// DIRECTIVES DE COMPILATION - Librairie
//

// Include the AccelStepper Library
#include <AccelStepper.h>

// Là on inclus un fichier .h qui contiens des fonctions utilitaires ...
#include <util_fonc.h>


//
// DIRECTIVES DE COMPILATION - Pour paramétrages / configuration 
//

// une directive "#define MOT_CLE expression" c'est utilisé par le compilateur pour remplacer dans le code qui suit le "MOT_CLE" par l"expression" (on ne retrouvera dans le code compilé que cette "expression".)
// C'est pratique pour avoir des variables lors de la compilation mais qui au final seront des constantes dans le code compilé (pour gagner de l'espace memoire)
/** (là j'ai piqué un bout de marlin :) pour me simplifier la vie :)  )
 * Vitesse de communication du port serie (Serial Port Baud Rate)
 * This is the default communication speed for all serial ports.
 * Set the baud rate defaults for additional serial ports below.
 *
 * 250000 works in most cases, but you might try a lower speed if
 * you commonly experience drop-outs during host printing.
 * You may try up to 1000000 to speed up SD file transfer.
 *
 * :[2400, 9600, 19200, 38400, 57600, 115200, 250000, 500000, 1000000]
 */
#define BAUDRATE 115200

/*
 * Faut til faire du debugage ?
 * TODO a revoir pour vraiment ne pas compiler les parties de debugage pour avoir un code compilé plus légé ...
*/
#define DEBUG_MODE_ON

/*
 * Pour le delay de la loop principal 
 * 500 ms 
*/
#define TEMPO_LOOP 500

#define TEMPERATURE_CIBLE_DEFAUT 30.00 // Là pour tester la thermistance 30°C c'est plus pratique.

// une declaration de varibable "typeVariable nomVariable [ = valeuraffectation ] ; " là dans le code généré il y aura un espace memoire reservé pour identifier cette
#ifndef TEMPERATURE_CIBLE_DEFAUT
float temp_cible = 30.00;
#else
float temp_cible = TEMPERATURE_CIBLE_DEFAUT;
#endif

//Thermometer with thermistor
/*thermistor parameters:
 * RT0: 10 000 Ω // PPAC 100 000 Ω = 100K Ω
 * B: 3977 K +- 0.75%// PPAC  4092
 * T0:  25 C
 * +- 5%
 */
//These values are in the datasheet ( PPAC : là j'ai modifier d'aprés des fichier du code source du marlin pour coller a la thermistance utilisé
#define RT0 100000 // Ω
#define B 4092     //3977      // K
//--------------------------------------

#define VCC 5.00 //Supply voltage
//#define VCC 3.30    //Supply voltage // 

// Là valeur de la résitance du montage pour la mesure ... a modifier au besion
#define R 10000 //R=10KΩ

//
//
//

#define PIN_LED_TEMOIN_PAS_A_TEMPERATURE_CIBLE 10 // led rouge pour dire ue ce n'est pas en temperature
#define PIN_LED_TEMOIN_A_TEMPERATURE_CIBLE 11     //  led verte pour dire que c est en temperature
// N.B. : Pour moi, dans un programme, il faut bien différencier la "fonction logique" du "matériel"
// c-a-d ( on peut mettre les couleur de led que l'on veux mais dans le code je veux savoir c'est pour quelle usage ...)

//
// Les variables du programme
//

// Là des variable pour de la mesure de A0 arriver a une température en °C
//Variables
float RT, VR, ln, TX, T0, VRT;

//
//
// Là c'est a revoir ... mais c'est la methode (utiliser des directive de compilation #ifdef ... #endif) pour reduire la taille du programme si on d'esactive le debug sur le port serie.
#ifdef DEBUG_MODE_ON
boolean debug = true;
#else
boolean debug = false;
#endif

boolean marlinOutputLike = true;

//
// Define pin connections & motor's steps per revolution
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;

float motorSpeed = 200;

// Include the AccelStepper Library //#include <AccelStepper.h>
// Define motor interface type
#define motorInterfaceType 1

// Creates an instance
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

boolean activerMoteur = true;

//
// Declaration de mes variables utiles pour le programme
//


void outputSketchInfos(){
  Serial.print(F("\nSketch:   " __FILE__ "\n"
                 "Compiled: " __DATE__ " " __TIME__ "\n\n"));  
}

/* Initialisation (executé une seul fois au démmarge de l'arduino.)
     */
// Un programme Arduino doit impérativement contenir la fonction "setup"
// Elle ne sera exécutée qu'une seule fois au démarrage du microcontroleur
// Elle sert à configurer globalement les entrées sorties
void setup()
{                    
  // Vitesse du port serie pour debug / messages
  Serial.begin(BAUDRATE); //PPAC avant : Serial.begin(9600);
  
  // Attendez que le port s'ouvre
  // Une while boucle bouclera continuellement et infiniment jusqu'à ce
  // que l'expression entre parenthèses () devienne fausse.
  // Quelque chose doit changer la variable testée,
  // sinon la boucle while ne se fermera jamais
  while (!Serial)
  {
    ;   // attendez que le port série se connecte.
        // Nécessaire pour le port USB natif uniquement
  }

  outputSketchInfos();
  //outputTableDesCaracteres();
  
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  myStepper.setMaxSpeed(10000); //1000
                                //  myStepper.setAcceleration(50);
  myStepper.setSpeed(2000);     //200
                                // myStepper.moveTo(2000);//200

  if (debug)
  {
    // TODO a revoir c'est pas top de mettre du debug dans l'init est il y a un defaut d'entrelacement avec les sortie des message de la "loop" ...
    // il doit y avoir une notion de ? semaphore a mettre en place  ou de niveau d'intéruption ?
    Serial.print("### Debut initialisation ! Temperature cible = ");
    Serial.print(temp_cible);
    Serial.println(" °C");
    Serial.flush(); // forcer le vidage du tampon ?
  }

  // Là c'est ? de l'initialisation d'une constante pour correctement calculer une température d'aprés une mesure ?
  T0 = 25 + 273.15; //Temperature T0 from datasheet, conversion from Celsius to kelvin
  //
  // Initialisation des pin de sorties
  pinMode(PIN_LED_TEMOIN_PAS_A_TEMPERATURE_CIBLE, OUTPUT);
  digitalWrite(PIN_LED_TEMOIN_PAS_A_TEMPERATURE_CIBLE, LOW); // turn the LED off by making the voltage LOW

  pinMode(PIN_LED_TEMOIN_A_TEMPERATURE_CIBLE, OUTPUT);
  digitalWrite(PIN_LED_TEMOIN_A_TEMPERATURE_CIBLE, LOW); // turn the LED off by making the voltage LOW

  // moteur driver type ...
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Autre initialisation a faire (TODO) ? Si on veux faire des lecture sur le port serie pour rendre le programme plus pratique
  // definir la température cible avec une commande g-code du genre "M104 S200" ? mais là il y a du taf car il faut faire un parseur...
  //...
  if (debug)
  {
    Serial.print("### Fin initialisation ! Temperature cible = ");
    Serial.print(temp_cible);
    Serial.println(" °C");
    Serial.flush(); // forcer le vidage du tampon ?
  }
  //
  if ( marlinOutputLike ){
      Serial.println("start");
  }
}

// Le programme principal s’exécute par une boucle infinie appelée Loop
void loop()
{

  // TODO a mettre dans une fonction
  // mesure température et conversion en °C
  VRT = analogRead(A0);         //Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT; //Conversion to voltage (la A0 [0..1023] -> [0V..5V]
  VR = VCC - VRT;
  RT = VRT / (VR / R); //Resistance of RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); //Temperature from thermistor

  TX = TX - 273.15; //Conversion to Celsius

  if (debug && false)
  {
    // Sortie sur la consol pour débuger la mesure et le calcul en °C
    Serial.print("Temperature:");
    Serial.print("\t");
    Serial.print(TX);
    // Serial.print("C\t\t");
    //  Serial.print(TX + 273.15);        //Conversion to Kelvin
    //  Serial.print("K\t\t");
    //  Serial.print((TX * 1.8) + 32);    //Conversion to Fahrenheit
    //  Serial.println("F");
    Serial.print(" / ");
    Serial.print(temp_cible);
    Serial.println();
  }
  // 
  if ( marlinOutputLike ){
  Serial.print("T:");
  Serial.print(TX);
  Serial.print(" /");
  Serial.print(temp_cible);
  Serial.print(" ");
  Serial.print("B:");
  Serial.print("0.00");
  Serial.print(" /");
  Serial.print("0.00");
  Serial.print(" ");
  Serial.print("@:");
  Serial.print("0");
  Serial.print(" ");
  Serial.print("B@:");
  Serial.print("0");
  Serial.println();
  }

  // Condition de température
  if (TX > temp_cible)
  {
    // { la température mesuré et au dessus de la température cible }
    if (debug && false)
    {
      Serial.print("Cible ");
      Serial.print(temp_cible);
      Serial.println(" atteint !");
    }
    //
    digitalWrite(PIN_LED_TEMOIN_A_TEMPERATURE_CIBLE, HIGH);    // turn the LED on (HIGH is the voltage level)
    digitalWrite(PIN_LED_TEMOIN_PAS_A_TEMPERATURE_CIBLE, LOW); // turn the LED off by making the voltage LOW
  }
  else
  {
    // { la température mesuré et en dessous de la température cible }
    if (debug && false)
    {
      Serial.print("Il faut chauffer pour arriver a cible ");
      Serial.print(temp_cible);
      Serial.println("!");
    }
    digitalWrite(PIN_LED_TEMOIN_A_TEMPERATURE_CIBLE, LOW);      // turn the LED on (HIGH is the voltage level)
    digitalWrite(PIN_LED_TEMOIN_PAS_A_TEMPERATURE_CIBLE, HIGH); // turn the LED off by making the voltage LOW
  }

  //delay(TEMPO_LOOP);
  if (activerMoteur)
  {
    if (false)
    {
      if (myStepper.distanceToGo() == 0)
      {
        myStepper.moveTo(-myStepper.currentPosition());
      }
      myStepper.run();
    }

    // constante speed
    // Set the speed in steps per second:
    //myStepper.setSpeed(400);
    myStepper.setSpeed(motorSpeed);

    // Step the motor with a constant speed as set by setSpeed():
    myStepper.runSpeed();
  }

  if (activerMoteur && false)
  {
    // Là c'est juste pour tester le moteur .
    // Comme c'est codé dans la boucle principla et avec de delay cela va créer un ralentissement / conflic entre le code de mesure et de mouvement moteur ( donc la vitesse moteur ne sera pas vraiment controllable, il y aura des ralentissement du moteur lors des entré serie car elle sont parsé ...)
    // Il faudrai utiliser un timer pour avoir une sorte de multitache mais l) les timer sous arduino sur un controlleur atmega c'est ... pas forcement smple
    // alors il fau plutot utiliser une librairie qui gere cela ?

    // Set motor direction clockwise
    digitalWrite(dirPin, HIGH);

    // Spin motor slowly
    for (int x = 0; x < stepsPerRevolution; x++)
    {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(2000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(2000);
    }
    /*
  delay(1000); // Wait a second
  
  // Set motor direction counterclockwise
  digitalWrite(dirPin, LOW);

  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second
             */
    //
  }

  int incomingByte = 0; // for incoming serial data

  while (Serial.available() > 0)
  {
    // look for the next valid integer in the incoming serial stream:
    int red = 0; //Serial.parseInt();
    // do it again:
    int green = 0; //Serial.parseInt();
    // do it again:
    int blue = 0; //Serial.parseInt();
    // look for the newline. That's the end of your sentence:
    // read the incoming byte:
    incomingByte = Serial.read();
    if (incomingByte == '\n')
    {
      // constrain the values to 0 - 255 and invert
      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
      red = 255 - constrain(red, 0, 255);
      green = 255 - constrain(green, 0, 255);
      blue = 255 - constrain(blue, 0, 255);
      // fade the red, green, and blue legs of the LED:
      //   analogWrite(redPin, red);
      //   analogWrite(greenPin, green);
      //   analogWrite(bluePin, blue);
      // print the three numbers in one string as hexadecimal:
      //Serial.print(red, HEX);
      //Serial.print(green, HEX);
      //Serial.println(blue, HEX);
      if ( marlinOutputLike ){
        // TODO un parser des commandes g-code flavor marlin (compatible Repetier Host et Pronterface )
//Replies from the RepRap machine to the host computer
//All communication is in printable ASCII characters.
// Messages sent back to the host computer are terminated by a newline and look like this:

//xx [line number to resend] [T:93.2 B:22.9] [C: X:9.2 Y:125.4 Z:3.7 E:1902.5] [Some debugging or other information may be here]


        // Si la commande g-code reçue est correcte ... et pas d'erreur detecté
      Serial.println("ok");
      // Si la commande g-code recu n'est pas correcte ?

      // demande de réenvoie
      //Serial.println("rs ");
      //Serial.println(num_ligne_a_reenvoyer);// num ligne a réenvoyer

      // Erreur hardware fault has been detected. The RepRap machine will shut down immediately after it has sent this message.
      //Serial.println("!!");

// "//" This is some debugging or other information on a line on its own. It may be sent at any time.

//Such lines will always be preceded by //.

//On the latest version of Pronterface and soon Octoprint a special comment of the form:

// "// action:command"

// is allowed to be sent from the firmware, the command can currently be pause, resume or disconnect which will execute those commands on the host. As this is also a comment other hosts will just ignore these commands.
//Serial.println("// ");
      }
    }
    if ( false){
    // say what you got:
    Serial.print("I received: '");
    Serial.print((char)incomingByte);
    Serial.print("' : ");
    Serial.println(incomingByte, DEC);

    if (incomingByte == 'M')
    {
      activerMoteur = !activerMoteur;
      int iVal = Serial.parseInt();
      Serial.print("I parse: ");
      Serial.print((char)incomingByte);
      Serial.println(iVal);
    }
    if (incomingByte == 'T')
    {
      
      float fVal = Serial.parseFloat(SKIP_ALL);
      Serial.print("I parse: ");
      Serial.print((char)incomingByte);
      Serial.println(fVal);
      temp_cible = fVal;
      //Serial.print(fVal);
    }
    if (incomingByte == 'P')
    {
      float fVal = Serial.parseFloat();
      Serial.print("P parse: ");
      Serial.print((char)incomingByte);
      Serial.println(fVal);
      //Serial.print(fVal);
    }
    if (incomingByte == 'S')
    {
      float fVal = Serial.parseFloat();
      Serial.print("S parse: ");
      Serial.print((char)incomingByte);
      Serial.println(fVal);
      //Serial.print(fVal);
      motorSpeed = fVal;
    }
    }
  }

  // Un delais pour ... histoire de prendre sons temps ? (surtout si avec le mode debug pour ne pas avoir trop de blabla ! )
  delay(500); // TODO a mettre en #define TEMPO_LOOP 500
  delay(TEMPO_LOOP);
}

// FIN de fichier.
