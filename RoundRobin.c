/*######################################
# University of Information Technology #
# IT007 Operating System #
# Tran Hoang Bao Ly, 21521109 #
# File: roundrobin.c #
######################################*/
#include<stdio.h>
typedef struct Process {
    int pn, arr,burst, rt, tat, wt, remain;
} process;
typedef struct GanttStruct{
    int start, finish, pn;
} GS;

void Lswap(process *a, process *b){
    process c = *a;
    *a = *b;
    *b = c;
}
process GlobalQueue[100];
int low = 0, height = -1;
int size(){
    return height - low + 1;
}
void enqueue(process k){
    GlobalQueue[++height] = k;
}
process dequeue(){
    process  a;
    if (height - low < 0) return a;
    return GlobalQueue[low++];
}
process p[10];
int n;
int getIndex(int pn){
    for (int i = 0; i < n; i++){
        if (pn == p[i].pn) return i;
    }
}
void main(){
    int i, q;
    int totwt=0, tottat=0, totrt = 0;
    printf("Enter the number of processes:");
    scanf("%d",&n);
    printf("Enter quantumn timee:");
    scanf("%d",&q);
    for(i=0;i<n;i++) {
        printf("Enter the Process Name & Burst Time:");
        scanf("%d%d",&p[i].pn,&p[i].burst);
        p[i].remain = p[i].burst;
        p[i].arr = 0;
        enqueue(p[i]);
    }
    GS gantt[40];
    gantt[0].finish = 0;
    int cG = 1;
    i = 0;
    while (size() > 0){
        process k = dequeue();
        gantt[cG].pn = k.pn;
        gantt[cG].start = gantt[cG-1].finish;
        if (k.remain > q){
            gantt[cG].finish = gantt[cG].start + q;
            k.remain =  k.remain - q;
            enqueue(k);
        } else {
            gantt[cG].finish = gantt[cG].start+k.remain;
        }
        cG++;
    }
    /// Find roudtrip time, respond time and waiting time base on gantt
    for (int i = 0; i < n; i++){
        //resprond timee
        for (int j = 1; j < cG; j++){
            if (p[i].pn == gantt[j].pn){
                p[i].rt = gantt[j].start - p[i].arr;
                break;
            }
        }
        for (int j = cG; j > 0; j--){
            if (p[i].pn == gantt[j].pn){
                p[i].tat = gantt[j].finish - p[i].arr;
                break;
            }
        }
        p[i].wt = p[i].tat- p[i].burst;
    }
    printf("Gantt Chart \n");
    printf("|\t%d", gantt[1].pn);
    for (int i = 2; i < cG; i++){
        printf("\t|\t%d", gantt[i].pn);
    }
    printf("\t|\n");
    printf("%d", gantt[1].start);
    for (int i = 2; i <= cG; i++){
        printf("\t|\t%d", gantt[i-1].finish);
    }
    printf("\t|\n");
    for(i=0;i<n;i++) {
        totwt+=p[i].wt;
        tottat+=p[i].tat;
        totrt+=p[i].rt;
    }
    printf("Average turn around time: %f \n", (float)tottat/n);
    printf("Average respond time: %f \n", (float)totrt/n);
    printf("Average waiting time: %f \n", (float)totwt/n);
}