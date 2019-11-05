# seguidorCDR2019

### Descripción:

El repositorio contiene el software del seguidorCDR2019 (hasta que le pongamos nombre). A continuación se deja información útil.

### Integrantes:

| Integrante             | Tareas           			  		           |
| -----------------------|:---------------------------------------:|
| **Santiago Rodríguez** | Diseño de placas. Soldado. Software?    |
| **Juan C. Scatuerchio**| Diseño 3D chasis. Armardo.              |

### Estado del proyecto:
- [x] Diseño de Hardware de placa de sensores y placa controladora (micro y puente H).
- [x] Placas soldadas y probadas.
- [x] Chasis impreso y probado.
- [x] Pieza sostén de batería, LEDs y pulsadores.
- [x] (BETA) Software funcional del seguidor.

### Sobre el Hardware:
El hardware consta de dos placas, cuyos diseños están abiertos y libres, disponibles a través del programa "CircuitMaker" y puede accederse a los mismos a través de "Projects" y buscar el proyecto por el nombre. Los proyectos se llaman:
 - **LineFollowerCDR2018_SensorsBoard - ReverseLogic** : Placa con 7 sensores CNY70.
 - **LineFollowerCDR2018_MainBoard DIP**: Placa con el microcontrolador, el puente H y entrada de sensores.

### PINOUT del seguidor:
A continuación se muestra el PINOUT del seguidor ante cualquier cuestión de conexionado, revisión del código y asignación de pines, etc. Para las referencias del seguidor, considerar que es visto "desde atrás y desde arriba".

| PIN MICRO              | Detalle/Función   			  		             |
| -----------------------|:-------------------------------------------------:|
| **A1 (D15)**           | Sensor extremo derecho (S1). Entrada digital.     |
| **A0**                 | Sensor S2. Entrada analógica.                     |
| **A2**                 | Sensor S3. Entrada analógica.                     |
| **A3**                 | Sensor S4. Entrada analógica.                     |
| **A4**                 | Sensor S5. Entrada analógica.                     |
| **A5**                 | Sensor S6. Entrada analógica.                     |
| **D4**                 | Sensor extremo izquierdo (S7). Entrada digital.   |

| PIN MICRO |                    Detalle/Función                    |
| --------- | :---------------------------------------------------: |
| **D10**   |   Control velocidad M1 (Motor derecho). Salida PWM.   |
| **D8**    |  Control sentido M1 (Motor derecho). Salida Digital.  |
| **D9**    |  Control velocidad M2 (Motor izquierdo). Salida PWM.  |
| **D7**    | Control sentido M2 (Motor izquierdo). Salida Digital. |

| PIN MICRO |                     Detalle/Función                     |
| --------- | :-----------------------------------------------------: |
| **D6**    |                 **Pulsador de "start"**                 |
| **D5**    | **Interruptor de selección de modo de funcionamiento.** |
| **D12**   |            **MISO / LED1 verde/rojo (ver)**             |
| **D12**   |            **MOSI / LED2 verde/rojo (ver)**             |

| PIN MICRO |                   Detalle/Función                    |
| --------- | :--------------------------------------------------: |
| **D0**    |      Rx del ATMEGA328. Tx de eventual USB-UART.      |
| **D1**    |      Tx del ATMEGA328. Rx de eventual USB-UART.      |
| **D3**    | Rx auxiliar del ATMEGA328 (soft). Tx de eventual BT. |
| **D2**    | Tx auxiliar del ATMEGA328 (soft). Rx de eventual BT. |

### Conectores y PINOUT de los conectores:
Para que cualquiera pueda utilizar el seguidor y pueda conectar correctamente las interfaces se detalla cada conexionado con una foto explicativa. En caso de que se desee armar otra placa también se deja una foto de la capa inferior, donde hay algunos componentes SMD. Por cualquier cosa, el esquemático se encuentra en el siguiente [Link](hardware\LineFollowerCDR2018_MainBaord DIP/Esquematico.pdf).

3D Vista TOP             | 3D Vista BOTTOM 
:-------------------------:|:-------------------------:
![](hardware/LineFollowerCDR2018_MainBaord%20DIP/Imagenes/MainBoardTOP.png) |  ![](hardware/LineFollowerCDR2018_MainBaord%20DIP/Imagenes/MainBoardBOTTOM.png)


Donde en la siguiente tabla se detalla la funcionalidad de cada conector:

| CONECTOR               | Detalle/Función   			  		                |
| -----------------------|:----------------------------------------------------:|
| **P1**                 | Entrada de tensión de 7.4V de la batería.			|
| **P2**                 | Pads para soldar un interruptor para ON/OFF.			|
| **P3**                 | Conector de programación ISP (en desuso por ahora).  |
| **P4**                 | Conector de programación con USB-SERIAL (CP2102).    |
| **P5**                 | Conector de entrada de sensores CNY70 y alimentación.|
| **J1**                 | Conector para módulo BT HC-05.					    |
| **J2**                 | Salida para el motor M1.								|
| **J3**                 | Salida para el motor M2.								|
| **D2**                 | LED rojo/verde (VER).								|
| **D3**                 | LED rojo/verde (VER).								|
| **SW1**                | Pulsador del RESET del micro.						|
| **SW2**                | Conector para pulsador de start.						|
| **SW2**                | Conector para interruptor de selección de modo.   	|

También se muestra la placa de sensores en su vista superior e inferior para eventuales dudas sobre cómo se colocan las resistencias o para saber a qué sensor corresponde cada pista del circuito. También se encuentra disponible el esquemático en el siguiente [Link](hardware\LineFollowerCDR2018_SensorsBoard - ReverseLogic/Esquematico.pdf).

![Placa de sensores 3D TOP.](hardware/LineFollowerCDR2018_SensorsBoard%20-%20ReverseLogic/Imagenes/SensorBoardTOP.png)

![Placa de sensores 3D BOTTOM.](hardware/LineFollowerCDR2018_SensorsBoard%20-%20ReverseLogic/Imagenes/SensorBoardBOTTOM.png)

### Sobre el Software:
En principio se planea realizar un único sketch de Arduino con la totalidad del código. Como estrategia se planea usar un proporcional-derivador. De los 7 sensores se planea utilizar los 5 centrales como entradas analógicas, y los otros 2 de las puntas como entradas digitales (para cambios muy abruptos). Como se cuenta con más de un pulsador pueden establecerse rutinas de calibración...por ejemplo, una rutina que lea los valores de cada sensor y almacene los valores máximos y mínimos, con esos valores se pueden remapear las lecturas para aprovechar todo el rango dinámico. El otro pulsador puede utilizarse para la "largada".

### TO DO:

- [ ] ......

### Fotos:
- Todavía no hay.

* * *
