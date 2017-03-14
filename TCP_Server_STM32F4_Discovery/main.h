//--------------------------------------------------------------
// File     : main.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_MAIN_H
#define __STM32F4_UB_MAIN_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4x7_eth_bsp.h"
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "lwip/tcp.h"
#include "string.h"
#include "stm32f4xx_adc.h"
#include "stdio.h"



//--------------------------------------------------------------
// DHCP-Mode
// zum ausschalten, auskommentieren
// falls DHCP disable -> wird die statische IP Adresse benutzt
//--------------------------------------------------------------
//#define USE_DHCP




//--------------------------------------------------------------
// MAC-Adresse (02:00:00:00:00:00)
//--------------------------------------------------------------
#define MAC_ADDR0   2
#define MAC_ADDR1   0
#define MAC_ADDR2   0
#define MAC_ADDR3   0
#define MAC_ADDR4   0
#define MAC_ADDR5   0


//--------------------------------------------------------------
// statische IP-Adresse (192.168.0.10)
// (falls DHCP enable wird diese ignoriert)
//--------------------------------------------------------------
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   2
#define IP_ADDR3   10


//--------------------------------------------------------------
// Netzmsaske (255.255.255.0)
//--------------------------------------------------------------
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0


//--------------------------------------------------------------
// Gateway Adresse (192.168.0.1)
//--------------------------------------------------------------
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   2
#define GW_ADDR3   1




//--------------------------------------------------------------
// IP-Adresse vom HOST (192.168.0.15)
//--------------------------------------------------------------
#define HOST_IP_ADDR0   192
#define HOST_IP_ADDR1   168
#define HOST_IP_ADDR2   2
#define HOST_IP_ADDR3   1


//--------------------------------------------------------------
// UDP-Port Nummern
//--------------------------------------------------------------
#define  OWN_UDP_PORT      	7
#define  HOST_UDP_PORT      7

void SysTick_Handler(void);



//--------------------------------------------------------------
#endif // __STM32F4_UB_MAIN_H
