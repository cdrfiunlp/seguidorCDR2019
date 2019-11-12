 #define ERROR_CASE_1 0.0
 #define ERROR_CASE_2 10.0
 #define ERROR_CASE_3 20.0
 #define ERROR_CASE_4 30.0
 #define ERROR_CASE_5 50.0
 #define ERROR_CASE_6 -10.0
 #define ERROR_CASE_7 -20.0
 #define ERROR_CASE_8 -30.0
 #define ERROR_CASE_9 -50.0
 #define ERROR_MAX 80.0
 #define ERROR_MIN -80.0
 #define DELTA_ERROR_MAX 10.0
 #define DELTA_ERROR_MIN -10.0

/*#define ERROR_CASE_1 0.0
#define ERROR_CASE_2 5
#define ERROR_CASE_3 20.0
#define ERROR_CASE_4 40.0
#define ERROR_CASE_5 100.0
#define ERROR_CASE_6 -5.0
#define ERROR_CASE_7 -20.0
#define ERROR_CASE_8 -40.0
#define ERROR_CASE_9 -100.0
#define ERROR_MAX 100.0
#define ERROR_MIN -100.0
#define DELTA_ERROR_MAX 20.0
#define DELTA_ERROR_MIN -20.0*/


#define DELAY_REVERSA 150 // en ms (probar 100, 50, 25)

