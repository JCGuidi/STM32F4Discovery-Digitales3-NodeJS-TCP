//--------------------------------------------------------------
// File     : stm32_ub_udp_server.c
// Datum    : 26.05.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, SYSCFG, EXTI, MISC
// Funktion : Ethernet UDP-Server (per PHY : DP83848C)
//            IP-Stack = lwIP (V:1.3.2)
//
// Hinweis  : benutzt wird die RMII-Schnittstelle
//
//            PA1  = RMII_Ref_Clk       PC1 = ETH_MDC
//            PA2  = ETH_MDIO           PC4 = RMII_RXD0
//            PA7  = RMII_CRS_DV        PC5 = RMII_RXD1
//            PB11 = RMII_TX_EN
//            PB12 = RMII_TXD0          NRST = Reset
//            PB13 = RMII_TXD1
//            PB14 = RMII_INT
//
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_udp_server.h"


//--------------------------------------------------------------
// Globale Variabeln
//--------------------------------------------------------------
volatile uint32_t LocalTime = 0;
extern uint8_t EthLinkStatus;
struct udp_pcb *upcb_server;
struct ip_addr OwnIPaddr;
struct ip_addr HostIPaddr;


//--------------------------------------------------------------
// Globale Struktur für UDP_Server
//--------------------------------------------------------------
UDP_SERVER_t UDP_SERVER = {
  UDP_SERVER_NOINIT,"\0",0
};


//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);



//--------------------------------------------------------------
// Init vom UDP Server
// Return wert :
//      UDP_INIT_OK => Server wurde initialisiert
//   != UDP_INIT_OK => Fehler, Server wurde nicht initialisiert
//--------------------------------------------------------------
UDP_INIT_STATUS_t UB_UDP_Server_Init(void)
{
  UDP_INIT_STATUS_t ret_wert=UDP_INIT_OK;
  uint32_t check=0;

  if(UDP_SERVER.status==UDP_SERVER_NOINIT) {
    // Ethernet init
    check=ETH_BSP_Config();
    if(check==1) ret_wert=UDP_INIT_ETH_MACDMA_ERR;
    if(check==2) ret_wert=UDP_INIT_ETH_PHYINT_ERR;

    // Init vom LwIP-Stack
    LwIP_Init();

    // init vom Server ist fertig
    UDP_SERVER.status=UDP_SERVER_NOCONNECT;
  }

  return(ret_wert);
}


//--------------------------------------------------------------
// zum starten vom UDP-Server
// Return wert :
//      UDP_CONNECT_OK => UDP-Server läuft
//   != UDP_CONNECT_OK => Fehler, UDP-Server läuft nicht
//--------------------------------------------------------------
UDP_SERVER_CONNECT_t UB_UDP_Server_Connect(void)
{
  UDP_SERVER_CONNECT_t ret_wert=UDP_CONNECT_OK;
  err_t err;

  if(UDP_SERVER.status==UDP_SERVER_NOINIT) return(UDP_INIT_ERR);
  if(UDP_SERVER.status==UDP_SERVER_RUNNING) return(UDP_RUNNING);
  if(EthLinkStatus>0) return(UDP_NO_LINK);

  // Verbindung für den Server erstellen
  upcb_server = udp_new();

  if (upcb_server==NULL) return(UDP_ERR1);

  IP4_ADDR( &HostIPaddr, HOST_IP_ADDR0, HOST_IP_ADDR1, HOST_IP_ADDR2, HOST_IP_ADDR3 );
  IP4_ADDR( &OwnIPaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3 );

  // Server an eigene IP binden
  err=udp_bind(upcb_server,&OwnIPaddr,OWN_UDP_PORT);

  if (err != ERR_OK) return(UDP_ERR2);

  // Funktion für UDP-Receive einrichten
  udp_recv(upcb_server, udp_receive_callback, NULL);

  // Connect vom Server ist fertig
  UDP_SERVER.status=UDP_SERVER_RUNNING;

  return(ret_wert);
}


//--------------------------------------------------------------
// zum bennden vom UDP-Server
//--------------------------------------------------------------
void UB_UDP_Server_Disconnect(void)
{
  if(UDP_SERVER.status==UDP_SERVER_RUNNING) {
    udp_remove(upcb_server);
    UDP_SERVER.status=UDP_SERVER_NOCONNECT;
  }
}


