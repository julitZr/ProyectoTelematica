Introducción

El objetivo de este proyecto es desarrollar las competencias necesarias en el diseño y desarrollo de 
aplicaciones concurrentes en red. Para lograr esto, se empleará la API Sockets con el fin de escribir 
un protocolo de comunicaciones que permita a una aplicación cliente comunicarse con una 
aplicación servidor.
En este trabajo se requiere implementar un servidor DHCP así como el cliente. El servidor debe ser 
capaz de entregar parámetros IPv4 a una red local o remota, entre los parámetros que 
proporcionaría a los clientes se deben tener como mínimo Dirección IP, mascara de subred, default 
Gateway, DNS server y dominio. En la implementación se debe determinar el respectivo protocolo 
de capa de transporte. 

Desarrollo

Este trabajo se realizó mediante dos maquinas virtuales, ambas de ubuntu, las cuales una funcionaba de 
cliente y la otra de servidor permitiendo así implementar un protocólo DHCP de tipo SOCK_DGRAM. 
Lo primero antes de desarrollor fue investigar el funcionamiento de las maquinas virtuales y los protocolos para posteriormente implementarlos.
Luego, lo primero que desarrollamos fue el montaje de las dos maquinas virutales en VirtualBox, 
Después se cogieron por indivual cada maquina y se le instalaron dos comandos los caules fueron ipcongif y gcc 
que nos permitian saber la ip de cada máquina y tener el compilador de c en cada máquina. Además, se configuraron la máquinas
para que utilizaran el adaptador de red del equipo donde se encuentran las dos máquinas.
Después, a través del editor de texto de cada máquina se adjuntaron los códigos tanto .h como .c del cliente y del servidor,
permitiendo así llevar a cabo las principales fases del proceso DHCP (DISCOVER, 
OFFER, REQUEST, ACK).

Aspectos logrados

-Funcionamiento del servidor DHCP: capacidad para asignar IPs correctamente.
-Interacción del cliente con el servidor: correcto envío y recepción de mensajes 
DHCP.
-Documentación clara y detallada.

Aspectos no logrados

uso de DHCP relay y bases de datos de IP´s

Conclusiones

En el presente proyecto, se logró aprender e implementar un protocolo DHCP simulando su funcionamiento entre un servidor y un cliente. 
Tambien, gracias a esta implementacion aprendimos a como funciona este tipo de protocolo y la usabilidad e importancia que tiene
en aspectos tales como la automatización y la simplificación de la gestión de red ya que como vimos, nos permite eliminar 
la necesidad de configurar manualmente cada dispositivo, en este caso, en el cliente.
Una de las conclusiones clave es que el uso de DHCP facilita la escalabilidad de la red. A medida que una organización crece, 
la infraestructura de red debe adaptarse sin aumentar proporcionalmente la carga administrativa. DHCP permite añadir nuevos 
dispositivos y usuarios sin esfuerzos manuales ni configuraciones complejas, asegurando que la red pueda expandirse sin problemas.
Finalmente, la adopción de DHCP no solo mejora la administración de redes complejas, sino que también refuerza su sostenibilidad a largo plazo. 
El protocolo permite una mayor flexibilidad y adaptabilidad en redes que evolucionan constantemente, lo cual es crucial para organizaciones
que dependen de infraestructuras dinámicas y en constante crecimiento.

Referencias

https://beej.us/guide/bgnet/
https://beej.us/guide/bgc/
https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
https://datatracker.ietf.org/doc/html/rfc2131
https://datatracker.ietf.org/doc/rfc3456/
https://www.watchguard.com/help/docs/fireware/12/es-419/Content/es-419/networksetup/configure_dhcp_server_c.html


