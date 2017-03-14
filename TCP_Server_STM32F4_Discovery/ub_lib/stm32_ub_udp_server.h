//--------------------------------------------------------------
// File     : stm32_ub_udp_server.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_UDP_SERVER_H
#define __STM32F4_UB_UDP_SERVER_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "main.h"

//--------------------------------------------------------------
// UDP-Puffer
//--------------------------------------------------------------
#define  UDP_RX_BUFFER_SIZE   30 // Anzahl der Bytes vom Puffer


typedef enum {
  UDP_SERVER_NOINIT =0,  // UDP-Server noch nicht initialisiert
  UDP_SERVER_NOCONNECT,  // UDP-Server noch nicht verbunden
  UDP_SERVER_RUNNING     // UDP-Server läuft
}UDP_SERVER_STATUS_t;

//--------------------------------------------------------------
// Struktur für UDP_Server
//--------------------------------------------------------------
typedef struct {
  UDP_SERVER_STATUS_t status;
  char rxbuf[UDP_RX_BUFFER_SIZE];
  uint32_t rxlen;
}UDP_SERVER_t;



typedef enum {
  UDP_INIT_OK =0,          // kein Fehler beim Init
  UDP_INIT_ETH_MACDMA_ERR, // Fehler beim init vom MAC
  UDP_INIT_ETH_PHYINT_ERR  // Fehler beim init vom PHY
}UDP_INIT_STATUS_t;


typedef enum {
  UDP_CONNECT_OK =0, // Verbindung ist ok
  UDP_INIT_ERR,      // Server noch nicht initialisiert
  UDP_RUNNING,       // Server läuft schon
  UDP_NO_LINK,       // keine Verbindung zum LAN
  UDP_ERR1,          // Error Nr. 1
  UDP_ERR2           // Error Nr. 2
}UDP_SERVER_CONNECT_t;


typedef enum {
  UDP_SERVER_OFFLINE =0,  // UDP-Server ist offline
  UDP_REVEICE_EMPTY,      // UDP RX-Puffer ist leer
  UDP_RECEIVE_READY       // UDP RX-Puffer ist voll
}UDP_RECEIVE_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
UDP_INIT_STATUS_t UB_UDP_Server_Init(void);
UDP_SERVER_CONNECT_t UB_UDP_Server_Connect(void);
void UB_UDP_Server_Disconnect(void);
ErrorStatus UB_UDP_Server_SendString(char *ptr);
UDP_RECEIVE_t UB_UDP_Server_Do(char *ptr);


//--------------------------------------------------------------
#endif // __STM32F4_UB_UDP_SERVER_H
