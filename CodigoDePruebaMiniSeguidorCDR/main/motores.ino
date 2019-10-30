

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
    Función para apagar los Motores con valores absolutos VelM1/2 : (-255; 255)
  ----------------------------------------------------------------------------------*/

void MoverMotores (byte motor1PWM, byte motor1D, byte motor2PWM, byte motor2D, int velocidadMotor1, int velocidadMotor2)
{
  if (velocidadMotor1 > 0)
  {
    analogWrite(motor1PWM, velocidadMotor1);
    digitalWrite(motor1D, 0);
  } else {
    analogWrite(motor1PWM, 255 + velocidadMotor1);
    digitalWrite(motor1D, 1);
  }

  if (velocidadMotor2 > 0)
  {
    analogWrite(motor2PWM, velocidadMotor2);
    digitalWrite(motor2D, 0);
  } else  {
    analogWrite(motor2PWM, 255 + velocidadMotor2);
    digitalWrite(motor2D, 1);
  }
}

/*-------------------------------------------------------------
    Función para apagar los Motores
  -------------------------------------------------------------*/
void ApagarMotores(byte motor1PWM, byte motor1D, byte motor2PWM, byte motor2D)
{
  analogWrite(motor1PWM, 255);
  analogWrite(motor2PWM, 255);
  digitalWrite(motor1D, 1);
  digitalWrite(motor2D, 1);
}

/*
    SeguirLinea; Mueve los motores seg�n la salida de LecturaSensores2.
*/

void SeguirLinea(byte motor1PWM, byte motor1D, byte motor2PWM, byte motor2D, int VelFiltrada, int Vel, int delay_ms)
{ 

    int M1Vel = 0;
    int M2Vel = 0;


    //M1Vel = Vel-VelFiltrada+5; // M1 Izq. visto de arriba. Debo sumarle un poco más por diferencias de motores.
    //M2Vel = Vel+VelFiltrada-13; // M2 Der. visto de arriba. Debo restarle un poco más por diferencias de motores.

    M1Vel = Vel-VelFiltrada; // M1 Izq. visto de arriba. Debo sumarle un poco más por diferencias de motores.
    M2Vel = Vel+VelFiltrada; // M2 Der. visto de arriba. Debo restarle un poco más por diferencias de motores.
          
        if(M1Vel > 0)
        {
          LEDsDrive(1,0);          
        }
        else
        {
          LEDsDrive(0,1); 
        }
        
        //if(M1Vel > VEL_MAX){M1Vel = VEL_MAX; M2Vel = -VEL_MAX;}
        //if(M2Vel > VEL_MAX){M2Vel = VEL_MAX; M1Vel = -VEL_MAX;}
        //if(M1Vel < -VEL_MAX){M1Vel = -VEL_MAX;M2Vel = VEL_MAX;}
        //if(M2Vel < -VEL_MAX){M2Vel = -VEL_MAX;M1Vel = VEL_MAX;}
        
        M1Vel = constrain(M1Vel,-VEL_MAX,VEL_MAX);
        M2Vel = constrain(M2Vel,-VEL_MAX,VEL_MAX);
        
        MoverMotores(motor1PWM, motor1D, motor2PWM, motor2D, M1Vel, M2Vel); 
		
	delay(delay_ms);
                                                               
        
}


 
 
 