int LecturaSensores2(char mode, char serial, float* error, float* error_ant, unsigned long* LastTime2, byte* flags)
{
  int sensor[5] = {0};
  int umbral    = 800;
  byte palabra  = 0x00;

  sensor[0] = analogRead(S1);
  sensor[1] = analogRead(S2);
  sensor[2] = analogRead(S3);
  sensor[3] = analogRead(S4);
  sensor[4] = analogRead(S5);

  for (int i = 0; i <= 4; i++)
  {
    if (sensor[i] >= umbral)
    {
      palabra = palabra | (0x01 << i);
    }
  }

  switch (palabra)
  {
  case 0B00000100:
    //case 0x04:  // CASO 1
    *flags = 0x01;
    *error_ant = *error;
    *error = ERROR_CASE_1;
    break;

  case 0B00001100:
    //case 0x0C:  // CASO 2
    if (*flags != 0x02)
    {
      *flags = 0x02;
      *error_ant = *error;
      *error = ERROR_CASE_2;
    }
    if ((*flags == 0x02) && (*error < ERROR_CASE_3)) // si ya habia entrado antes, voy acumulando
    {
      *error_ant = *error;
      *error += 1;
    }
    break;

  case 0B00001000:
    //case 0x08:  // CASO 3
    if (*flags != 0x03) // flag correspondiente al caso
    {
      *flags = 0x03; // flag = TRUE
      *error_ant = *error;
      *error = ERROR_CASE_3;
    }
    if ((*flags == 0x3) && (*error < ERROR_CASE_4)) // si ya habia entrado antes, voy acumulando
    {
      *error_ant = *error;
      *error += 1;
    }
    break;

  case 0B00011000:
    //case 0x18:  // CASO 4
    if (*flags != 0x04)
    {
      *flags = 0x04; // flag = TRUE
      *error_ant = *error;
      *error = ERROR_CASE_4;
    }
    if ((*flags == 0x04) && (*error < ERROR_CASE_5)) // si ya habia entrado antes, voy acumulando
    {
      *error_ant = *error;
      *error += 1;
    }
    break;

  case 0B00010000:
    //case 0x10: // CASO 5
    if (*flags != 0x05)
    {
      *flags = 0x05; // flag = TRUE
      *error_ant = *error;
      *error = ERROR_CASE_5;
    }
    if ((*flags == 0x05) && (*error < ERROR_MAX)) // si ya habia entrado antes, voy acumulando
    {
      *error_ant = *error;
      *error += 1;
    }
    break;

  case 0B00000110:	
    //case 0x06: // CASO 6
    if (*flags != 0x06)
    {
      *flags = 0x06; // flag = TRUE
      *error_ant = *error;
      *error = ERROR_CASE_6;
    }
    if ((*flags == 0x06) && (*error > ERROR_CASE_7)) // si ya habia entrado antes, voy acumulando
    {
      *error_ant = *error;
      *error -= 1;
    }
    break;

  case 0B00000010:	
    //case 0x02: // CASO 7
    if (*flags != 0x07)
    {
      *flags = 0x07; // flag = TRUE
      *error_ant = *error;
      *error = ERROR_CASE_7;
    }
    if ((*flags == 0x07) && (*error > ERROR_CASE_8)) // si ya habia entrado antes, voy acumulando
    {
      *error_ant = *error;
      *error -= 1;
    }
    if ((*flags == 0x07) && (*error == ERROR_CASE_8)) // hasta llegar al error del caso siguiente
    {
      *error_ant = *error;
      *error = ERROR_CASE_8;
    }
    break;

  case 0B00000011:
    //case 0x03: // CASO 8
    if (*flags != 0x08) // flag correspondiente al caso
    {
      *flags = 0x08; // flag = TRUE
      *error_ant = *error;
      *error = ERROR_CASE_8;
    }
    if ((*flags == 0x08) && (*error > ERROR_CASE_9)) // si ya habia entrado antes, voy acumulando
    {
      *error_ant = *error;
      *error -= 1;
    }
    break;

  case 0B00000001:
    //case 0x01: // CASO 9
    if (*flags != 0x09)
    {
      *flags = 0x09; // flag = TRUE
      *error_ant = *error;
      *error = ERROR_CASE_9;
    }
    if ((*flags == 0x09) && (*error > ERROR_MIN)) // si ya habia entrado antes, voy acumulando
    {
      *error_ant = *error;
      *error -= 1;
    }
    break;

    /*   case 0x0A:  // CASOS ESPECIALES....CAPAZ NO LOS PONGO
     error_ant = *error;
     error = 0;
     break;
     
     case 0x11:    // CASOS ESPECIALES....CAPAZ NO LOS PONGO
     error_ant = *error;
     error = 0;
     break;*/

  case 0x00:  // si me salgo, vuelvo a entrar...
    MoverMotores(M1PWM, M1A, M2PWM, M2A, -50, -50);
    delay(DELAY_REVERSA);
    break;

  case 0x1F:
    *error_ant = *error;
    *error = ERROR_CASE_1;
    break;

  default:
    *error_ant = *error;
    *error = ERROR_CASE_1;
    break;

  }

  float delta_error = *error - *error_ant;
  if (delta_error >= DELTA_ERROR_MAX)delta_error = DELTA_ERROR_MAX; // "bumpless"
  if (delta_error <= DELTA_ERROR_MIN)delta_error = DELTA_ERROR_MIN; // "bumpless"
  float pwm_filtrado = (*error * KP) + delta_error * KD;

  unsigned long Now2 = micros();
  unsigned long dt2 = Now2 - *LastTime2;

  if ((serial == 1) && (dt2 > DT2))
  {
    /*Serial.println("----");
     Serial.println(sensor[0]);
     Serial.println(sensor[1]);
     Serial.println(sensor[2]);
     Serial.println(sensor[3]);
     Serial.println(sensor[4]);*/
    /*Serial.print("Estados sensores     ");
     Serial.println(palabra, BIN);
     Serial.println();*/
    Serial.print("Error[k]:     ");
    Serial.println(*error);
    Serial.println();
    Serial.print("Error[k-1]:     ");
    Serial.println(*error_ant);
    Serial.println();
    Serial.print("PWM filtrado[k]:     ");
    Serial.println(pwm_filtrado);
    Serial.println();
    Serial.println("----");
    *LastTime2 = Now2;
  }

  return (int)pwm_filtrado;

}
