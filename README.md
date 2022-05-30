# Sensor remoto y autónomo de ruido ambiental 
Este dispositivo se ha diseñado para monitorizar el ruido ambiental de forma indefinida utilizando energia solar. 

Por otra parte se van a utilizar comunicaciones de largo alcance tipo LPWAN (Low Power Wide Area Networks). En este caso será una modluación **LoRa** (Long Range) que además es muy eficientes en el consumo de energía. La tecnología LoRa y la capa de red abierta LoRaWAN son los protagonistas del despliegue del IoT.

## Material utilizado
- Hemos utilizado la [placa Heltec HTCC-AB01](https://es.aliexpress.com/item/1005001288501966.html) para la banda ISM europea de 868MHz que tiene las siguientes caracteristicas:
  - Compatible con Arduino, por tanto compatible en Windows, Mac OS y Linux
  - Configurable a través de comandos AT
  - Chip CP2102 de puerto serie a USB integrado
  - Basada en ASR605x (ASR6501, ASR6502) , esos chips ya están integrados el PSoC ®  4000 serie MCU (ARM ®  cortex ® M0 + Core) y SX1262;
  - Soporte de la especificación LoRaWAN 1.0.2
  - Diseño de energía ultra bajo, sólo 3.5uA en Deep Sleep. Para este tipo de proyecto, el consumo son muy importantes y aquí tenemos es tabla que ayudará a dimensionar la bateria y el panel solar:
   ![Consumos](./img/cubecell_htcc-ab01_power.png)
  - Sistema de gestión de energía solar incluido, se puede conectar directamente con un panel solar de 5-7 V
  - Conector SH1.25-2 para baterías incluido, sistema de administración de batería integrado (administración de carga y descarga, protección de sobrecarga, detección de energía de la batería, conmutación automática de energía de batería / USB)
  - Interfaz micro USB con protección ESD (Electrostatic Discharge), protección contra cortocircuitos, blindaje RF y otras medidas de protección
  - Buena correspondencia de impedancia y larga distancia de comunicación
  - Certificación CE y FCC

<img src="./img/boardAB01.png" width="500" align="center" />

Podremos encontrar toda la documentación de esta placa en: [Documentos y ficheros](https://resource.heltec.cn/download/CubeCell), [FAQs](https://github.com/HelTecAutomation/HeltecDocs/blob/master/en/source/cubecell/frequently_asked_questions.md) 

- [Batería de polímero de litio de 3,7V y conector JST SH1.0 de 1,0mm y dos pines](https://es.aliexpress.com/item/4000288987647.html)

<img src="./img/Battery_LiPo37Vph2.png" width="500" align="center" />

- [Panel solar de 6V](https://es.aliexpress.com/item/4001128543657.html)

<img src="./img/SolarPanel.png" width="300" align="center" />

- [SparkFun Sound Detector](https://www.sparkfun.com/products/14262)

<img src="./img/IP66_case.png" width="300" align="center" />

- [Carcasa hermética IP66](https://es.aliexpress.com/item/33060319519.html)

<img src="./img/SparkFun_SoundDetector.jpeg" width="300" align="center" />

## Conexionado

Del microcontrolador CubeCell sólo vamos a utilizar el pin 2 que corresponde al único conversor analógico/digital de la placa. Intermante es el mismo que utiliza para ver el estado de carga de la bateria. Por lo que perderemos esta función.  

<img src="./img/cubecell_htcc-ab01_pinout.png"  align="center" />

Otro detalle a tener en cuenta respecto a la alimentación del sensor de sonido, es que aunque sería recomendable utilizar una tensió de 5V que suministraria la placa por el pin VIN. Este sólo tiene tensión cuando se alimenta por USB. Por lo que tendremos que utilizar el pin VDD que suministra 3.3 en la configuración de bateria y son suficientes para que el sensor funciones correctamente.

<img src="./img/jaleometro_bb.png"  align="center" />

## Configuración IDE Arduino
Lo para poder programar adecuadamente el dispositivo tendremos que configurar el entorno de programación de Arduino con las configuraciones de hardware y librerias de esta placa.
1. Añadir la URL https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.4.0/package_CubeCell_index.json a Preferences->Settings->Additional boards:
2. 

## Agradecimientos
https://learn.sparkfun.com/tutorials/sik-experiment-guide-for-the-arduino-101genuino-101-board-spanish/experimento-15-usar-la-placa-de-detector-de-sonido

