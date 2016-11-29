//
// Created by linchanghui on 11/18/16.
//

#include <sys/time.h>
#include <time.h>
#include "main.h"



#define Tot_Packets 208

/* just get lastest info */
int _system(const char * cmd, sds* pRetMsg)
{
    FILE * fp;
    char * p = NULL;
    int res = -1;
    int msg_len = 128;
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
        char ret_msg[128] = {0};
        memset(ret_msg, 0, msg_len);
        //get lastest result
        while(fgets(ret_msg, msg_len, fp) != NULL)
        {
            *pRetMsg = sdscat(*pRetMsg, ret_msg);
        }

        if ( (res = pclose(fp)) == -1)
        {
            printf("close popenerror!\n");
            return -3;
        }
        return 0;
    }
}

/**
 * 获取pf_ring里面几个关键字段
 */
void get_pf_ring_infos(sds* pRetMsg) {
    /*
         * 在这里把几个需要记录的字段记录下来
         * Msg:Tot Packets        : 3939813
         * Msg:Tot Pkt Lost       : 0
         * Msg:Tot Insert         : 3939813
         */
    tot_packets_pre = get_digit("(?<=Tot Packets\\s{8}:\\s{1})[0-9]{0,19}", *pRetMsg);

    tot_pkt_lost_pre = get_digit("(?<=Tot Pkt Lost\\s{7}:\\s{1})[0-9]{0,19}", *pRetMsg);
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
    time_t start, stop;
    char *demo_pcap = "/mnt/hgfs/ubuntu_shared/pcap_test/165.pcap";
    char *default_pcap_dir = "/mnt/hgfs/ubuntu_shared/pcap_test";
    char *default_ip = "37.76";
    sds ret_msg = sdsnew("");
    int ret = 0;



    while ((arg = getopt (argc, argv, "c:n:s:p:pd:")) != -1) {
        switch (arg) {
            case 'c':
                concurrency = atoi(optarg);
                break;
            case 'n':
                network_segment = atoi(optarg);
                break;
            case 's':
                srcip = optarg;
                break;
            case 'p':
                demo_pcap = optarg;
                break;
            case 'pd':
                default_pcap_dir = optarg;
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
                else if(optopt == 's'){
                    fprintf(stderr, "-s requires an argument.\n");
                }
                else if(optopt == 'p'){
                    fprintf(stderr, "-p requires an argument.\n");
                }
                else if(optopt == 'pd'){
                    fprintf(stderr, "-pd requires an argument.\n");
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
    sds *replay_commands = sds_malloc(concurrency* sizeof(sds));
    int ip = 1;
    //开始修改pcap的网段
    for(int i = 1; i<=concurrency; i++) {
        //每一个并发一个ip
        if(ip % 256 == 0) {
            ip = 1;
            network_segment++;
        }
        sds filename = sdscatprintf(sdsempty(), "%d_%dto165.pcap", network_segment, ip);
        rewrite_command = sdscatprintf(
                sdsempty(),
                "userid root tcprewrite --srcipmap=192.168.%s/32:192.168.%d.%d/32 --dstipmap=192.168.%s/32:192.168.%d.%d/32 --infile=%s --outfile=%s/%s",
                srcip == NULL?default_ip:srcip,
                network_segment,
                ip,
                srcip == NULL?default_ip:srcip,
                network_segment,
                ip,
                demo_pcap,
                default_pcap_dir,
                filename);
        system(rewrite_command);
        //这里不用字符串拼接，改成存入数组，后面用for循环去做
        replay_commands[i-1] = sdscatprintf(
                sdsempty(),
                "userid root nohup tcpreplay -i eth0 %s/%s & ",
                default_pcap_dir,
                filename);
        ip++;
    }

    int total_incr = Tot_Packets*concurrency; //没一个pcap对应的packages个数,乘以并发数得到最后增加的packages个数
    //一开始先打印出原来的值和最后期待的值
    _system(pf_ring_info, &ret_msg);
    get_pf_ring_infos(&ret_msg);
    ret_msg = sdsempty();

    printf("Tot Packets：%d\n", tot_packets_pre);
    printf("Tot Pkt Lost：%d\n", tot_pkt_lost_pre);
    printf("Tot Packets expected：%d\n", tot_packets_pre + total_incr);
    time(&start);


    for (int j = 0; j < concurrency; ++j) {
//        printf("%s\n", replay_commands[j]);
        system(replay_commands[j]);
    }
    free(replay_commands);

    //计算所有并发执行完成所需的时间
    do {
        _system(get_tcpreplay_proc, &ret_msg);
        ret_msg = sdsempty();
        sleep(1);
    }while (sdslen(ret_msg) != 0);
    time(&stop);
    printf("Finished in about %.0f seconds. \n", difftime(stop, start));


}