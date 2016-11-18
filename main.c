//
// Created by linchanghui on 11/18/16.
//

#include "main.h"
#include <stdlib.h>
#include <memory.h>
#include <getopt.h>
#include <stdio.h>
#include "sds.h"

/*
 * 输入参数
 * n 网段
 * c 并发量
 */
int main (int argc,char *argv[]) {
    int i;
    int network_segment = 0;  /* Value for the "-n" optional argument. */
    int concurrency = 0;  /* Value for the "-c" optional argument. */
    int arg;
    char *demo_pcap = "/mnt/hgfs/ubuntu_shared/pcap_test/165.pcap";

    while ((arg = getopt (argc, argv, "c:n:")) != -1) {
        switch (arg) {
            case 'c':
                concurrency = atoi(optarg);
                break;
            case 'n':
                network_segment = atoi(optarg);
                break;
            case '?':
                // Encountered an unknown or improperly formatted flag,
                // deal with these appropriately.
                //
                // getopt() populates the flag character into the 'optopt'
                // global.
                if (optopt == 'c'){
                    fprintf(stderr, "-c requires an argument.\n");
                }
                else if(optopt == 'n'){
                    fprintf(stderr, "-n requires an argument.\n");
                }
                else{
                    fprintf(stderr, "Unknown flag '%c'.\n", optopt);
                }
                //Fall-through here is intentional
            default:
                //Hit some unexpected case - quit.
                exit(EXIT_FAILURE);
        }
    }
    sds rewrite_command;
    sds replay_command = sdsempty();
    //开始修改pcap的网段
    for(int i = 1; i<concurrency; i++) {
        sds filename = sdscatprintf(sdsempty(), "%dto165.pcap", i);
        rewrite_command = sdscatprintf(
                sdsempty(),
                "userid root tcprewrite --srcipmap=192.168.37.76/32:192.168.%d.%d/32 --dstipmap=192.168.37.76/32:192.168.%d.%d/32 --infile=/mnt/hgfs/ubuntu_shared/pcap_test/165.pcap --outfile=/mnt/hgfs/ubuntu_shared/pcap_test/%s",
                network_segment,
                i,
                network_segment,
                i,
                filename);
        system(rewrite_command);
        replay_command = sdscat(replay_command, sdscatprintf(
                sdsempty(),
                "userid root tcpreplay -i eth0 /mnt/hgfs/ubuntu_shared/pcap_test/%s & ",
                filename)
        );
    }

    printf(replay_command);
    system(replay_command);

}