#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <Winsock2.h>   // Esto es para manipular el socket
#include <time.h>       //Para la funcion random

void generarNombreUsuario   (char *nombre, int longitud);
void generarContrasena      (char *contrasena, int longitud);

int main() {
    WSADATA wsa;
    SOCKET servidor, cliente;
    struct sockaddr_in DireccionServidor, DireccionCliente;
    char buffer[1024];
    int puertoHost = 8084;
    int longitudCliente;
    int capacidad;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { // Inicializo el Winsock
        printf("Error al iniciar Winsock, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        return 1;
    }

    servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor  == INVALID_SOCKET) {
        printf("No se pudo crear el socket, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    DireccionServidor.sin_family = AF_INET;         // Mapeo para que se me conecte al servidor
    DireccionServidor.sin_addr.s_addr = INADDR_ANY; //Para que acepte cualquier conexion
    DireccionServidor.sin_port = htons(puertoHost); // Puerto del servidor

    if (bind(servidor, (struct sockaddr*)&DireccionServidor, sizeof(DireccionServidor)) != 0) {
        printf("Error al hacer el bind, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return 1;
    }
    printf("-----------------------------------------------------------------\n");
    printf("Conexion al socket exitosa\n");
    if (listen(servidor, SOMAXCONN) == SOCKET_ERROR) {
        printf("Error al escuchar conexiones, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return 1;
    }

    printf("Servidor escuchando en el puerto %d...\n", puertoHost);

    longitudCliente = sizeof(struct sockaddr_in); // Acepto una conexion entrante
    cliente = accept(servidor, (struct sockaddr*)&DireccionCliente, &longitudCliente);
    if (cliente == INVALID_SOCKET) {
        printf("Fallo al aceptar la conexion, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        closesocket(servidor);
        WSACleanup();
        return 1;
    }
    printf("-----------------------------------------------------------------\n");
    printf("Cliente conectado.\n");
    printf("-----------------------------------------------------------------\n");

    if ((capacidad = recv(cliente, buffer, sizeof(buffer) - 1, 0)) == SOCKET_ERROR) {    // Proceso la opción enviada por el cliente
        printf("Error al recibir datos, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
    } else {
        buffer[capacidad] = '\0';
        printf("Opcion recibida: %s\n", buffer);

    if (strcmp(buffer,"2") == 0 ){
        printf("Generando contrasena, aguarde por favor!...\n");
        printf("-----------------------------------------------------------------\n");}
    else{
        printf("Generando nommbre, aguarde por favor!...\n");
        printf("-----------------------------------------------------------------\n");}

        if (strcmp(buffer, "1") == 0) {
            if ((capacidad = recv(cliente, buffer, sizeof(buffer) - 1, 0)) == SOCKET_ERROR) {
                printf("Error al recibir longitud, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
            } else {
                buffer[capacidad] = '\0';
                int longitud = atoi(buffer);

                if (longitud < 5 || longitud > 15) {
                    strcpy(buffer, " La longitud del nombre de usuario debe estar entre 5 y 15.");
                } else {
                    generarNombreUsuario(buffer, longitud);
                }
                send(cliente, buffer, strlen(buffer), 0); //Envio la respuesta al cliente.
            }
        } else if (strcmp(buffer, "2") == 0) {
            if ((capacidad = recv(cliente, buffer, sizeof(buffer) - 1, 0)) == SOCKET_ERROR) {
                printf("Error al recibir longitud, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
            } else {
                buffer[capacidad] = '\0';
                int longitud = atoi(buffer);

                if (longitud < 8 || longitud > 50) {
                    strcpy(buffer, " La longitud de la contrasena debe estar entre 8 y 50.");
                } else {
                    generarContrasena(buffer, longitud);
                    printf("Generando contrasena, aguarde por favor!...\n");
                    printf("-----------------------------------------------------------------\n");
                }

                send(cliente, buffer, strlen(buffer), 0); // Envio la respuesta al cliente.
            }
        } else {
            strcpy(buffer, "Opcion no valida.");
            send(cliente, buffer, strlen(buffer), 0); //Devuelvo error a cliente
        }
        printf("-----------------------------------------------------------------\n");
    }
    closesocket(cliente); // Cierro las conexiones y limpio el Winsock
    closesocket(servidor);
    WSACleanup();

    return 0;}

void generarNombreUsuario(char *nombre, int longitud) {
    char *vocales       = "aeiou";
    char *consonantes   = "bcdfghjklmnpqrstvwxyz";
    int i;
    srand(time(NULL));  // esto es para iniciar la funcion random

    int empezarConVocal = rand() % 2;  // 0 para consonante, 1 para vocal

    for (i = 0; i < longitud; i++) {
        if ((i % 2 == 0 && empezarConVocal) || (i % 2 != 0 && !empezarConVocal)) {
            nombre[i] = vocales[rand() % strlen(vocales)];
        } else {
            nombre[i] = consonantes[rand() % strlen(consonantes)];
        }
    }
    nombre[longitud] = '\0';  // Terminar la cadena con un carácter nulo
}

void generarContrasena(char *contrasena, int longitud) {
    char *caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int i;
    srand(time(NULL)); // esto es para iniciar la funcion random

    for (i = 0; i < longitud; i++) {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0';  // Termino con un valor nulo, para mas adelante, ayudar a que las funciones strlen o printf, sepan donde termina el caracter
}
