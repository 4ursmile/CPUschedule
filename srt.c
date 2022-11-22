/*######################################
# University of Information Technology #
# IT007 Operating System #
# Tran Hoang Bao Ly, 21521109 #
# File: srt.c #
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
process GlobalPQueue[100];
int low = 0, height = -1;
int size()
{
    return height - low + 1;
}
void MoveArr(int sPos, int delta, int lenght)
{
    for(int i = lenght; i>0; i--)
    {
        GlobalPQueue[sPos+i-1+delta] = GlobalPQueue[sPos+i-1];
    }
}
void enqueue(process k)
{
    height++;
    for(int i = low; i<height; i++)
    {
        if (k.remain < GlobalPQueue[i].remain)
        {
            MoveArr(i, 1, height-i);
            GlobalPQueue[i] = k;
            return;
        }
    }
    GlobalPQueue[height] = k;
}
process dequeue()
{
    process  a;
    if (height - low < 0) return a;
    return GlobalPQueue[low++];
}
process p[10];
int n;
int getIndex(int pn){
    for (int i = 0; i < n; i++){
        if (pn == p[i].pn) return i;
    }
}
void main(){
    int i;

    int totwt=0, tottat=0, totrt = 0;
    printf("Enter the number of processes:");
    scanf("%d",&n);
    for(i=0;i<n;i++) {
        printf("Enter the Process Name, Arrival Time & Burst Time:");
        scanf("%d%d%d",&p[i].pn,&p[i].arr,&p[i].burst);
        p[i].remain = p[i].burst;
    }
    for(i = 0; i<n-1; i++)
        for(int j = i+1; j<n; j++)
            if (p[i].arr > p[j].arr)
                Lswap(&p[i],&p[j]);
    GS gantt[30];
    gantt[0].finish = 0;
    int cG = 1;
    i = 0;
    while (i<n || size() > 0){
        if ((i<n && p[i].arr < gantt[cG-1].finish) ) {
            int pid = getIndex(gantt[cG-1].pn);
            if (p[i].remain < p[pid].remain - (p[i].arr - gantt[cG-1].start)){
                gantt[cG].start = p[i].arr;
                gantt[cG].finish = gantt[cG].start + p[i].remain;
                gantt[cG].pn = p[i].pn;
                p[pid].remain -= (p[i].arr - gantt[cG-1].start);
                enqueue(p[pid]);
                i++;
                cG++;
                continue;
            }
            enqueue(p[i]);
            i++;
            continue;
        } else if (i<n) {
            enqueue(p[i]);
            i++;
        }
        process k = dequeue();
        gantt[cG].start = k.arr>gantt[cG-1].finish?k.arr:gantt[cG-1].finish;
        gantt[cG].finish = gantt[cG].start + k.remain;
        gantt[cG].pn = k.pn;
        Replace(k);
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
    
    for(i = 0; i<n-1; i++)
        for(int j = i+1; j<n; j++)
            if (p[i].pn > p[j].pn)
                Lswap(&p[i],&p[j]);
    printf("\nPName\tArrtime\tBurst\tTAT\tRT\tFinish");
    for(i=0;i<n;i++) {
        printf("\n%d\t%d\t%d\t%d\t%d\t%d",p[i].pn, p[i].arr, p[i].burst, p[i].tat, p[i].rt,  p[i].wt);
        totwt+=p[i].wt;
        tottat+=p[i].tat;
        totrt+=p[i].rt;
    }
    printf("\n");
    printf("Average turn around time: %f \n", (float)tottat/n);
    printf("Average respond time: %f \n", (float)totrt/n);
    printf("Average waiting time: %f \n", (float)totwt/n);
}