# Webserv

Este proyecto es un servidor HTTP desarrollado en C++98 como parte del programa de formación de 42. El objetivo principal es implementar un servidor HTTP que cumpla con las especificaciones descritas en el [RFC 2616](https://www.rfc-editor.org/rfc/rfc2616) y sea compatible con navegadores web modernos.

## Tabla de contenidos

1. [Descripción del Proyecto](#descripción-del-proyecto)
2. [Características](#características)
3. [Instalación](#instalación)
4. [Uso](#uso)
5. [Estructura del Proyecto](#estructura-del-proyecto)
---

## Descripción del Proyecto

Webserv es un servidor HTTP no bloqueante que utiliza `epoll()` para manejar todas las operaciones de entrada/salida. Es compatible con los métodos HTTP `GET`, `POST` y `DELETE` y puede servir sitios web estáticos, manejar cargas de archivos y ejecutar scripts CGI.

Este proyecto fue diseñado para mejorar la comprensión de los protocolos de red y la programación de sockets, así como para desarrollar habilidades en manejo de concurrencia y resiliencia del sistema.

## Características

- Compatible con HTTP/1.1.
- Soporte para múltiples métodos HTTP (`GET`, `POST`, `DELETE`).
- Manejo de múltiples conexiones simultáneas utilizando `epoll()`.
- Capacidad para servir sitios web estáticos.
- Ejecución de scripts CGI (por ejemplo, PHP o Python).
- Configuración personalizada a través de un archivo de configuración.
- Resiliencia bajo condiciones de estrés.

## Instalación

- Clona este repositorio en tu máquina local:
   ```bash
   git clone https://github.com/tuusuario/webserv.git
   cd webserv
- Compila el proyecto utilizando el Makefile incluido:

  ```bash
    make
  ```
- Para limpiar los archivos compilados:

  ```bash
    make clean
  ```
- Para eliminar los archivos binarios generados:

  ```bash
    make fclean
  ```
- Para recompilar desde cero:
  ```bash
  make re
  ```
## Uso

1. Ejecuta el servidor con el siguiente comando:

  ```bash
    ./webserv [archivo_de_configuración]
  ```
Si no proporcionas un archivo de configuración, el servidor usará un archivo predeterminado.
Asegúrate de que el archivo de configuración contenga las reglas necesarias para los puertos, rutas y otras configuraciones.
Archivo de Configuración:

  El archivo de configuración define parámetros como:

  Puertos y hosts para el servidor.
  Páginas de error predeterminadas.
  Límites de tamaño para el cuerpo del cliente.
  Rutas con reglas específicas, como métodos permitidos, redirecciones, archivos raíz, etc.
Ejemplo básico:

  ```txt
  
    server {
        listen 8080;
        server_name localhost;
    
        location / {
            root /var/www/html;
            index index.html;
        }
    
        error_page 404 /404.html;
    }
  ```
2. Conéctate al servidor:
Abre un navegador web y accede al servidor utilizando la dirección y el puerto configurados, por ejemplo:
  ```bash
    http://localhost:8080
  ```