//--------------------------------------------------------------
// Sendet einen String per UDP zum Host
// Return_wert :
//  -> ERROR   , wenn String nicht gesendet wurde
//  -> SUCCESS , wenn String gesendet wurde
//--------------------------------------------------------------
ErrorStatus UB_UDP_Server_SendString(char *ptr)
{
  ErrorStatus ret_wert=ERROR;
  struct pbuf *p;

  // nur senden, wenn Server läuft
  if(UDP_SERVER.status==UDP_SERVER_RUNNING) {
    // Puffer einrichten
    p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)ptr), PBUF_POOL);
    if (p == NULL) return(ERROR);

    // String in Puffer kopieren
    pbuf_take(p, (char*)ptr, strlen((char*)ptr));

    // Daten per UDP an HOST senden
    udp_sendto(upcb_server,p,&HostIPaddr,HOST_UDP_PORT);

    // Puffer wieder löschen
    pbuf_free(p);

    ret_wert=SUCCESS;
  }

  return(ret_wert);
}


//--------------------------------------------------------------
// UDP Server
// diese Funktion muss zyklisch aufgerufen werden
// Return Wert :
//  -> wenn Server Offline   = UDP_SERVER_OFFLINE
//  -> wenn nichts empfangen = UDP_REVEICE_EMPTY
//  -> wenn String empfangen = UDP_RECEIVE_READY
//                             (-> String steht in *ptr)
//--------------------------------------------------------------
UDP_RECEIVE_t UB_UDP_Server_Do(char *ptr)
{
  UDP_RECEIVE_t ret_wert=UDP_REVEICE_EMPTY;
  uint32_t n;
  char wert;

  if(UDP_SERVER.status!=UDP_SERVER_NOINIT) {
    // test ob ein Packet angekommen ist
    if (ETH_CheckFrameReceived())
    {
      // empfangenes Packet auswerten
      LwIP_Pkt_Handle();
    }
    // LwIP zyklisch aufrufen
    LwIP_Periodic_Handle(LocalTime);

    // check ob LAN-Verbinguns noch ok
    if(EthLinkStatus>0) {
      if(UDP_SERVER.status==UDP_SERVER_RUNNING) {
        udp_remove(upcb_server);
        UDP_SERVER.status=UDP_SERVER_NOCONNECT;
      }
    }
  }

  if(UDP_SERVER.status!=UDP_SERVER_RUNNING) return(UDP_SERVER_OFFLINE);

  // Check ob ein UDP-String empfangen wurde
  if(UDP_SERVER.rxlen>0) {
    ret_wert=UDP_RECEIVE_READY;
    // String aus dem Puffer kopieren
    n=0;
    do {
      wert=UDP_SERVER.rxbuf[n];
      ptr[n]=wert;
      n++;
    }while((wert!=0) && (n<UDP_RX_BUFFER_SIZE-1));
    ptr[n]=0x00;
    // Merker wieder löschen
    UDP_SERVER.rxlen=0;
  }

  return(ret_wert);
}


//--------------------------------------------------------------
// interne Funktion
// Funktion zum empfangen von
// UDP-Daten
// (wird automatisch beim UDP-receive aufgerufen)
//--------------------------------------------------------------
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
  uint32_t n;
  char wert;
  char *ptr;

  ptr=(char*)p->payload;

  // empfangene Daten in Puffer kopieren
  n=0;
  do {
    wert=*ptr;
    UDP_SERVER.rxbuf[n]=wert;
    ptr++;
    n++;
  }while((wert!=0) && (n<UDP_RX_BUFFER_SIZE-1));
  UDP_SERVER.rxbuf[n]=0x00;
  // Merker setzen, das UDP-Daten vorhanden sind
  UDP_SERVER.rxlen=n;

  // Puffer löschen
  pbuf_free(p);

}



//--------------------------------------------------------------
// Systick (wird alle 10ms aufgerufen)
//--------------------------------------------------------------
void SysTick_Handler(void)
{
  // incrementiert den Counter
  LocalTime += 10;  // +10 weil Timerintervall = 10ms
}


//--------------------------------------------------------------
// ISR von Ext-Interrupt (PB14)
//--------------------------------------------------------------
void EXTI15_10_IRQHandler(void)
{
  // check welcher Ext-Interrupt (10 bis 15) aufgetreten ist
  if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
  {
    // wenn es der Interrupt vom LINK-Status war

    // Interrupt Handler starten
    Eth_Link_ITHandler(DP83848_PHY_ADDRESS);
    // ISR-Flag löschen
    EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
  }
}

