/* ---- sensorestest ---- */
/* La idea es probar el codigo para analizar la lectura de los sensores
   y la generación de la señal de error. Todo esto se simulará en el lazo
   de control, por el momento de unos 4 ms. Se midió con el osciloscopio
   en los pines una señal cuadrada que cambia de estado cada 4 ms, verificando
   que el la temporización es adecuada.
   NOTA: si se quieren enviar muchos datos por el puerto serie, no se cumplirá
   lo del lazo a 4 ms....para tenerlo en cuenta....
    - Con la palanca de modo se entra en modo calibración. Se pasa el seguidor
    por la pista y se vuelve la palanca a la posición normal.
    - La función "lecturaSensores" calcula el error de la linea.
    - El resto del código es el PD (sin I) a partir del error y la salida
    output_pid.
    - Puedo sacar por los LEDS una señal cuadrada para ver el tiempo del lazo
    con un osciloscopio (OSCILOSCOPE_DEBUG).
    - También puedo sacar datos de error y del PID por serie (PD_SERIAL_DEBUG).
    - Con el flag CNY_SERIAL_DEBUG se habilitan las mediciones de los sensores.
*/

#define KP 1
#define KD 3
#define SETPOINT_DRIFT 0.1
#define MAX_OUTPUT_PID 320  // Esto sería la máxima acción de control (módulo)
#define PWM_DEAD_ZONE 50    // los motores recien se mueven en 50 de 320.
#define PD_SERIAL_DEBUG 0   // para enviar o no (en 0) el error, la salida del pid, etc.
#define CNY_SERIAL_DEBUG 1  // para enviar o no (en 0) la lectura de los sensores y el error bruto
#define OSCILOSCOPE_DEBUG 0 // habilita para sacar una señal cuadrada por los pines de los LEDs.
#define LOOP_TIME_MS 4000   // tiempo del lazo

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

// VARIABLES TIEMPO
unsigned long loop_timer, tiempoActual, tiempo;

// VARIABLES CALIBRACION SENSORES
int SensorMin[5] = {512, 512, 512, 512, 512}; // la mitad del rango 0-1023 del analogRead
int SensorMax[5] = {512, 512, 512, 512, 512}; // la mitad del rango 0-1023 del analogRead

// PULSADOR Y LLAVE
#define BOT_START 5
#define LLAVE 6

// VARIABLES CONTROLADOR
float setpoint_centrado = 0;
float error, output_pid, pid_last_d_error, pid_diff;
float pid_output_M1, pid_output_M2;


// STATES
byte Flags[1] = {0};
boolean flag = false;

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

  Serial.begin(250000);
  Serial.println("/*** sensorestest ***/");
  Serial.println("/Presione START para inicio. Recuerde habilitar la calibracion para operar normalmente. Luego regrese la palanca a la posicion normal./");
  LEDsInit();
  MotoresInit(M1A, M2A);
  ApagarMotores(M1PWM, M1A, M2PWM, M2A);
  pinMode(BOT_START, INPUT_PULLUP); // Boton para programa
  pinMode(LLAVE, INPUT_PULLUP);     // Llave para elegir opcion

  while (digitalRead(BOT_START) == 1)
  {
    LEDsBlink(1, 1, 1, 100);
  }

  ApagarMotores(M1PWM, M1A, M2PWM, M2A);
  LEDsDrive(1, 1);

  while (digitalRead(BOT_START) == 1) {}

  Serial.println("*** GO! ***");
  LEDsDrive(0, 0);

  // preparo el tiempo del loop
  loop_timer = micros() + LOOP_TIME_MS;


}

void loop() {

  error = (float)lecturaSensores(CNY_SERIAL_DEBUG, &SensorMin[0], &SensorMax[0]) + setpoint_centrado; // el setpoint será "0". El maximo error se espera entre 300 y -300.
  //if (output_pid > 10)error += output_pid * 0.015;  // esto sería para que si el error es muy grande, le agrego al error algo proporcional a la salida, para que corrija más rápido (?
  //if (output_pid < -10)error += output_pid * 0.02;  // esto sería para que si el error es muy grande, le agrego al error algo proporcional a la salida, para que corrija más rápido (?

  if (output_pid < 0) {
    setpoint_centrado += SETPOINT_DRIFT;      // esto es para que a medida que esté en el centro se vaya "ajustando". El valor no está definido aun!
    if (!OSCILOSCOPE_DEBUG)LEDsDrive(0, 1);   // prendo un LED si no estoy en el modo debug oscilocopio.
  }
  if (output_pid > 0) {
    setpoint_centrado -= SETPOINT_DRIFT;      // esto es para que a medida que esté en el centro se vaya "ajustando". El valor no está definido aun!
    if (!OSCILOSCOPE_DEBUG)LEDsDrive(1, 0);   // o el otro LED si no estoy en el modo debug oscilocopio.
  }

  pid_diff = error - pid_last_d_error;                               // calculo la parte derivativa
  if (pid_diff > MAX_OUTPUT_PID)pid_diff = MAX_OUTPUT_PID;           // la restrinjo a un valor máximo
  else if (pid_diff < -MAX_OUTPUT_PID)pid_diff = -MAX_OUTPUT_PID;
  output_pid = KP * (float)error + KD * (pid_diff);                  // calculo la salida del controlador
  if (output_pid > MAX_OUTPUT_PID)output_pid = MAX_OUTPUT_PID;       // la restrinjo a un valor máximo
  else if (output_pid < -MAX_OUTPUT_PID)output_pid = -MAX_OUTPUT_PID;
  pid_last_d_error = error;                                          // guardo el error para la próxima vuelta

  if ((output_pid < PWM_DEAD_ZONE) && (output_pid > -PWM_DEAD_ZONE)) {
    output_pid = 0;                                                  // banda muerta porque los motores solamente se mueven si el pwm es mayor a PWM_DEAD_ZONE
    if (!OSCILOSCOPE_DEBUG)LEDsDrive(0, 0);                          // apago los dos LED porque no hay acción de control si no estoy en el modo osciloscopio
  }

  if (PD_SERIAL_DEBUG) {                                             // envío los datos para debugging
    Serial.print("Error:\t");
    Serial.print("Error D:\t");
    Serial.println("Salida PID:\t");
    Serial.print(error);      Serial.print("\t");
    Serial.print(pid_diff);   Serial.print("\t\t");
    Serial.print(output_pid); Serial.println("\t");
  }

  if (OSCILOSCOPE_DEBUG) {
    flag = !flag;                                // cambio de estado el LED en cada vuelta (para el osciloscopio).
    LEDsDrive(flag, flag);
  }

  if (digitalRead(LLAVE) == 0) {
    CalibrarSensores(&SensorMin[0], &SensorMax[0]);
    Serial.println("CALIBRANDO...");
  }

  while (loop_timer > micros());                // espero a que pasen LOOP_TIME_MS y se repite
  loop_timer += LOOP_TIME_MS;                   // preparo el tiempo límite del siguiente bucle

}


