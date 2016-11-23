//
// Created by linchanghui on 11/18/16.
//

#include "main.h"
#include <stdlib.h>
#include <memory.h>
#include <getopt.h>
#include <stdio.h>
#include "sds.h"
#include <stdio.h>

#define Tot_Packets 208


/* just get lastest info */
int _system(const char * cmd, char *pRetMsg, int msg_len)
{
    FILE * fp;
    char * p = NULL;
    int res = -1;
    if (cmd == NULL || pRetMsg == NULL || msg_len < 0)
    {
        printf("Param Error!\n");
        return -1;
    }
    if ((fp = popen(cmd, "r") ) == NULL)
    {
        printf("Popen Error!\n");
        return -2;
    }
    else
    {
        memset(pRetMsg, 0, msg_len);
        //get lastest result
        while(fgets(pRetMsg, msg_len, fp) != NULL)
        {
            printf("Msg:%s",pRetMsg); //print all info
        }

        if ( (res = pclose(fp)) == -1)
        {
            printf("close popenerror!\n");
            return -3;
        }
        pRetMsg[strlen(pRetMsg)-1] = '\0';
        return 0;
    }
}

/*
 * 输入参数
 * n 网段
 * c 并发量
 * s b被替换的原ip
 */
int main (int argc,char *argv[]) {
    int i;
    int network_segment = 0;  /* Value for the "-n" optional argument. */
    int concurrency = 0;  /* Value for the "-c" optional argument. */
    int arg;
    char *srcip = NULL;
    char *demo_pcap = "/mnt/hgfs/ubuntu_shared/pcap_test/165.pcap";
    char *default_ip = "37.76";
    char ret_msg[128] = {0};
    int ret = 0;



//    while ((arg = getopt (argc, argv, "c:n:s:")) != -1) {
//        switch (arg) {
//            case 'c':
//                concurrency = atoi(optarg);
//                break;
//            case 'n':
//                network_segment = atoi(optarg);
//                break;
//            case 's':
//                srcip = optarg;
//                break;
//            case '?':
//                // Encountered an unknown or improperly formatted flag,
//                // deal with these appropriately.
//                //
//                // getopt() populates the flag character into the 'optopt'
//                // global.
//                if (optopt == 'c'){
//                    fprintf(stderr, "-c requires an argument.\n");
//                }
//                else if(optopt == 'n'){
//                    fprintf(stderr, "-n requires an argument.\n");
//                }
//                else if(optopt == 's'){
//                    fprintf(stderr, "-s requires an argument.\n");
//                }
//                else{
//                    fprintf(stderr, "Unknown flag '%c'.\n", optopt);
//                }
//                //Fall-through here is intentional
//            default:
//                //Hit some unexpected case - quit.
//                exit(EXIT_FAILURE);
//        }
//    }
//    sds rewrite_command;
//    sds *replay_commands = sds_malloc(concurrency* sizeof(sds));
//    int ip = 1;
//    //开始修改pcap的网段
//    for(int i = 1; i<=concurrency; i++) {
//        //每一个并发一个ip
//        if(ip % 256 == 0) {
//            ip = 1;
//            network_segment++;
//        }
//        sds filename = sdscatprintf(sdsempty(), "%d_%dto165.pcap", network_segment, ip);
//        rewrite_command = sdscatprintf(
//                sdsempty(),
//                "userid root tcprewrite --srcipmap=192.168.%s/32:192.168.%d.%d/32 --dstipmap=192.168.%s/32:192.168.%d.%d/32 --infile=/mnt/hgfs/ubuntu_shared/pcap_test/165.pcap --outfile=/mnt/hgfs/ubuntu_shared/pcap_test/%s",
//                srcip == NULL?default_ip:srcip,
//                network_segment,
//                ip,
//                srcip == NULL?default_ip:srcip,
//                network_segment,
//                ip,
//                filename);
////        system(rewrite_command);
//        _system(rewrite_command, ret_msg, sizeof(ret_msg));
//        //这里不用字符串拼接，改成存入数组，后面用for循环去做
//        replay_commands[i-1] = sdscatprintf(
//                sdsempty(),
//                "userid root nohup tcpreplay -i eth0 /mnt/hgfs/ubuntu_shared/pcap_test/%s & ",
//                filename);
//        ip++;
//    }
//
//    int total_incr = Tot_Packets*concurrency; //没一个pcap对应的packages个数,乘以并发数得到最后增加的packages个数
//    //一开始先打印出原来的值和最后期待的值
//    sds pf_ring_pre = sdsnew(sdscatprintf(
//            sdsempty(),
//            "userid root expect -f /mnt/hgfs/ubuntu_shared/test2.sh dbaegis cobra8029 192.168.37.115 80291655cobra %d",
//            total_incr));
////    system(pf_ring_pre);
//    _system(pf_ring_pre, ret_msg, sizeof(ret_msg));
//
//    for (int j = 0; j < concurrency; ++j) {
//        printf(replay_commands[j]);
////        system(replay_commands[j]);
//        _system(pf_ring_pre, ret_msg, sizeof(ret_msg));
//    }
//    free(replay_commands);


    char *pf_ring_aft = "userid root expect -f script/pf_ring_pre dbaegis cobra8029 192.168.37.115 80291655cobra";
//    system(pf_ring_aft);
    _system(pf_ring_aft, ret_msg, sizeof(ret_msg));
    printf("haha: %s",ret_msg);




}