#define KP 1
#define KD 2.5	          // Probar con KD >> KP 
#define VEL_MAX 150
#define VEL VEL_MAX/3*2   // con 200
#define PWM_DEAD_ZONE 75  // los motores recien se mueven en 75
#define DT 1000           // Cada 1 ms accion de control. (ESTO HAY QUE CAMBIARLO POR EL WHILE)

#define M1PWM 10
#define M1A 8
#define M2PWM 9
#define M2A 7
#define LED1 12 // color rojo
#define LED2 11 // color verde

#define S1 A0
#define S2 A2
#define S3 A3
#define S4 A4
#define S5 A5


#define DT2 50000 // 200000

#define BOT_START 5
#define LLAVE 6

// TIME
unsigned long dt = 0;
unsigned long Now = 0;
unsigned long LastTime[1] = {0};
unsigned long LastTime2[1] = {0};

// CONTROLLER
float LastError[1] = {0};
float Error[1] = {0};
float Constantes[4] = {KP, KD, DT, VEL};
int Vel_filtrada[1] = {0};

// SENSORES
boolean Ledon_state = false;

// STATES
byte Flags[1] = {0};
boolean Flag = false;

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
  Serial.println("/*** sigue_linea_PD ***/");
  Serial.println("*** INITS ***");
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

  while(digitalRead(BOT_START) == 1){}

  Serial.println("*** GO! ***");
  LEDsDrive(0, 0);

}

void loop() {

  if(digitalRead(BOT_START) == 0)
    {
    Flag = !Flag;
    delay(200);
    }

  if (Flag == true)
  {
    Now = micros();
    dt = Now - *LastTime;

    if (dt > Constantes[2])
    {
      Vel_filtrada[0] = LecturaSensores2(1, 0, Error, LastError, LastTime2, Flags);
      SeguirLinea(M1PWM, M1A, M2PWM, M2A, Vel_filtrada[0], Constantes[3]);
      *LastTime = Now;
    }

  }
  else
  {
    LEDsDrive(0, 0);
    ApagarMotores(M1PWM, M1A, M2PWM, M2A);
  }

}
