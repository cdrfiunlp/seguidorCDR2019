
int lecturaSensores(char serial, int* SensorMin, int* SensorMax)
{
  int sensor[5] = {0};
  int sumasensor = 0;
  int error = 0;

  /*----- leo los sensores y los re-mapeo ------ */
  sensor[0] = map(analogRead(S1), *(SensorMin), *(SensorMax), 0, 255); SensorMin++; SensorMax++;
  sensor[1] = map(analogRead(S2), *(SensorMin), *(SensorMax), 0, 255); SensorMin++; SensorMax++;
  sensor[2] = map(analogRead(S3), *(SensorMin), *(SensorMax), 0, 255); SensorMin++; SensorMax++;
  sensor[3] = map(analogRead(S4), *(SensorMin), *(SensorMax), 0, 255); SensorMin++; SensorMax++;
  sensor[4] = map(analogRead(S5), *(SensorMin), *(SensorMax), 0, 255);
  /*----- para que no haya valores anómalos ------ */
  sensor[0] = constrain(sensor[0], 0, 255);
  sensor[1] = constrain(sensor[1], 0, 255);
  sensor[2] = constrain(sensor[2], 0, 255);
  sensor[3] = constrain(sensor[3], 0, 255);
  sensor[4] = constrain(sensor[4], 0, 255);
   /*----- suma de mediciones para normalizar ------ */
  sumasensor = sensor[0] + sensor[1] + sensor[2] + sensor[3] + sensor[4];
  sumasensor /= 5;
  /*----- Media pesada con pesos con separación de 16 ------ */
  error = ((sensor[0] << 6) + (sensor[1] << 5) - (sensor[3] << 5) - (sensor[4] << 6)) / sumasensor;

  /*----- Envío de datos para debug ------ */
  if (serial == 1)
  {
    Serial.print("S2\t");
    Serial.print("S3\t");
    Serial.print("S4\t");
    Serial.print("S5\t");
    Serial.print("S6\t");
    Serial.print("S2 min \t");
    Serial.print("S2 max \t");
    Serial.print("SUMA\t");
    Serial.println("ERROR\t");
    Serial.print(sensor[0]); Serial.print("\t");
    Serial.print(sensor[1]); Serial.print("\t");
    Serial.print(sensor[2]); Serial.print("\t");
    Serial.print(sensor[3]); Serial.print("\t");
    Serial.print(sensor[4]); Serial.print("\t");
    Serial.print(SensorMin[0]); Serial.print("\t");
    Serial.print(SensorMax[0]); Serial.print("\t");
    Serial.print(sumasensor); Serial.print("\t");
    Serial.print(error);     Serial.println("\t");
  }

  return error;  
}

/*----------------------------------------------------------------------------------
    Función para calibrar los sensores.
  ----------------------------------------------------------------------------------*/

void CalibrarSensores(int* sensormin, int* sensormax)
{

  int* pSensorMin = sensormin;
  int* pSensorMax = sensormax;

  int Sensor[5] = {0};

  Sensor[0] = analogRead(S1);
  Sensor[1] = analogRead(S2);
  Sensor[2] = analogRead(S3);
  Sensor[3] = analogRead(S4);
  Sensor[4] = analogRead(S5);

  for (int p = 0; p < 5; p++)
  {
    if (Sensor[p] > *pSensorMax)
    {
      *pSensorMax = Sensor[p];
    }
    if (Sensor[p] < *pSensorMin)
    {
      *pSensorMin = Sensor[p];
    }
    pSensorMax++;
    pSensorMin++;
  }
}
