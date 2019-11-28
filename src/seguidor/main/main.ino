#define KP 2.15
#define KD 14
#define SETPOINT_DRIFT 0
#define PWM_DEAD_ZONE 70               // los motores recien se mueven en 70 de 320.
#define PWM_MAX_ABS 320                // límite máximo del PWM
#define PWM_MAX 285                    // velocidad máxima para el auto
#define PWM_MAX_M2 PWM_MAX             // maximo PWM para el motor 2. El motor 1 tendrá un PWM un poco menor (ver cuenta debajo)
#define PWM_MAX_M1 (((PWM_MAX_M2 * 3.77) - 129) + 135.5) / 3.93
#define MAX_OUTPUT_PID (PWM_MAX + 200) // Esto sería la máxima acción de control (módulo). Los 100 restantes por ahora es la "marcha atrás"
#define PD_SERIAL_DEBUG 0              // para enviar o no (en 0) el error, la salida del pid, etc.
#define CNY_SERIAL_DEBUG 0             // para enviar o no (en 0) la lectura de los sensores y el error bruto
#define OSCILOSCOPE_DEBUG 0            // habilita para sacar una señal cuadrada por los pines de los LEDs.
#define LOOP_TIME_MS 2000              // tiempo del lazo

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
float velM2_RPM = 0; float velM1 = 0; float velM2 = 0;

// STATES
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
  Serial.println("/*** sigue_linea_PD ***/");
  Serial.println("*** INITS ***");
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

  while (digitalRead(BOT_START) == 1) {}

  Serial.println("*** GO! ***");
  LEDsDrive(0, 0);
  delay(300);

  // preparo el tiempo del loop
  loop_timer = micros() + LOOP_TIME_MS;

}

void loop() {

  if (digitalRead(BOT_START) == 0)
  {
    flag = !flag;
    delay(200);
  }

  if (digitalRead(LLAVE) == 0) {
    CalibrarSensores(&SensorMin[0], &SensorMax[0]);                 // calibro los sensores si muevo la llave. Hay que volverla a la posicion para que funcione correctamente el auto.
    LEDsBlink(0, 1, 1, 10);
    LEDsBlink(1, 0, 1, 10);
    Serial.println("CALIBRANDO...");
  }

  if (OSCILOSCOPE_DEBUG) {
    flag = !flag;                                                   // cambio de estado el LED en cada vuelta (para el osciloscopio).
    LEDsDrive(flag, flag);
  }

  if (flag == true)
  {
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
      if (!OSCILOSCOPE_DEBUG)LEDsDrive(1, 1);                          // apago los dos LED porque no hay acción de control si no estoy en el modo osciloscopio
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Velocidad de motores
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // La idea es probar descontarle a cada motor lo suficiente como para que gire.... si esto no funciona, volveremos a "VM +- correccion"
    // pero en ese caso no se puede subir más la velocidad de ambos más que VM.

    velM2_RPM = (output_pid * 3.77) - 129;      // correccion de velocidad de los motores. Ver script medicionVelocidadMotores.m
    velM1 = (velM2_RPM + 135.5) / 3.93;         // el PWM del M1 será algo menor que el de M2
    velM2 = output_pid;                         // el de M2 es directamente la salida del PID

    if (output_pid > 0) {
      digitalWrite(M2A, 0);
      analogWrite25k(M2PWM, PWM_MAX_M2);
      if (velM1 > PWM_MAX_M1) {
        digitalWrite(M1A, 1);
        analogWrite25k(M1PWM, PWM_MAX_ABS + PWM_MAX_M1 - velM1);
      } else {
        digitalWrite(M1A, 0);
        analogWrite25k(M1PWM, PWM_MAX_M1 - velM1);
      }
    } else if (output_pid < 0) {
      digitalWrite(M1A, 0);
      analogWrite25k(M1PWM, PWM_MAX_M1);
      if (velM2 < -PWM_MAX_M2) {
        digitalWrite(M2A, 1);
        analogWrite25k(M2PWM, PWM_MAX_ABS + PWM_MAX_M2 + velM2);
      } else {
        digitalWrite(M2A, 0);
        analogWrite25k(M2PWM, PWM_MAX_M2 + velM2);
      }
    } else {
      digitalWrite(M1A, 0);
      analogWrite25k(M1PWM, PWM_MAX_M1);
      digitalWrite(M2A, 0);
      analogWrite25k(M2PWM, PWM_MAX_M2);
    }


    if (PD_SERIAL_DEBUG) {                                             // envío los datos para debugging
      Serial.print("Error:\t");
      Serial.print("Error D:\t");
      Serial.print("Output PD:\t");
      Serial.print("VelM2 (PD output):\t");
      Serial.print("GiroM2:\t");
      Serial.print("VelM1:\t");
      Serial.println("GiroM1:\t");
      Serial.print(error);      Serial.print("\t");
      Serial.print(pid_diff);   Serial.print("\t\t");
      Serial.print(output_pid);   Serial.print("\t\t");
      if (output_pid > 0) {

        if (velM1 > PWM_MAX_M1) {
          Serial.print(PWM_MAX_M2); Serial.print("\t\t\t");
          Serial.print("1"); Serial.print("\t");
          Serial.print(PWM_MAX_ABS + PWM_MAX_M1 - velM1); Serial.print("\t");
          Serial.print("0"); Serial.println("\t");
        } else {
          Serial.print(PWM_MAX_M2); Serial.print("\t\t\t");
          Serial.print("1"); Serial.print("\t");
          Serial.print(PWM_MAX_M1 - velM1); Serial.print("\t");
          Serial.print("1"); Serial.println("\t");
        }
      } else {
        if (velM2 < -PWM_MAX_M2) {
          Serial.print(PWM_MAX_ABS + PWM_MAX_M2 + velM2); Serial.print("\t\t\t");
          Serial.print("0"); Serial.print("\t");
          Serial.print(PWM_MAX_M1); Serial.print("\t");
          Serial.print("1"); Serial.println("\t");
        } else {
          Serial.print(PWM_MAX_M2 + velM2); Serial.print("\t\t\t");
          Serial.print("1"); Serial.print("\t");
          Serial.print(PWM_MAX_M1); Serial.print("\t");
          Serial.print("1"); Serial.println("\t");
        }
      }
    }



  }
  else
  {
    LEDsDrive(0, 0);
    ApagarMotores(M1PWM, M1A, M2PWM, M2A);
  }

  while (loop_timer > micros());                // espero a que pasen LOOP_TIME_MS y se repite
  loop_timer += LOOP_TIME_MS;                   // preparo el tiempo límite del siguiente bucle

}
