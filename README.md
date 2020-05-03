# Informe TP1: Protocolo DBUS

**Martín Andrés Suarez**

**Padrón: 101.540**

**Repositorio: `https://github.com/martinsuarezz/taller-tp1`**

--------------

## Cliente

El funcionamiento general del cliente está comandadado por una estructura client_t. La misma delega a otras estructuras las funciones más específicas.

Cuando se inicializa el programa, se crea una estrcutra client_t. Primero se le pide que se conecte a la dirección y puerto remoto del servidor. Para lograr esto, hace uso de la estructura socket_t. La misma se encarga de las comunicaciones remotas (establecer conexiones, enviar y recibir bytes, etc).

Luego de haber establecido correctamente una conexión con un servidor, el client_t se encarga de leer la entrada de datos, de traducirlo según el protocorlo D-BUS y enviarlo al servidor.

Para la lectura de datos, ya sea por archivo o entrada estándar, el client_t utiliza un filehandler_t. Esta estructura permite abstraer la lectura de líneas de la entrada de datos.

Para traducir las líneas al protocolo D-BUS, el cliente hace uso de la estructura encoder_t.

A su vez, tanto filehandler_t como encoder_t al tener que manejar mensajes de longitud variables utilizan la estructura dinamicvector_t que funciona como un vector de caracteres de longitud variable.

Una vez que la entrada de datos ya fue traducida, el client_t utiliza el socket_t para enviarlo al servidor y aguarda su respuesta. Mientras que se pueda seguir leyendo líneas, la ejecución continua.

[Diagrama del cliente](https://github.com/martinsuarezz/taller-tp1/img/sequenceDiagram.png)

## Servidor

El servidor esta comandado por la estructura server_t. Esta, al igual que el cliente delega la mayor parte de sus responsabilidades en otras estructuras.

Al inicializar el servidor, se crea una estructura server_t. Se le pasa un puerto del que debe escuchar direcciones entrantes y luego se pide que realice sus funciones.

El servidor, delega su conectividad con el cliente a la estructura socket_t.

Para decodificar los mensajes que recibe, se utiliza una estructura decoder_t. Esta estructura, a su vez hace uso de una estructura message_t la cual almacena de forma más ordenada los strings que deberá imprimir luego por pantalla.

Cuando se termina de decodificar e imprimir un mensaje, se envía la confirmación al cliente y se continua esperando nuevos mensajes.

[Diagrama del servidor](https://github.com/martinsuarezz/taller-tp1/img/sequenceDiagram2.png)