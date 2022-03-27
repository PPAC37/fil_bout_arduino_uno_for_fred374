

Après une déclaration et affectation de variable il faut absolument un ';' 

Bien faire attention un crochet ouvrant '[' c'est pas une accolade ouvrant '{'


https://www.arduino.cc/reference/en/language/functions/communication/serial/read/

https://www.arduino.cc/en/Tutorial/BuiltInExamples/ReadASCIIString

https://www.arduino.cc/reference/en/language/functions/communication/serial/


https://www.arduino.cc/reference/en/language/functions/communication/serial/parsefloat/
/*
Syntax
Serial.parseFloat()
Serial.parseFloat(lookahead)
Serial.parseFloat(lookahead, ignore)

Parameters
Serial: serial port object. See the list of available serial ports for each board on the Serial main page.
lookahead: the mode used to look ahead in the stream for a floating point number. Allowed data types: LookaheadMode. Allowed lookahead values:

SKIP_ALL: all characters other than a minus sign, decimal point, or digits are ignored when scanning the stream for a floating point number. This is the default mode.

SKIP_NONE: Nothing is skipped, and the stream is not touched unless the first waiting character is valid.

SKIP_WHITESPACE: Only tabs, spaces, line feeds, and carriage returns are skipped.

ignore: used to skip the indicated char in the search. Used for example to skip thousands divider. Allowed data types: char

Returns
Data type: float.*/





// ??? ssi modif env pour avoir printf spécial ... ///Serial.printf("I received: %s%f\n",incomingByte,fVal);
     


//https://playground.arduino.cc/Learning/Memory/

// dans le contexte du langage Arduino F(...) 
// voir https://www.locoduino.org/spip.php?article131 
// et l'usage de PROGMEM https://www.arduino.cc/reference/en/language/variables/utilities/progmem/ 
/* 
<code>
const char text[] PROGMEM = "Bonjour Locoduino";
void setup()
{
  Serial.begin(115200);
 
 *  char buffer[50];
  strcpy_P(buffer, text);
  Serial.println(buffer);
}
 
void loop()
{
}
</code>
*/


