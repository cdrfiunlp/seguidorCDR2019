

/*
   1 - MotoresInit
   2 - MoverMotores
   3 - ApagarMotores
   4 - SeguirLinea
*/



/*----------------------------------------------------------------------------------
    Función para inicializar los Motores
  ----------------------------------------------------------------------------------*/

void MotoresInit(byte motor1D, byte motor2D)
{
  pinMode(motor1D, OUTPUT);
  pinMode(motor2D, OUTPUT);
}

/*----------------------------------------------------------------------------------
    Función para usar PWM a 25 kHz en los pines 9 y 10 (timer 1). valor: entre 0 y 320
  ----------------------------------------------------------------------------------*/

void analogWrite25k(int pin, int valor)
{
  switch (pin) {
    case 9:
      OCR1A = valor;
      break;
    case 10:
      OCR1B = valor;
      break;
    default:
      // no other pin will work
      break;
  }
}

/*----------------------------------------------------------------------------------
    Función para mover los Motores con valores absolutos VelM1/2 : (-320; 320)
  ----------------------------------------------------------------------------------*/

void MoverMotores (byte motor1PWM, byte motor1D, byte motor2PWM, byte motor2D, int velocidadMotor1, int velocidadMotor2)
{
  if (velocidadMotor1 >= PWM_DEAD_ZONE) {  // si estoy en zona lineal, muevo el motor linealmente
    analogWrite25k(motor1PWM, velocidadMotor1);
    digitalWrite(motor1D, 0);
  }
  else if (velocidadMotor1 <= -PWM_DEAD_ZONE) { // idem pero en sentido contrario
    analogWrite25k(motor1PWM, 320 + velocidadMotor1);
    digitalWrite(motor1D, 1);
  }
  else if (velocidadMotor1 > 0) { // si estoy en la banda muerta pero positiva, voy y lo muevo la cantidad mínima
    analogWrite25k(motor1PWM, PWM_DEAD_ZONE);
    digitalWrite(motor1D, 0);
  }
  else {                          // idem pero en sentido contrario
    analogWrite25k(motor1PWM, 320 + PWM_DEAD_ZONE);
    digitalWrite(motor1D, 1);
  }

  if (velocidadMotor2 >= PWM_DEAD_ZONE) {  // si estoy en zona lineal, muevo el motor linealmente
    analogWrite25k(motor2PWM, velocidadMotor2);
    digitalWrite(motor2D, 0);
  }
  else if (velocidadMotor2 <= -PWM_DEAD_ZONE) { // idem pero en sentido contrario
    analogWrite25k(motor2PWM, 320 + velocidadMotor2);
    digitalWrite(motor2D, 1);
  }
  else if (velocidadMotor1 > 0) { // si estoy en la banda muerta pero positiva, voy y lo muevo la cantidad mínima
    analogWrite25k(motor2PWM, PWM_DEAD_ZONE);
    digitalWrite(motor2D, 0);
  }
  else {
    analogWrite25k(motor2PWM, 320 + PWM_DEAD_ZONE);
    digitalWrite(motor2D, 1);
  }
}

/*-------------------------------------------------------------
    Función para apagar los Motores
  -------------------------------------------------------------*/
void ApagarMotores(byte motor1PWM, byte motor1D, byte motor2PWM, byte motor2D)
{
  analogWrite25k(motor1PWM, 320);
  analogWrite25k(motor2PWM, 320);
  digitalWrite(motor1D, 1);
  digitalWrite(motor2D, 1);
}

/*
    SeguirLinea; Mueve los motores seg�n la salida de LecturaSensores2.
*/

void SeguirLinea(byte motor1PWM, byte motor1D, byte motor2PWM, byte motor2D, int VelFiltrada, int Vel)
{

  int M1Vel = 0;
  int M2Vel = 0;

  M1Vel = Vel - VelFiltrada; // M1 Der. visto desde arriba y desde atrás.
  M2Vel = Vel + VelFiltrada; // M2 Der. visto desde arriba y desde atrás..

  if (M1Vel > 0)
  {
    LEDsDrive(1, 0);
  }
  else
  {
    LEDsDrive(0, 1);
  }

  M1Vel = constrain(M1Vel, -VEL_MAX, VEL_MAX);
  M2Vel = constrain(M2Vel, -VEL_MAX, VEL_MAX);

  MoverMotores(motor1PWM, motor1D, motor2PWM, motor2D, M1Vel, M2Vel);

}





