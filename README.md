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
- [ ] Pieza sostén de batería, LEDs y pulsadores.
- [ ] Software funcional del seguidor.

### Sobre el Hardware:
El hardware consta de dos placas, cuyos diseños están abiertos y libres, disponibles a través del programa "CircuitMaker". Los proyectos se llaman:
 - **LineFollowerCDR2018_SensorsBoard - ReverseLogic** : Placa con 7 sensores CNY70.
 - **LineFollowerCDR2018_MainBoard DIP**: Placa con el microcontrolador, el puente H y entrada de sensores.
 
### PINOUT del seguidor:
A continuación se muestra el PINOUT del seguidor ante cualquier cuestión de conexionado, revisión del código y asignación de pines, etc. Para las referencias del seguidor, considerar que es visto "desde atrás y desde arriba".

| PIN                    | Detalle/Función   			  		                     |
| -----------------------|:-------------------------------------------------:| 
| **A1 (D15)**           | Sensor extremo derecho (S1). Entrada digital.     |
| **A0**                 | Sensor S2. Entrada analógica.                     |
| **A2**                 | Sensor S3. Entrada analógica.                     |
| **A3**                 | Sensor S4. Entrada analógica.                     |
| **A4**                 | Sensor S5. Entrada analógica.                     |
| **A5**                 | Sensor S6. Entrada analógica.                     |
| **D4**                 | Sensor extremo izquierdo (S7). Entrada digital.   |

| PIN                    | Detalle/Función   			  		                        |
| -----------------------|:----------------------------------------------------:| 
| **D10**                | Control velocidad M1 (Motor derecho). Salida PWM.    |
| **D8**                 | Control sentido M1 (Motor derecho). Salida Digital.  |
| **D9**                 | Control velocidad M2 (Motor izquierdo). Salida PWM.  |
| **D7**                 | Control sentido M2 (Motor izquierdo). Salida Digital.|

### Conectores y PINOUT de los conectores:
Para que cualquiera pueda utilizar el seguidor y pueda conectar correctamente las interfaces se detalla cada conexionado con una foto explicativa.


### Sobre el Software:
En principio se planea realizar un único sketch de Arduino con la totalidad del código. Como estrategia se planea usar un proporcional-derivador. De los 7 sensores se planea utilizar los 5 centrales como entradas analógicas, y los otros 2 de las puntas como entradas digitales (para cambios muy abruptos). Como se cuenta con más de un pulsador pueden establecerse rutinas de calibración...por ejemplo, una rutina que lea los valores de cada sensor y almacene los valores máximos y mínimos, con esos valores se pueden remapear las lecturas para aprovechar todo el rango dinámico. El otro pulsador puede utilizarse para la "largada".

### TO DO:

- [ ] ......

### Fotos:
- Todavía no hay.

* * *
