/*######################################
# University of Information Technology #
# IT007 Operating System #
# Tran Hoang Bao Ly, 21521109 #
# File: sjf.c #
######################################*/
#include<stdio.h>

typedef struct Process {
    int pn, arr,burst, rt, tat, wt;
} process;
typedef struct GanttStruct
{
    int start;
    int finish;
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
        if (k.burst < GlobalPQueue[i].burst)
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
void Replace(process k)
{
    for (int i = 0; i < n; i++)
    {
        if (p[i].pn == k.pn)
        {
            p[i] = k;
            return;
        }
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
    }
    for(i = 0; i<n-1; i++)
        for(int j = i+1; j<n; j++)
            if (p[i].arr > p[j].arr)
                Lswap(&p[i],&p[j]);
    GS gantt[30];
    gantt[0].finish = 0;
    int cG = 1;
    i = 0;
    while (i<n || size() > 0)
    {
        if (i<n && p[i].arr < gantt[cG-1].finish) {
            enqueue(p[i]);
            i++;
            continue;
        } else if (i<n && (p[i].arr == gantt[cG-1].finish ||  size() == 0 ) ) {
            enqueue(p[i]);
            i++;
        }
        process k = dequeue();
        gantt[cG].start = k.arr>gantt[cG-1].finish?k.arr:gantt[cG-1].finish;
        k.rt = gantt[cG].start - k.arr;
        gantt[cG].finish = gantt[cG].start + k.burst;
        k.tat = gantt[cG].finish - k.arr;
        k.wt = k.tat - k.burst;
        Replace(k);
        cG++;
    }
    
    for (i = 1; i < n;  i++){  
    }
    for(i = 0; i<n-1; i++)
        for(int j = i+1; j<n; j++)
            if (p[i].pn > p[j].pn)
                Lswap(&p[i],&p[j]);
    printf("\nPName\tArrtime\tBurst\tTAT\tRT\tWaiting");
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
