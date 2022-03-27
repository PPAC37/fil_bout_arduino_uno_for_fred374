Essai Arduino Uno Mesure de température 
====


# fil_bout_arduino_uno_for_fred374

Pour aider 
[fred347](https://www.lesimprimantes3d.fr/forum/profile/33156-fred347/) 
sur le sujet 
https://www.lesimprimantes3d.fr/forum/topic/44540-demande-daide-pour-projet/
dans le forum de [https://www.lesimprimantes3d.fr/forum/](https://www.lesimprimantes3d.fr/forum/)

Un programme/sketch Arduino uno pour faire du recyclage avec des bouteilles d'eau plastique (PETG?) en filament pour imprimante 3D.

https://www.lesimprimantes3d.fr/forum/uploads/monthly_2021_11/404018649_Fichier1test.png.740974e837bb58ca29a6e3a193485982.png
404018649_Fichier1test.png.740974e837bb58ca29a6e3a193485982.png

 je voudrais pouvoir: 

1)
a) connecter mon hot-end (corp de chauffe et thermistance)a mon arduino uno (avec ou sans mon bouclier cnc sheld v3)
b) connecter mon moteur PaP (nema 17) a mon arduino uno (avec ou sans mon bouclier )

2)
a) quoi metre dans mon arduino ???(marlin ,bibliothèque stepper......)
b) comment modifier le programme 

3) que je puisse gérer ca (T° et vitesse du moteur )avec un pc relier en usb 


pour la vitesse tu a raison plus le fil va s'enrouler plus il iras vite c'est 
pour ca je voulais modifier le système et faire l'enroulage après avoir tirer sur le fil 
https://www.lesimprimantes3d.fr/forum/uploads/monthly_2021_11/1922091561_Fichier1fdm.png.a3bfe7cef1bd641aa09b691a0c2b05c0.png
1922091561_Fichier1fdm.png.a3bfe7cef1bd641aa09b691a0c2b05c0.png
* oui j'aime les dessin 

et pour l'écran j'en nais pas donc pronterface était pour moi facile de gérer 



- 👋 Hi, I’m @PPAC37
- 👀 I’m interested in ...
- 🌱 I’m currently learning ...
- 💞️ I’m looking to collaborate on ...
- 📫 How to reach me ...

<!---
PPAC37/PPAC37 is a ✨ special ✨ repository because its `README.md` (this file) appears on your GitHub profile.
You can click the Preview link to take a look at your changes.
--->

Logging Issues
------------
For crashes and similar issues, please attach the following information:

* (On Windows) The log as produced by dxdiag (start -> run -> dxdiag -> save output)
* The Cura GUI log file, located at
  * `%APPDATA%\cura\<Cura version>\cura.log` (Windows), or usually `C:\Users\<your username>\AppData\Roaming\cura\<Cura version>\cura.log`
  * `$HOME/Library/Application Support/cura/<Cura version>/cura.log` (OSX)
  * `$HOME/.local/share/cura/<Cura version>/cura.log` (Ubuntu/Linux)



Projet pour l'utilisation d'un capteur de température (thermistance NTC 100K Ω )


https://www.lesimprimantes3d.fr/forum/topic/44540-demande-daide-pour-projet/

![Screenshot](README-logo.PNG)


Code basé sur / inspiré de 
--------

How Easy Is It to Use a Thermistor?! © GPL3+ 
Marcazzan_M
https://create.arduino.cc/projecthub/Marcazzan_M/how-easy-is-it-to-use-a-thermistor-e39321?ref=search&ref_id=NTC%20Thermistor&offset=1

Modifié pour une thermistance NTC 100K et du debug et deux led 

et 
https://www.makerguides.com/a4988-stepper-motor-driver-arduino-tutorial/

Control Stepper Motor with A4988 Driver Module & Arduino
https://lastminuteengineers.com/a4988-stepper-motor-driver-arduino-tutorial/

https://lastminuteengineers.com/drv8825-stepper-motor-driver-arduino-tutorial/


DRV8825 Stepper Motor Driver Carrier, High Current
https://www.pololu.com/product/2133

A4988 Stepper Motor Driver Carrier
https://www.pololu.com/product/1182


=== 
( Là un extrait de Configuration.h de marlin v2.0.9.2 )
```
/* Analog Thermistors - 4.7kΩ pullup - Normal
-------
1 : 100kΩ  EPCOS - Best choice for EPCOS thermistors
*/
#define TEMP_SENSOR_0 1
```


Dependencies
------------
*



Références
------------
* [Thermistance](https://fr.wikipedia.org/wiki/Thermistance) : Une thermistance est un composant électronique dont la résistance électrique varie en fonction de la température.


Base de connaissances
------------


NTC 10k

https://create.arduino.cc/projecthub/salhar1/mini-thermostat-e732ed?ref=search&ref_id=NTC%20Thermistor&offset=3

https://create.arduino.cc/projecthub/Marcazzan_M/how-easy-is-it-to-use-a-thermistor-e39321?ref=search&ref_id=NTC%20Thermistor&offset=1


NTC 100K
https://create.arduino.cc/projecthub/dbarbee/temperature-controled-charcoal-smoker-58db49?ref=search&ref_id=NTC%20100K%20Thermistor&offset=1



Stepper Motor Basics
https://forum.arduino.cc/t/stepper-motor-basics/275223/2

torque
https://forum.arduino.cc/t/stepper-motor-basics/275223/30


Utilisation des Timers de l’Arduino
https://www.aranacorp.com/fr/utilisation-des-timers-de-larduino/

Manual pulsing for interrupt driven stepper
https://forum.arduino.cc/t/manual-pulsing-for-interrupt-driven-stepper/491582/5

  

```
 #include <Stepper.h>
 Stepper myStepper(StepsPerRevolution, 8,9,10,11);
 Stepper myStepper(StepsPerRevolution, 9,10);
 setSpeed(rpms);
 step(steps);

```
 
Continuer avec pour s'inspirer :
------------

https://create.arduino.cc/projecthub/dbarbee/temperature-controled-charcoal-smoker-58db49?ref=search&ref_id=NTC%20100K%20Thermistor&offset=1
  
3D Printer Thermal Runaway Watchdog / Thermistor Tester © CC BY-NC-SA
https://create.arduino.cc/projecthub/binsun148/3d-printer-thermal-runaway-watchdog-thermistor-tester-4a0ebb
  



License
----------------
?released under the terms of the LGPLv3 or higher. A copy of this license should be included with the software.