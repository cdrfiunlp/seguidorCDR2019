/* ---- pwmtest ---- */
/* La idea es probar el codigo para generar pwm a 25 kHz de 0 a 320
 * Los motores arrancan apagados y a medida que se presiona el botón 
 * amarillo se aumenta en 10 el pwm. Segun el osciloscopio esto implica
 * un aumento del 3,1 % aproximadamente del ciclo de trabajo, lo que 
 * concuerda con la cuenta 10/320*100 = 3.125.
 */

#define VEL_MAX 150
#define VEL VEL_MAX/3*2   // con 200
#define PWM_DEAD_ZONE 0  // los motores recien se mueven en

#define M1PWM 10
#define M1A 8
#define M2PWM 9
#define M2A 7
#define LED1 12 // color rojo
#define LED2 11 // color verde

#define BOT_START 5
#define LLAVE 6


// STATES
byte Flags[1] = {0};
boolean Flag = false;
int kk = 0;

void setup() {

  // Configurar Timer 1 para PWM @ 25 kHz. min: 0, max: 320
  TCCR1A = 0;           // undo the configuration done by...
  TCCR1B = 0;           // ...the Arduino core library
  TCNT1  = 0;           // reset timer
  TCCR1A = _BV(COM1A1)  // non-inverted PWM on ch. A
           | _BV(COM1B1)  // same on ch; B
           | _BV(WGM11);  // mode 10: ph. correct PWM, TOP = ICR1
  TCCR1B = _BV(WGM13)   // ditto
           | _BV(CS10);   // prescaler = 1
  ICR1   = 320;         // TOP = 320

  // Set the PWM pins as output.
  pinMode( 9, OUTPUT);
  pinMode(10, OUTPUT);
  //---- fin Configurar Timer 1 ---- //

  Serial.begin(9600);
  Serial.println("/*** pwm test ***/");
  LEDsInit();
  MotoresInit(M1A, M2A);
  ApagarMotores(M1PWM, M1A, M2PWM, M2A);
  pinMode(BOT_START, INPUT_PULLUP); // Boton para programa
  pinMode(LLAVE, INPUT);           // Llave para elegir opcion

  while (digitalRead(BOT_START) == 1)
  {
    LEDsBlink(1, 1, 1, 100);
  }

  ApagarMotores(M1PWM, M1A, M2PWM, M2A);
  LEDsDrive(1, 1);

}

void loop() {

  if (digitalRead(BOT_START) == 0)
  {
    if (kk == 320)
    {
      kk = 0;
    }
    kk = kk + 10;
    LEDsBlink(1, 1, 1, 200);
    delay(200);
    LEDsDrive(0, 0);
  }

  digitalWrite(M1A, 0);
  digitalWrite(M2A, 0);
  analogWrite25k(M2PWM, kk);
  analogWrite25k(M1PWM, kk);
  Serial.println(kk);

}
