# <img src="./img/jaleometro_logo_min_transparent.png" />  Sensor remoto y autónomo de ruido ambiental 
Este dispositivo se ha diseñado para monitorizar el ruido ambiental de forma indefinida utilizando energía solar. 

Por otra parte se van a utilizar comunicaciones de largo alcance tipo LPWAN (Low Power Wide Area Networks). En este caso será una modulación **LoRa** (Long Range) que además es muy eficientes en el consumo de energía. La tecnología LoRa y la capa de red abierta LoRaWAN son los protagonistas del despliegue del IoT.

## Material utilizado
- Hemos utilizado la [placa Heltec HTCC-AB01](https://es.aliexpress.com/item/1005001288501966.html) para la banda ISM europea de 868MHz que tiene las siguientes características:
  - Compatible con Arduino, por tanto compatible en Windows, Mac OS y Linux
  - Configurable a través de comandos AT
  - Chip CP2102 de puerto serie a USB integrado
  - Basada en ASR605x (ASR6501, ASR6502) , esos chips ya están integrados el PSoC ®  4000 serie MCU (ARM® cortex® M0 + Core) y SX1262;
  - Soporte de la especificación LoRaWAN 1.0.2
  - Diseño de energía ultra bajo, solo 3.5uA en Deep Sleep. Para este tipo de proyecto, el consumo son muy importantes y aquí tenemos es tabla que ayudará a dimensionar la batería y el panel solar:
   ![Consumos](./img/cubecell_htcc-ab01_power.png)
  - Sistema de gestión de energía solar incluido, se puede conectar directamente con un panel solar de 5-7 V
  - Conector SH1.25-2 para baterías incluido, sistema de administración de batería integrado (administración de carga y descarga, protección de sobrecarga, detección de energía de la batería, conmutación automática de energía de batería / USB)
  - Interfaz micro USB con protección ESD (Electrostatic Discharge), protección contra cortocircuitos, blindaje RF y otras medidas de protección
  - Buena correspondencia de impedancia y larga distancia de comunicación
  - Certificación CE y FCC

<img src="./img/boardAB01.png" width="500" align="center" />

Podremos encontrar toda la documentación de esta placa en: [Documentos y ficheros](https://resource.heltec.cn/download/CubeCell), [FAQs](https://github.com/HelTecAutomation/HeltecDocs/blob/master/en/source/cubecell/frequently_asked_questions.md) 

- [Batería de polímero de litio de 3,7V y conector JST SH1.0 de 1,0mm y dos pines](https://es.aliexpress.com/item/4000288987647.html)

<img src="./img/Battery_LiPo37Vph2.png" width="350" align="center" />

- [Panel solar de 6V](https://es.aliexpress.com/item/4001128543657.html)

<img src="./img/SolarPanel.png" width="350" align="center" />

- [SparkFun Sound Detector](https://www.sparkfun.com/products/14262)

<img src="./img/SparkFun_SoundDetector.jpeg" width="300" align="center" />

- [Carcasa hermética IP66](https://es.aliexpress.com/item/33060319519.html)

<img src="./img/IP66_case.png" width="400" align="center" />

Por otra parte también se necesitarán otros componentes no electrónicos como cables, mini-protoboard... lo que conforma este kit:

<img src="./img/jaleometro_kit.jpg" align="center" />

## Conexionado

Del microcontrolador CubeCell solo vamos a utilizar el pin 2 que corresponde al único conversor analógico/digital de la placa. Internamente es el mismo que utiliza para ver el estado de carga de la batería. Por lo que perderemos esta función.  

<img src="./img/cubecell_htcc-ab01_pinout.png"  align="center" />

Otro detalle a tener en cuenta respecto a la alimentación del sensor de sonido, es que aunque sería recomendable utilizar una tensión de 5V que suministraría la placa por el pin VIN. Este solo tiene tensión cuando se alimenta por USB. Por lo que tendremos que utilizar el pin VDD que suministra 3.3 en la configuración de batería y son suficientes para que el sensor funciones correctamente.

<img src="./img/jaleometro_bb.png"  align="center" />

### Tuneo del sensor de Sparkfun
Las pruebas realizadas con la v0 revelaron falta de sensibilidad. El sensor permite esta ajuste por hardware soldando una resistencia en este punto:

<img src="./img/sensor_sparkfun_sensor_position.jpeg" width="300"  align="center" />

Los valores posibles están en esta tabla:

<img src="./img/sensor_sparkfun_resistor_table.png"  align="center" />

Y finalmente en nuestro caso elegimos la de 1M para obtener una ganancia de 60 dB:

<img src="./img/sensor_sparkfun_sensor_resistor.png" width="300"  align="center" />

<img src="./img/BarnizadoPCB.jpg" width="400"  align="right" />

Debido a que el sensor estará fuera de la caja estanca. Aunque protegido por material plástico. Requiere de una protección especial a fin de retardar todo lo posible la oxidación de sus componentes. Hay muchas soluciones, pero hemos elegido la más artesanal usando resina colofonia disuelta al 40% en alcohol isopropílico. La solución se puede aplicar con pincel y el tiempo de secado es de unas 24 horas.

## Configuración IDE Arduino
Lo para poder programar adecuadamente el dispositivo tendremos que configurar el entorno de programación de Arduino con las configuraciones de hardware y librerías de esta placa.
1. Añadir la URL https://github.com/HelTecAutomation/CubeCell-Arduino/releases/download/V1.5.0/package_CubeCell_index.json a Preferences->Settings->Additional boards:
2. Añadir la librería de CayenneLPP. **Importante: versión 1.1.0**:

<img src="./img/lib_cayenne.png"  align="center" />

3. Si aparece un aviso de instalar otras librerias con dependencias. Seleccionar opción de instalar todas:

<img src="./img/library_dependencies.png"  align="center" />

4. Seleccionar la tarjeta CubeCell HTCC-AB01 y configurar todos sus parámetros así:

<img src="./img/arduino_board_setup_cubecell.png"  align="center" />

### Subir el sketch de Arduino
Con el cable conectado seleccionamos el puerto USB que nuestro ordenador haya asignado. Una vez compilado y cargado en el dispositivo. Abrimos el monitor serie para comprobar que log de actividad del programa es correcto.

## Configuración del sensor en la nube (TTN)

Vamos a utilizar los servicios de The Things Network que es un servicio en la nube al que están conectados los _gateway_ libres de la las Comunidades TTN. 
La ruta de la información es la siguiente:
1. El sensor (_nodo_) transmite por radio con modulación LoRa el trama de datos que recibe el _gateway_
2. Este _gateway_ está conectado a internet para retransmitir la trama, hora como paquete de datos.
3. Los servidores de TTN decodifican el mensaje y ofrecen diferentes integraciones y _endpoints_ para que nuestras aplicaciones utilicen la información leída por los sensores.

Hay muchas variantes para implementar este enrutamiento. Para este se ha elegido estas configuraciones:
- Dispositivo es del tipo ABP (Activation-by-personalisation) lo que significa que se identificará en la red con un _DevAddr_ y una _Session key_ preconfigurada. Para ello tenemos que completar el registro de una aplicación y un dispositivo. 
- Publicación del paquete de-codificado en una cola MQTT a la que nuestra aplicación está subscrita.

<img src="./img/TTN_p1.jpg" width="200" align="left" />

Estos son los pasos a seguir empezando por acceder a la aplicación _back-end_ de TTN en esta URL: https://console.cloud.thethings.network/ en la que seleccionaremos nuestra región y luego nos loguearemos con nuestro usuario registrado.

<br>
<br>
<br>
<br>
<br>
<br>
<br>

### Registro de la aplicación
En TTN los dispositivos (_devices_) iguales, se agrupan en una aplicación desde la cual serán registrados. por lo tanto, primero hay que añadir una aplicación:

En el formulario de alta de aplicación rellenaremos estos campos: 
- Para el _Application ID_, elige un identificador único, en minúsculas, puedes usar caracteres alfanuméricos peor no guiones ´-´ consecutivos.
- Para  _Description_, elige la descripción que prefieras.
- No hace falta poner nada en _Application EUI_ 
- Presiona _Create application_ para finalizar

<img src="./img/TTN_p2.jpg" width="250" align="left" />
<img src="./img/TTN_p3.jpg" width="250" align="left" />
<img src="./img/TTN_p4.jpg" width="300" align="right" />

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

Aparecerá ahora la página con la nueva aplicación añadida donde encontrarás un botón de _+ Add end device_ necesario para crear los dispositivos asociados. 

<img src="./img/TTN_app_created.png" align="center" />

### Registro del dispositivo
En TTN un dispositivo (devide) representa la configuración de lo que también llama nodo (node) que a fin de cuentas es nuestro sensor. 
Al acceder al formulario de registro nos quedamos en la pestaña de _From The LoRaWAN Device Repository_ ya que estamos usando un microcontrolador conocido.
Ahora tenemos que seleccionar el tipo, modelo y marca de nuestro sensor tal y como aparece en la captura inferior. 
<img src="./img/TTN_device.png"  align="center" />

El la casilla inferior vemos que pide una clave de JoinEUI que normalmente nos la daría el fabricante. Pero en nuestro caso utilizaremos una sencilla aplicación web para generarla:  https://descartes.co.uk/CreateEUIKey.html
<img src="./img/Create_random_EUI.png"  align="center" />

Lo siguiente será generar las _DevEUI_ y _AppKey_ pulsando los correspondientes iconos de la derecha para que se genere automáticamente.
Finalmente queda rellenar el _Device ID_ que será el nombre único de este nodo.

<img src="./img/TTN_device_keys.png"  align="center" />

Finalmente pulsaremos _Register en device_ para que, seguidamente aparezca la pantalla con la configuración de nuestro nuevo nodo.Aquí podremos ver los datos de configuración y parámetros que necesitamos por ser un dispositivo de tipo ABP. Y que tendremos que pasar al fichero de configuración settings.h que se cargará en el _sketch_ del IDE de Arduino.

<img src="./img/TTN_device_created.png"  align="center" />

Para poder copiar las _keys_ en el formato "array" que se requiere en el fichero de configuración: "settings.h" 
Nota: hay que pulsar el icono <> de "_toggle array formating_" para tener el formato adecuado para este fichero.

```cpp
// LoRaWAN Configuration

/* OTAA para*/
static uint8_t devEui[] = {  };
static uint8_t appEui[] = {  };
static uint8_t appKey[] = {  };

uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };
static DeviceClass_t lorawanClass = LORAWAN_CLASS;

// Devise location
const float latitude = 39.573;
const float longitude = 2.732;
const int alt = 20;

// Other params
float SensorId= 10.01;// Sensor  identifcator number 
const int DutyCycle = 600000; // Transmision and reading period
const int Sleep4NoNoise = 300000; // Sleep period when low noise 
const int LowNoiseLevel = 180; // Limit for low noise mode
```

### Formato de la trama

Desde el menú vertical izquierdo accederemos al _Payload Formatters_ y la opción Uplink para definir cómo decodificar la trama de datos de nuestro mensaje LoRa que hemos establecido en el firmware. En nuestro caso hemos usado la librería CayenneLPP y esto es lo que aquí tenemos de seleccionar como tipo:

<img src="./img/TTN_app_uplink_format.png" width="400" align="center" />

De esta manera el mensaje MQTT que vamos a utilizar para leer los valores del sensor; será generado por backend de TTN en una estructura JSON perfectamente formateada:

```json
{
 "uplink_message":{
      "f_port":2,
      "f_cnt":539,
      "frm_payload":"AQID6QFlAE8CZQDDAogGCGAAaiwAC7g=",
      "decoded_payload":{
         "analog_in_1":10.02,
         "gps_2":{
            "altitude":30,
            "latitude":39.936,
            "longitude":3.718
         },
         "luminosity_1":79,
         "luminosity_2":195
      }
}
```
### Integración MQTT
Por último necesitaremos las credenciales de acceso al broker MQTT que TTN nos ofrece como una opción de integración para poder suscribirnos a topic desde nuestra aplicación:

<img src="./img/TTN_app_mqtt.png" width="400" align="center" />

**IMPORTANTE**: copia el password ahora porque una vez cerrada esta ventana. No tendrás opción a consultarlo.

## Análisis de la señal
Cuando hablamos de sonido o de ruido, la magnitud que se utiliza con mayor frecuencia son los dBA y la medida estandarizada para calidad ambiental es la LAeq (Equivalent Continuous Sound Pressure Level). En nuestro caso será para el tiempo dado de 10 minutos.
Calcular este indice requiere de varias transformaciones, calibrados y cálculos que se detallan a continuación:

1. (A) Partimos que el sensor de sonido _SparkFun Sound Detector_ nos da una medida analógica con un rango de 0 a 4500. Puede dar valores superiores pero aplicamos un filtro software de banda alta para eliminar los _outlier_. Si embargo esta medida analógica no es el valor instantáneo del sonido o presión sonora. Si no la envolvente de 1 segundo de muestreo calculada por el circuito del sensor. Línea verde del gráfico:
<img src="./img/sensor_sparkfun_waves.png" align="center" />

Este gráfico muestra los voltajes de salida a lo largo del tiempo.
- El trazo verde oscuro es la salida de audio del detector de sonido. El voltaje de audio directamente desde el micrófono se encuentra en esta salida.
- El trazo verde claro es la envolvente. Este voltaje analógico rastrea la amplitud del sonido. Se puede observar que el tercer pulso se vuelve notablemente más fuerte a medida que avanza.
- Finalmente, la línea roja es la salida de la puerta. Esta salida es baja cuando las condiciones son tranquilas y aumenta cuando se detecta sonido.
1. (B) Y tenemos que determinar el valor base que corresponda con la medida en silencio.
2. Calculamos el ruido medio del periodo de ciclo de transmisión que es de 10 minutos. 

<img src="./img/leq_formula.jpeg" align="right" />

Donde:
Leq = nivel de presión sonora continuo equivalente en dB
p0 = nivel de presión de referencia (típicamente 20 µPa)
pA = presión sonora adquirida
t1 = tiempo de inicio de la medición
t2 = tiempo de finalización de la medición
<img src="./img/leq_chart.jpeg" align="right" />

3. Finalmente para obtener el LAeq basándonos en la media y no al Leq anterior que sería lo correcto. Aplicamos la fórmula que nos dará una magnitud en decibelios:
> **LAeq = Calibración * 20 * log10 ( Ruido Medio / Ruido Base )**

## Gráficas y alarmas en Node-RED
Partimos de una instancia Node-RED con nodo para mostrar un interface de usuario que se llama _dashboard_. Lo que se puede hacer desde la opción _Manage Palette_ de la aplicación de administración.

Se ha construido un sencillo flujo para representar unas gráficas y poder establecer un umbral de alerta para enviar un mensaje a IFTTT

<img src="./img/jaleometro_nodered_ui_mqtt.png"  align="center" />

El cual genera el siguiente dashboard:

<img src="./img/jaleometro_nodered_ui.png"  align="center" />

En el flujo de Node-RED se tratan y muestran gráficamente los mensajes MQTT leidos. El _payload_ contiene el json con los valores de:
- Ruido medio en "luminosity_1"
- Ruido pico en "luminosity_2"
- Id de sensor en "analog_in_1"
- Coordenadas geográficas en "gps_2"

Para importar el flujo deberemos acceder a la aplicación Node-RED que tenemos arrancada dónde encontraremos la opción de importar pulsando el botón de menú tipo hamburguesa. Lo más fácil es copiar al portapapeles de equipo el contenido del fichero jaleometro_nodered_ui_mqtt y pegarlo en la pantalla:

<img src="./img/node-red_import-flow.png" width="400" align="center" />

Tras pulsar el botón de ¨import¨ tendrás la vista de los nodos y los flujos. Pero hay tres de ellos que no están correctamente configurados.

### Configuración cliente MQTT
Encontrarás todo configurado excepto las credenciales de acceso. Para utilizar las que has obtenido en los pasos anteriores. Solo tienes que ir a las propiedades del nodo MQTT, editar el _Server_ y acceder a la pestaña _Security_:

<img src="./img/jaleometro_nodered_user_mqtt.png" align="center" />

<img src="https://github.com/McOrts/garage-cerberus/blob/master/images/garage-cerberus_IFTTT.png" align="right" />

### Configurar integración con IFTTT
La integración con IFTTT (If This Then That) está montada con una sencilla llamada a su _endpoint_ REST utilizando en nodo tipo _http request_ con el siguiente formato. 
<br>
```
https://maker.ifttt.com/trigger/domohome_garage_intruder/with/key/???????????
```
<br>
Donde sustituiremos las interrogantes por la Key que encontramos en esta configuración del componente Maker _Webhooks_ en IFTTT. Que configuraremos para recibir el evento disparar los avisos nativos en nuestra móvil a fin de tener notificación de la alarma.

<img src="https://github.com/McOrts/garage-cerberus/blob/master/images/garage-cerberus_IFTTT-webhooks.png"  width="300" align="center" />

## Agradecimientos
- A Christoph Hafner y Javier Maura cuyo proyecto Moix me ha servido de inspiración https://github.com/Makespace-Mallorca/moix
- A mi ex-compañero Alexandre Coll por sus buenos consejos de cómo medir el ruido https://www.linkedin.com/in/alexandre-coll-molina/
- A Sparkfun por su buena documentación: https://learn.sparkfun.com/tutorials/sik-experiment-guide-for-the-arduino-101genuino-101-board-spanish/experimento-15-usar-la-placa-de-detector-de-sonido

