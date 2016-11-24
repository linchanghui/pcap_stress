//
// Created by linchanghui on 11/18/16.
//
#include <stdlib.h>
#include <memory.h>
#include <getopt.h>
#include <stdio.h>
#include "sds.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef PACP_STRESS_MAIN_H
#define PACP_STRESS_MAIN_H

#endif //PACP_STRESS_MAIN_H



int tot_packets_pre;
int tot_pkt_lost_pre;
int tot_insert_pre;

int tot_packets_aft;
int tot_pkt_lost_aft;
int tot_insert_aft;
char *pf_ring_info = "userid root expect -f script/pf_ring_info dbaegis cobra8029 192.168.37.115 80291655cobra";
char *get_tcpreplay_proc = "userid root ps aux|grep tcpreplay|grep -v grep";
