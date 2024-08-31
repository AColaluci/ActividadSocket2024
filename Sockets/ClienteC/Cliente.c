#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <Winsock2.h> // Esto es para manipular el socket

void menu();

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in servidor;
    char mensaje[1024];
    char respuestaServidor[1024];
    int capacidad;
    int puertoHost = 8084;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { // Inicializo el Winsock
        printf("Error al iniciar Winsock, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        return 1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {     // Creo el socket
        printf("No se pudo crear el socket, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Socket creado...\n\n\n");
    system("PAUSE");
    system("CLS");      //Creo una pauso y limpio la pantalla

    servidor.sin_family = AF_INET;                         // Mapeo para que se me conecte al servidor
    servidor.sin_addr.s_addr = inet_addr("192.168.0.241"); // Dirección IP de mi computadora, seguramente haya que cambiar esto segun donde se ejecute
    servidor.sin_port = htons(puertoHost);                 // Puerto del servidor

    if (connect(sock, (struct sockaddr*)&servidor, sizeof(servidor)) < 0) { // Conecto al servidor
        printf("Error al conectar con el servidor, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Conectado al servidor...\n\n\n"); //TEST
    system("PAUSE");
    system("CLS");      //Creo una pauso y limpio la pantalla

    menu();
    gets(mensaje); // Muestro el menu y leo la opción del cliente

    if (send(sock, mensaje, strlen(mensaje), 0) < 0) { // Envio la opción seleccionada al servidor
        printf("Error al enviar mensaje, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    system("CLS");      //Creo una pauso y limpio la pantalla

    if (strcmp(mensaje, "1") == 0) {     // Proceso la opción del usuario
        printf("-------------------------------------------------------------\n");
        printf("***Minimo 5 caracteres - Maximo 15 caracteres***\n");
        printf("Ingrese la longitud del nombre de usuario: "); // Opción 1: Genera el nombre
        gets(mensaje); // Lee la longitud del nombre
        printf("-------------------------------------------------------------\n");

        if (send(sock, mensaje, strlen(mensaje), 0) < 0) { // Se envia la longitud al servidor
            printf("Error al enviar longitud, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return 1;
        }
    } else if (strcmp(mensaje, "2") == 0) {
        printf("-------------------------------------------------------------\n");
        printf("***Minimo 8 caracteres - Maximo 50 caracteres***\n");
        printf("Ingrese la longitud de la contrasena: "); // Opción 2: Genera contraseña
        gets(mensaje); // Lee la longitud de la contraseña
        printf("-------------------------------------------------------------\n");

        if (send(sock, mensaje, strlen(mensaje), 0) < 0) { // Se envia la longitud al servidor
            printf("Error al enviar longitud, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return 1;
        }

    } else { //Si le paso algo distinto a un 1 o 2, pincha!
        printf("\n\n Opcion no valida, cerrando cliente.\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if ((capacidad = recv(sock, respuestaServidor, sizeof(respuestaServidor) - 1, 0)) == SOCKET_ERROR) { // Recibo la respuesta del servidor
        printf("\n\nError al recibir respuesta, consulte con el administrador. Codigo de error: %d\n", WSAGetLastError());
    } else {

        respuestaServidor[capacidad] = '\0'; // Le pongo nulo al final para tratarlo como cadena de texto
        printf("\Respuesta del servidor: \t%s\n", respuestaServidor);
        printf("-------------------------------------------------------------\n");
    }

    closesocket(sock);  // Cierro las conexiones y limpio el Winsock
    WSACleanup();

    return 0;}

void menu() {
    printf("-------------------------------------------------\n");
    printf("1 - Generar nombre de usuario\n");
    printf("2 - Generar contrasena\n");
    printf("-------------------------------------------------\n");
    printf("Ingrese una opcion: "); }
