

/*
   1 - MotoresInit
   2 - MoverMotores
   3 - ApagarMotores
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





