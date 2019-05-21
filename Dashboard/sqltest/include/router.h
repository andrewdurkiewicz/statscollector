#ifndef LAN_DRIVER_H
#define LAN_DRIVER_H
#if 0
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
// #include <net/sock.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/ip.h>                  /* For IP header */
#include <linux/if_ether.h>                  /* For IP header */
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/string.h>
#include <linux/icmp.h>
#include <linux/netlink.h>
#include <net/dst.h>
#include <net/ip.h>
#include <linux/spinlock.h>
#endif

#define LAN_DRIVER_DEVICE_SUCCESS        0
#define LAN_DRIVER_DEVICE_MALLOC_FAILED -1
#define LAN_DRIVER_CMD_NOT_SUPPORTED    -2

#define IF_NAME "eth0"

#define SIZEOF_ETHERHEADER 14
#define SIOCGDRIVERSTATS 1
#define SIOCGDEVSTATS    2
#define SIOCGIFNUM       3
#define SIOCSDEBUG       4
#define SIOCRSDEBUG      5
#define SIOCRMBUF        6
#define SIOCNUMBUF       7
#define SIOCGMODE        8
#define SIOCSMODE        9
#define SIOCSCOPY        10

#define FR_LAN_DEV_NAME           "/dev/fr"
#define FR_LAN_DEV_NAME_MAJOR_VER 171

#define CS_LAN_DEV_NAME           "/dev/cs"
#define CS_LAN_DEV_NAME_MAJOR_VER 175

#define PO_LAN_DEV_NAME           "/dev/po"
#define PO_LAN_DEV_NAME_MAJOR_VER 173

#define LAN_DRIVER_MAX_IFC 2


#define DRV_MODE_CSCPSR 0
#define DRV_MODE_CSCP   1
#define DRV_MODE_CSSR   2  
#define DRV_MODE_CSCPSRRAW 3

#define DRV_MODE_MAX    DRV_MODE_CSCPSRRAW
static char *drv_mod_disp[] = {"CntxtSwitch+Copy+Search", "CntxtSwitch+Copy", "ContxtCwitch+Search",
                               "CntxtSwitch+Copy+Search+raw socket"};

typedef struct _ifc_pkt_stats_t_
{
    unsigned int rcvd;
    unsigned int sent;
    unsigned int dropped;

    unsigned int small_pkts;
    unsigned int other_pkts;
    unsigned int arp_rx_pkts;
    unsigned int mcast_pkts;
} ifc_pkt_stats_t;

typedef struct _lan_driver_stats_t_
{
    ifc_pkt_stats_t ifc_stats[LAN_DRIVER_MAX_IFC];
    unsigned int unknown_ifc_dropped;
    unsigned int malloc_failures;
}lan_driver_stats_t;


typedef struct lan_driver_device_stats_t_
{
    unsigned int open;
    unsigned int release;
    unsigned int read;
    unsigned int write;
    unsigned int poll;    
    unsigned int ioctl;
    unsigned int node_count;
    unsigned int user_to_kernel_copy_failed;
    unsigned int kernel_to_user_copy_failed;
    unsigned int skb_alloc_failed;
    unsigned int dev_q_xmit_failed;
    unsigned int read_buffer_too_small;
}lan_driver_device_stats_t;


typedef unsigned char boolean;
typedef unsigned char byte;
typedef unsigned short int word;        /* 16 bits */ 
typedef unsigned long dword;

#endif
