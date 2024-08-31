import socket
def main():  
    servidor_ip     = '192.168.0.241' # Mapeo del cliente a la ip de mi pc, por hay varia esto dependiendo donde se ejecute
    servidor_puerto = 8084            # Elijo este, porque tengo el 8080 en uso

    cliente_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Creo un socket 
    try:
       
        cliente_socket.connect((servidor_ip, servidor_puerto))  # Me conecto al servidor
        print("-------------------------------------------------")
        print("Conectado al servidor...")
        print("-------------------------------------------------\n")
        print("1 - Generar nombre de usuario\n")
        print("2 - Generar contrasena de usuario\n")
        print("-------------------------------------------------\n")
        opcion = input("Ingrese una opcion: ") # Pido una opcion al usuario
        
        if opcion not in ["1", "2"]:
            print("Opcion no valida.")
        
        cliente_socket.send(opcion.encode()) # Envio la opcion al servidor
        print("-------------------------------------------------\n")
        longitud = input("Ingrese la longitud: ") # Le pido una longitud deseada
        
        cliente_socket.send(longitud.encode())
        
        respuesta = cliente_socket.recv(1024).decode() # Consumo y muestro la respuesta del servidor
        
        if opcion == '1':
            print("\nEl nombre generado es: ", respuesta)
        else:
            print("\nLa contrasena generado es:   ", respuesta)
        print("-------------------------------------------------\n")
    
    except Exception as e:
        print("Error:", e)
    
    finally:        
        cliente_socket.close() # Cierro el socket

if __name__ == "__main__":
    main()