#include <TM1637Display.h>

#define stepsPerRev 200 //Pasos por revolucion
#define revsPerFloor 5 //RPM para subir o bajar un piso
#define dirDescending LOW //Cambiar si el asensor baja en lugar de subir

#define stepPin 2
#define dirPin 3
#define fotoSensorPin 4
// Module connection pins (Digital Pins)
#define CLK 5
#define DIO 6
#define btnPiso1 6
#define btnPiso2 7
#define btnPiso3 8


int pisoActual = 1;
int pisoDeseado = 1;
bool lastDir = false;

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };
const uint8_t SEG_SCH[] = {
  SEG_A | SEG_D | SEG_C | SEG_F | SEG_G,           // S
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_C | SEG_E | SEG_G | SEG_D                    // n
  };

TM1637Display display(CLK, DIO);
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };

void setup() {
  // put your setup code here, to run once:
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  pinMode(btnPiso1, INPUT_PULLUP);
  pinMode(btnPiso2, INPUT_PULLUP);
  pinMode(btnPiso3, INPUT_PULLUP);
  Serial.begin(9600);
  
  display.setBrightness(0x0f);

  // All segments on
  display.setSegments(data);
  while(buscarPiso())
  {
    Serial.println("Buscando piso 1"); 
    display.setSegments(SEG_SCH);
  }
  display.setSegments(SEG_DONE);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(pisoActual = pisoDeseado) noMover();
  else if(pisoActual<pisoDeseado) subir();
  else if(pisoActual>pisoDeseado) bajar();
  leerBotones();
}
void leerBotones()
{
  if(!digitalRead(btnPiso1)) pisoDeseado = 1;
  else if(!digitalRead(btnPiso2)) pisoDeseado = 2;
  else if(!digitalRead(btnPiso3)) pisoDeseado = 3;
}

void subir()
{
  digitalWrite(dirPin,!dirDescending); //Changes the rotations direction
  for(int x = 0; x < stepsPerRev*revsPerFloor; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  pisoActual++;
  imprimirPiso();
}
void bajar()
{
  digitalWrite(dirPin,dirDescending); //Changes the rotations direction
  for(int x = 0; x < stepsPerRev*revsPerFloor; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  pisoActual--;
 imprimirPiso(); 
}
void noMover()
{
  digitalWrite(stepPin,true);
  delayMicroseconds(5);
  imprimirPiso(); 
  lastDir = !lastDir;
}
void imprimirPiso()
{
  display.showNumberDec(pisoActual, false);
}
boolean buscarPiso()
{
  digitalWrite(dirPin,dirDescending); //Changes the rotations direction
  for(int x = 0; x < stepsPerRev/4; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
  }
  return digitalRead(fotoSensorPin);
}
