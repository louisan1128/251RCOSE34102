//2021320161 안승환 OS-Term Project CPU Scheduling Simulator

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*===================구조체 선언=====================*/
typedef struct {
    int pid;
    int arrival_time;
    int cpu_burst_time;
    int io_burst_time;
    int io_request_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int end_time;
    int io_flag;
} Process;


typedef struct {
    int p_i[100]; //process index
    int front;
    int rear;
} Queue;


/*===================함수 정의=======================*/

//기본 함수
int SelectMenu();
Process* Create_Process(int n);
void Display_Process(Process *p, int n);

//scheduling 함수
void FCFS(Process *p, int n);
void NP_SJF(Process *p, int n);
void P_SJF(Process *p, int n);
void NP_Pri(Process *p, int n);
void P_Pri(Process *p, int n);
void RR(Process *p, int n, int quantumn);
//queue 함수
void initQueue(Queue *q);
int isEmpty(Queue *q);
void enqueue(Queue *q, int p_i);
int dequeue(Queue *q);

//시스템 환경 설정
Queue Ready_Q, Wait_Q;
void config();
int cur_t; //현재 시간
int running; 
int cur_i; //실행중인 프로세스 index
int cpu_flag;
int IO_i;
int IO_running;
int num_fin;
int total_turn, total_wait;
double avg_turn, avg_wait;
int inserted[100];
int arrived[100];


//gnatt chart
int gantt_chart[1000];
void display_gantt_chart(int gantt_chart[], int num);

//process 복사사
Process *Copy_process(Process *p, int num);

//sorting 함수
void Sort_PID(Process *p, int num);
void Sort_Arriv(Process *p, int num);

//evaluation
void evaluation();
double eval_turn[6], eval_wait[6];
int FCFS_F =0;
int P_SJF_F = 0;
int NP_SJF_F = 0;
int P_Pri_F= 0;
int NP_Pri_F = 0;
int GG_F = 0;

/*===================메인 함수=======================*/

int main() {
    int choice, quantum, num_process;
    Process *process = NULL;
    
    while(1){
        choice = SelectMenu();
        switch(choice){
            case 1:
                if (process == NULL) {
                    printf("랜덤 프로세스 생성 하는 중....\n");
                } else {
                    printf("랜덤 프로세스 새로 생성 하는 중....\n");
                }
                //프로세스 개수 입력
                printf("몇개의 프로세스를 생성 할까요...?  (1~5사이)\n");
                printf("숫자만 적어주세요\n\n");
                scanf("%d", &num_process);
                if(num_process>5) {
                    printf("1~5사이 프로세스만 생성해주세요\n");
                    break;
                }

                process = Create_Process(num_process);

                printf("\n\n\n==================생성된 프로세스===================\n");
                Display_Process(process, num_process);
                break;
            case 2:
                printf("\n\n\n==================프로세스 보기=====================\n");
                if (process == NULL) {
                    printf("프로세스 먼저 생성 해주세요\n\n\n\n");
                } else {
                    Display_Process(process, num_process);
                }
                break;
            case 3:
                if(process == NULL) {
                    printf("프로세스 먼저 생성 해주세요\n\n\n\n");
                } else {
                    printf("\n\n\n\n\n\n\n==================FCFS 실행 중...===================\n");
                    Display_Process(process, num_process);
                    FCFS(process, num_process);
                    printf("=============================================================\n\n\n\n\n");
                }
                break;
            case 4:
                if(process == NULL) {
                    printf("프로세스 먼저 생성 해주세요\n\n\n\n");
                } else {
                    printf("\n\n\n\n\n\n\n=====================Non-Preemptive SJF 실행 중...=======================\n");
                    Display_Process(process, num_process);
                    NP_SJF(process, num_process);
                    printf("============================================================\n\n\n\n\n");
                }
                break;
            case 5:
                if(process == NULL) {
                    printf("프로세스 먼저 생성 해주세요\n\n\n\n");
                } else {
                    printf("\n\n\n\n\n\n\n=====================Preemptive SJF 실행 중...============================\n");
                    Display_Process(process, num_process);
                    P_SJF(process, num_process);
                    printf("============================================================\n\n\n\n\n");
                }
                break;
            case 6:
                if(process == NULL) {
                    printf("프로세스 먼저 생성 해주세요\n\n\n\n");
                } else {
                    printf("\n\n\n\n\n\n\n===================================Non-Preemptive Priority 실행 중...====================================\n");
                    Display_Process(process, num_process);
                    NP_Pri(process, num_process);
                    printf("============================================================\n\n\n\n\n");
                }
                break;
            case 7:
                if(process == NULL) {
                    printf("프로세스 먼저 생성 해주세요\n\n\n\n");
                } else {
                    printf("\n\n\n\n\n\n\n===================================Preemptive Priority 실행 중...====================================\n");
                    Display_Process(process, num_process);
                    P_Pri(process, num_process);
                    printf("============================================================\n\n\n\n\n");
                }
                break;
            case 8:
                if(process == NULL) {
                    printf("프로세스 먼저 생성 해주세요\n\n\n\n");
                } else {
                    printf("\n\n\n\n\n\n\nTime Quantum 입력: ");
                    scanf("%d", &quantum);
                    printf("\n===================================Round Robin 실행 중...====================================\n");
                    printf("Time Quantumn: %d", quantum);
                    Display_Process(process, num_process);
                    RR(process, num_process, quantum);
                    printf("============================================================\n\n\n\n\n");
                }
                break;
            case 9:
                if(process == NULL) {
                    printf("프로세스를 먼저 만들어 주세요!");
                } else {
                    printf("\n\n\n\n실행한 알고리즘의 성능 평가만 진행합니다\n");
                    Display_Process(process, num_process);
                    printf("==================================평가 결과==============================");
                    evaluation();
                    printf("============================================================\n\n\n\n\n");
                }
                break;
            case 10:
                printf("종료합니다\n");
                return 0;
            default:
                printf("잘못된 입력입니다. 다시 입력하세요\n");
                break;
        }
    }
    return 0;
}




/*=================초기 화면========================*/
int SelectMenu() {
    int n;
    printf("\n\n\n======= Scheduler Menu =======\n");
    printf("1. Random Process 생성\n");
    printf("2. Process 보기\n");
    printf("3. FCFS 실행\n");
    printf("4. Non-Preemptive SJF 실행\n");
    printf("5. Preemptive SJF 실행\n");
    printf("6. Non-Preemptive Priority 실행\n");
    printf("7. Preemptive Priority 실행\n");
    printf("8. Round Robin 실행\n");
    printf("9. 성능 평가\n");
    printf("10. 종료\n");
    printf("==============================\n");
    printf("!주의사항! 프로세스 생성 후 다른 메뉴 실행 바랍니다\n");
    printf("메뉴 선택: ");
    scanf("%d", &n);

    return n;
}


/*===============프로세스 생성================*/
Process* Create_Process(int num)
{
    srand(time(NULL));
    int i;

    Process* p = malloc(sizeof(Process) * (num));

    for(i = 0; i < num; i++) {
        p[i].pid = i + 1;
        p[i].arrival_time = rand() % 10;         // 0~9
        p[i].cpu_burst_time = (rand() % 10) + 1;  // 1~10
        p[i].io_burst_time = (rand() % 5) + 1;   // 1~5
        p[i].io_request_time = rand() % p[i].cpu_burst_time; 
        p[i].priority = (rand() % num) + 1;       // 1~num of process
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].end_time = 0;
        p[i].io_flag = 0;
    }

    return p;
}

/*===============프로세스 표시===============*/
void Display_Process(Process *p, int num) {
    if (p == NULL) {
        printf("\n\n프로세스가 없습니다 새로 생성해주세요\n\n");
    } else {
        printf("\n\t\tPID\tArriv\tCPU\tI/O_R\tI/O_B\tPri\n");
        for(int i = 0; i<num; i++) {
            printf("Process %d\t %d\t %d\t %d\t %d\t %d\t %d\n", 
                i+1, p[i].pid, p[i].arrival_time, p[i].cpu_burst_time,  
                p[i].io_request_time, p[i].io_burst_time, p[i].priority);
        }
        printf("\n");
    }
}

/*================FCFS=====================*/
void FCFS(Process *p_o, int num) {
    Process *p = Copy_process(p_o, num);
    //시스템 초기화 설정
    config();

    FCFS_F = 1;

    //arrival time 으로 정렬
    Sort_Arriv(p, num);

    //FCFS 실행
    while(running) {

        //도착한 프로세스 ready queue에 enqueue
        for(int i =0; i<num; i++) {
            if(p[i].arrival_time <= cur_t && inserted[i] == 0) {
                enqueue(&Ready_Q, i);
                inserted[i] = 1;
            }
        }

        //io 처리
        if(!isEmpty(&Wait_Q) && !IO_running) {
            IO_i = dequeue(&Wait_Q);
            IO_running = 1;
        }
        if(IO_running) {
            //io 실행
            p[IO_i].io_burst_time--;
            //IO finished
            if(p[IO_i].io_burst_time == 0) { 
                enqueue(&Ready_Q, IO_i);
                IO_running = 0;
            }
        }


        //ready queue에 있고, 현재 돌아가는 프로세스가 없으면 ready queue에서 꺼내기
        if(!isEmpty(&Ready_Q) && cpu_flag == 0) { 
            cur_i = dequeue(&Ready_Q);
            cpu_flag = 1;
        } 


        if(cpu_flag ==1) { //cpu 
            //실행
            p[cur_i].cpu_burst_time--;
            p[cur_i].io_request_time--;
            gantt_chart[cur_t] = p[cur_i].pid;

            //process finished
            if(p[cur_i].cpu_burst_time == 0) { 
                p[cur_i].end_time = cur_t + 1;
                cpu_flag=0;
                num_fin++;
            //io 요청
            } else if (p[cur_i].io_request_time == 0 && p[cur_i].io_flag == 0) {
                p[cur_i].io_flag = 1;
                enqueue(&Wait_Q, cur_i);
                cpu_flag = 0;
            }
            
        }

        //waiting  queue에 있는 프로세스 waiting time 증가
        for(int i = Ready_Q.front; i < Ready_Q.rear; i++) {
            int w_i = Ready_Q.p_i[i];
            p[w_i].waiting_time++;
        }

        if(num_fin == num) break; //모든 프로세스 수행
        cur_t++; //time + 1

    }
    


    //gnatt chart display
    display_gantt_chart(gantt_chart, cur_t);

    printf("==================결과===================\n");
    Sort_PID(p, num);
    printf("PID\tArr\tEnd\tTurn\tWait\n");
    //waiting time, turn around time 설정
    for(int i = 0; i<num; i++) {
        p[i].turnaround_time = p[i].end_time - p[i].arrival_time;
        total_turn += p[i].turnaround_time;
        total_wait += p[i].waiting_time;
        //결과 출력
        printf("%d\t%d\t%d\t%d\t%d\n", 
            p[i].pid, p[i].arrival_time, p[i].end_time, 
            p[i].turnaround_time, p[i].waiting_time);
    }

    avg_turn = (double)total_turn/num;
    eval_turn[0] = avg_turn;
    avg_wait = (double)total_wait/num;
    eval_wait[0] = avg_wait;
    printf("\nAverage Turnaround Time: %.2f\n",avg_turn);
    printf("Average Waiting Time: %.2f\n",avg_wait);

}


/*=================Non-preemptive SJF=====================*/
void NP_SJF(Process *p_o, int num) {
    Process *p = Copy_process(p_o, num);
    //시스템 초기화 설정
    config();

    NP_SJF_F = 1;
    //arrival time 으로 정렬
    Sort_Arriv(p, num);

    
    int SJF_list[100];
    int SJF_num = 0;

    //NP SJF 실행
    while(running) {
        //도착한 프로세스 enque
        for(int i = 0; i<num; i++) {
            if(p[i].arrival_time <= cur_t && inserted[i] == 0) {
                inserted[i] = 1;
                enqueue(&Ready_Q, i);
            }
        }

        //큐 초기화
        SJF_num = 0;
        while (!isEmpty(&Ready_Q)) {
            SJF_list[SJF_num++] = dequeue(&Ready_Q);
        }

        //sjf 정렬
        for(int i = 0; i<SJF_num - 1; i++) {
            for(int j = i + 1; j<SJF_num; j++) {
                if(p[SJF_list[i]].cpu_burst_time > p[SJF_list[j]].cpu_burst_time) {
                    int t = SJF_list[i];
                    SJF_list[i] = SJF_list[j];
                    SJF_list[j] = t;
                }
            }
        }

        //큐에 다시 enqueu
        for(int i = 0; i< SJF_num; i++) {
            enqueue(&Ready_Q, SJF_list[i]);
        }

        //io 처리
        if(!isEmpty(&Wait_Q) && !IO_running) {
            IO_i = dequeue(&Wait_Q);
            IO_running = 1;
        }
        if(IO_running) {
            //io 실행
            p[IO_i].io_burst_time--;
            //IO finished
            if(p[IO_i].io_burst_time == 0) { 
                enqueue(&Ready_Q, IO_i);
                IO_running = 0;
            }
        }


        //ready queue에 있고, 현재 돌아가는 프로세스가 없으면 ready queue에서 꺼내기
        if(!isEmpty(&Ready_Q) && cpu_flag == 0) { 
            cur_i = dequeue(&Ready_Q);
            cpu_flag = 1;
        } 


        if(cpu_flag ==1) { //cpu 
            //실행
            p[cur_i].cpu_burst_time--;
            p[cur_i].io_request_time--;
            gantt_chart[cur_t] = p[cur_i].pid;

            //process finished
            if(p[cur_i].cpu_burst_time <= 0) { 
                p[cur_i].end_time = cur_t + 1;
                cpu_flag=0;
                num_fin++;
            //io 요청
            } else if (p[cur_i].io_request_time == 0 && p[cur_i].io_flag == 0) {
                p[cur_i].io_flag = 1;
                enqueue(&Wait_Q, cur_i);
                cpu_flag = 0;
            }
            
        }

        //waiting  queue에 있는 프로세스 waiting time 증가
        for(int i = Ready_Q.front; i < Ready_Q.rear; i++) {
            int w_i = Ready_Q.p_i[i];
            p[w_i].waiting_time++;
        }

        
        if(num_fin == num) break; //모든 프로세스 수행
        cur_t++; //time + 1
    }
    //gnatt chart display
    display_gantt_chart(gantt_chart, cur_t);

    printf("==================결과===================\n");
    Sort_PID(p, num);
    printf("PID\tArr\tEnd\tTurn\tWait\n");
    //waiting time, turn around time 설정
    for(int i = 0; i<num; i++) {
        p[i].turnaround_time = p[i].end_time - p[i].arrival_time;
        total_turn += p[i].turnaround_time;
        total_wait += p[i].waiting_time;
        //결과 출력
        printf("%d\t%d\t%d\t%d\t%d\n", 
            p[i].pid, p[i].arrival_time, p[i].end_time, 
            p[i].turnaround_time, p[i].waiting_time);
    }

    avg_turn = (double)total_turn/num;
    eval_turn[1] = avg_turn;
    avg_wait = (double)total_wait/num;
    eval_wait[1] = avg_wait;
    printf("\nAverage Turnaround Time: %.2f\n",avg_turn);
    printf("Average Waiting Time: %.2f\n",avg_wait);

}


/*================================preemptive SJF===============================*/
void P_SJF(Process *p_o, int num) {
    Process *p = Copy_process(p_o, num);
    //시스템 초기화 설정
    config();
    
    P_SJF_F = 1;
    //arrival time sort
    Sort_Arriv(p, num);

    int SJF_list[100];
    int SJF_num = 0;

    //P SJF 실행
    while(running) {
        //도착한 프로세스 
        for(int i = 0; i<num; i++) {
            if(p[i].arrival_time <= cur_t && inserted[i] == 0) {
                inserted[i] = 1;
                enqueue(&Ready_Q, i);
            }
        }

        //큐 초기화
        SJF_num = 0;
        while (!isEmpty(&Ready_Q)) {
            SJF_list[SJF_num++] = dequeue(&Ready_Q);
        }

        //sjf 정렬
        for(int i = 0; i<SJF_num - 1; i++) {
            for(int j = i + 1; j<SJF_num; j++) {
                if(p[SJF_list[i]].cpu_burst_time > p[SJF_list[j]].cpu_burst_time) {
                    int t = SJF_list[i];
                    SJF_list[i] = SJF_list[j];
                    SJF_list[j] = t;
                }
            }
        }

        //큐에 다시 enqueu
        for(int i = 0; i< SJF_num; i++) {
            enqueue(&Ready_Q, SJF_list[i]);
        }

        //io 처리
        if(!isEmpty(&Wait_Q) && !IO_running) {
            IO_i = dequeue(&Wait_Q);
            IO_running = 1;
        }
        if(IO_running) {
            //io 실행
            p[IO_i].io_burst_time--;
            //IO finished
            if(p[IO_i].io_burst_time == 0) { 
                enqueue(&Ready_Q, IO_i);
                IO_running = 0;
            }
        }

        //ready queue에서 꺼내기
        if(!isEmpty(&Ready_Q) && cpu_flag == 0) { 
            cur_i = dequeue(&Ready_Q);
            cpu_flag = 1;
        } 

        if(cpu_flag ==1) { //cpu 
            //실행
            p[cur_i].cpu_burst_time--;
            p[cur_i].io_request_time--;
            gantt_chart[cur_t] = p[cur_i].pid;
            cpu_flag = 0; //매번 cpu 꺼주기 (다시 비교)

            //process finished
            if(p[cur_i].cpu_burst_time <= 0) { 
                p[cur_i].end_time = cur_t + 1;
                num_fin++;
            //io 요청
            } else if (p[cur_i].io_request_time == 0 && p[cur_i].io_flag == 0) {
                p[cur_i].io_flag = 1;
                enqueue(&Wait_Q, cur_i);
            } else {
                enqueue(&Ready_Q, cur_i); //다시 큐에 넣주기
            }
            
        }


        //ready  queue에 있는 프로세스 waiting time 증가
        for(int i = Ready_Q.front; i < Ready_Q.rear; i++) {
            int w_i = Ready_Q.p_i[i];
            p[w_i].waiting_time++;

            //현채 프로세스는 waiting time 증가 취소 
            if(w_i == cur_i) {
                p[w_i].waiting_time--;
            }
        }

        
        if(num_fin == num) break; //모든 프로세스 수행
        cur_t++; //time + 1
    }

    //gnatt chart display
    display_gantt_chart(gantt_chart, cur_t);


    printf("==================결과===================\n");
    Sort_PID(p, num);
    printf("PID\tArr\tEnd\tTurn\tWait\n");
    //waiting time, turn around time 설정
    for(int i = 0; i<num; i++) {
        p[i].turnaround_time = p[i].end_time - p[i].arrival_time;
        total_turn += p[i].turnaround_time;
        total_wait += p[i].waiting_time;
        //결과 출력
        printf("%d\t%d\t%d\t%d\t%d\n", 
            p[i].pid, p[i].arrival_time, p[i].end_time, 
            p[i].turnaround_time, p[i].waiting_time);
    }

    avg_turn = (double)total_turn/num;
    eval_turn[2] = avg_turn;
    avg_wait = (double)total_wait/num;
    eval_wait[2] = avg_wait;
    printf("\nAverage Turnaround Time: %.2f\n",avg_turn);
    printf("Average Waiting Time: %.2f\n",avg_wait);
}

/*================================Non_preemptive Priority============================*/
void NP_Pri(Process *p_o, int num) {
    Process *p = Copy_process(p_o, num);
    //시스템 초기화 설정
    config();
    
    NP_Pri_F = 1;
    //arrival time sort
    Sort_Arriv(p, num);

    int Pri_list[100];
    int Pri_num = 0;
    
    //NP_Pri 실행
    while(running) {
        //도착한 프로세스 enqueue
        for(int i = 0; i<num; i++) {
            if(p[i].arrival_time <= cur_t && inserted[i] == 0) {
                inserted[i] = 1;
                enqueue(&Ready_Q, i);
            }
        }

        //큐 초기화
        Pri_num = 0;
        while(!isEmpty(&Ready_Q)) {
            Pri_list[Pri_num++] = dequeue(&Ready_Q);
        }

        //priority 정렬
        for(int i = 0; i<Pri_num - 1; i++) {
            for(int j = i + 1; j<Pri_num; j++) {
                if(p[Pri_list[i]].priority > p[Pri_list[j]].priority) {
                    int t = Pri_list[i];
                    Pri_list[i] = Pri_list[j];
                    Pri_list[j] = t;
                }
            }
        }

        //enqueue
        for(int i = 0; i< Pri_num; i++) {
            enqueue(&Ready_Q, Pri_list[i]);
        }

        //io 처리
        if(!isEmpty(&Wait_Q) && !IO_running) {
            IO_i = dequeue(&Wait_Q);
            IO_running = 1;
        }
        if(IO_running) {
            //io 실행
            p[IO_i].io_burst_time--;
            //IO finished
            if(p[IO_i].io_burst_time == 0) { 
                enqueue(&Ready_Q, IO_i);
                IO_running = 0;
            }
        }


        //ready queue에 있고, 현재 돌아가는 프로세스가 없으면 ready queue에서 꺼내기
        if(!isEmpty(&Ready_Q) && cpu_flag == 0) { 
            cur_i = dequeue(&Ready_Q);
            cpu_flag = 1;
        } 


        if(cpu_flag ==1) { //cpu 
            //실행
            p[cur_i].cpu_burst_time--;
            p[cur_i].io_request_time--;
            gantt_chart[cur_t] = p[cur_i].pid;

            //process finished
            if(p[cur_i].cpu_burst_time <= 0) { 
                p[cur_i].end_time = cur_t + 1;
                cpu_flag=0;
                num_fin++;
            //io 요청
            } else if (p[cur_i].io_request_time == 0 && p[cur_i].io_flag == 0) {
                p[cur_i].io_flag = 1;
                enqueue(&Wait_Q, cur_i);
                cpu_flag = 0;
            }
            
        }

        //waiting  queue에 있는 프로세스 waiting time 증가
        for(int i = Ready_Q.front; i < Ready_Q.rear; i++) {
            int w_i = Ready_Q.p_i[i];
            p[w_i].waiting_time++;
        }

        
        if(num_fin == num) break; //모든 프로세스 수행
        cur_t++; //time + 1
    }
    

    //gnatt chart display
    display_gantt_chart(gantt_chart, cur_t);

    printf("==================결과===================\n");
    Sort_PID(p, num);
    printf("PID\tArr\tEnd\tTurn\tWait\n");
    //waiting time, turn around time 설정
    for(int i = 0; i<num; i++) {
        p[i].turnaround_time = p[i].end_time - p[i].arrival_time;
        total_turn += p[i].turnaround_time;
        total_wait += p[i].waiting_time;
        //결과 출력
        printf("%d\t%d\t%d\t%d\t%d\n", 
            p[i].pid, p[i].arrival_time, p[i].end_time, 
            p[i].turnaround_time, p[i].waiting_time);
    }

    avg_turn = (double)total_turn/num;
    eval_turn[3] = avg_turn;
    avg_wait = (double)total_wait/num;
    eval_wait[3] = avg_wait;
    printf("\nAverage Turnaround Time: %.2f\n",avg_turn);
    printf("Average Waiting Time: %.2f\n",avg_wait);


}



/*==========================Preemtive Priority==========================*/
void P_Pri(Process *p_o, int num) {
    Process *p = Copy_process(p_o, num);
    //시스템 초기화 설정
    config();
    
    P_Pri_F = 1;
    //arrival time sort
    Sort_Arriv(p, num);

    int Pri_list[100];
    int Pri_num = 0;

    //P_Pri 실행
    while(running) {
        //도착한 프로세스 enqueue
        for(int i = 0; i<num; i++) {
            if(p[i].arrival_time <= cur_t && inserted[i] == 0) {
                inserted[i] = 1;
                enqueue(&Ready_Q, i);
            }
        }

        //큐 초기화
        Pri_num = 0;
        while(!isEmpty(&Ready_Q)) {
            Pri_list[Pri_num++] = dequeue(&Ready_Q);
        }

        //priority 정렬
        for(int i = 0; i<Pri_num - 1; i++) {
            for(int j = i + 1; j<Pri_num; j++) {
                if(p[Pri_list[i]].priority >= p[Pri_list[j]].priority) {
                    int t = Pri_list[i];
                    Pri_list[i] = Pri_list[j];
                    Pri_list[j] = t;
                }
            }
        }

        //enqueue
        for(int i = 0; i< Pri_num; i++) {
            enqueue(&Ready_Q, Pri_list[i]);
        }

        //io 처리
        if(!isEmpty(&Wait_Q) && !IO_running) {
            IO_i = dequeue(&Wait_Q);
            IO_running = 1;
        }
        if(IO_running) {
            //io 실행
            p[IO_i].io_burst_time--;
            //IO finished
            if(p[IO_i].io_burst_time == 0) { 
                enqueue(&Ready_Q, IO_i);
                IO_running = 0;
            }
        }


        //ready queue에 있고, 현재 돌아가는 프로세스가 없으면 ready queue에서 꺼내기
        if(!isEmpty(&Ready_Q) && cpu_flag == 0) { 
            cur_i = dequeue(&Ready_Q);
            cpu_flag = 1;
        } 


        if(cpu_flag ==1) { //cpu 
            //실행
            p[cur_i].cpu_burst_time--;
            p[cur_i].io_request_time--;
            gantt_chart[cur_t] = p[cur_i].pid;
            cpu_flag = 0; //매번 cpu 꺼주기 (다시 비교)

            //process finished
            if(p[cur_i].cpu_burst_time <= 0) { 
                p[cur_i].end_time = cur_t + 1;
                num_fin++;
            //io 요청
            } else if (p[cur_i].io_request_time == 0 && p[cur_i].io_flag == 0) {
                p[cur_i].io_flag = 1;
                enqueue(&Wait_Q, cur_i);
            } else {
                enqueue(&Ready_Q, cur_i); //다시 큐에 넣주기
            }
            
        }

        //waiting  queue에 있는 프로세스 waiting time 증가
        for(int i = Ready_Q.front; i < Ready_Q.rear; i++) {
            int w_i = Ready_Q.p_i[i];
            p[w_i].waiting_time++;

            //현채 프로세스는 waiting time 증가 취소 
            if(w_i == cur_i) {
                p[w_i].waiting_time--;
            }
        }

        
        if(num_fin == num) break; //모든 프로세스 수행
        cur_t++; //time + 1
    }

    //gnatt chart display
    display_gantt_chart(gantt_chart, cur_t);

    printf("==================결과===================\n");
    Sort_PID(p, num);
    printf("PID\tArr\tEnd\tTurn\tWait\n");
    //waiting time, turn around time 설정
    for(int i = 0; i<num; i++) {
        p[i].turnaround_time = p[i].end_time - p[i].arrival_time;
        total_turn += p[i].turnaround_time;
        total_wait += p[i].waiting_time;
        //결과 출력
        printf("%d\t%d\t%d\t%d\t%d\n", 
            p[i].pid, p[i].arrival_time, p[i].end_time, 
            p[i].turnaround_time, p[i].waiting_time);
    }

    avg_turn = (double)total_turn/num;
    eval_turn[4] = avg_turn;
    avg_wait = (double)total_wait/num;
    eval_wait[4] = avg_wait;
    printf("\nAverage Turnaround Time: %.2f\n",avg_turn);
    printf("Average Waiting Time: %.2f\n",avg_wait);

}


/*========================Round Robin==========================*/
void RR(Process *p_o, int num, int q) {
    Process *p = Copy_process(p_o, num);
    //시스템 초기화 설정
    config();
    
    GG_F = 1;
    //quantumn time track 하는 variable
    int q_t = 0; 
    //arrival time sort
    Sort_Arriv(p, num);

    while(running) {
        //도착한 프로세스 enqueue
        for(int i = 0; i<num; i++) {
            if(p[i].arrival_time <= cur_t && inserted[i] == 0) {
                inserted[i] = 1;
                enqueue(&Ready_Q, i);
            }
        }

        //io 처리
        if(!isEmpty(&Wait_Q) && !IO_running) {
            IO_i = dequeue(&Wait_Q);
            IO_running = 1;
        }
        if(IO_running) {
            //io 실행
            p[IO_i].io_burst_time--;
            //IO finished
            if(p[IO_i].io_burst_time == 0) { 
                enqueue(&Ready_Q, IO_i);
                IO_running = 0;
            }
        }

        //ready queue에 있고, 현재 돌아가는 프로세스가 없으면 ready queue에서 꺼내기
        if(!isEmpty(&Ready_Q) && cpu_flag == 0) { 
            cur_i = dequeue(&Ready_Q);
            cpu_flag = 1;
        } 

        if(cpu_flag ==1) { //cpu 
            //실행
            p[cur_i].cpu_burst_time--;
            p[cur_i].io_request_time--;
            gantt_chart[cur_t] = p[cur_i].pid;
            q_t++;

            //process finished
            if(p[cur_i].cpu_burst_time == 0) { 
                p[cur_i].end_time = cur_t + 1;
                cpu_flag=0;
                num_fin++;
                q_t=0;

            //io 요청
            } else if (p[cur_i].io_request_time == 0 && p[cur_i].io_flag == 0) {
                p[cur_i].io_flag = 1;
                enqueue(&Wait_Q, cur_i);
                cpu_flag = 0;
                q_t=0;
            
            //quantumn time에 도달 Ready queue에 다시 넣기
            } else if(q_t== q) { 
                enqueue(&Ready_Q, cur_i);
                q_t=0;
                cpu_flag=0;
            } 
            
        }

        //waiting  queue에 있는 프로세스 waiting time 증가
        for(int i = Ready_Q.front; i < Ready_Q.rear; i++) {
            int w_i = Ready_Q.p_i[i];
            p[w_i].waiting_time++;

            //현채 프로세스는 waiting time 증가 취소 
            if(w_i == cur_i) {
                p[w_i].waiting_time--;
            }
        }

        
        if(num_fin == num) break; //모든 프로세스 수행
        cur_t++; //time + 1
    }





    //gnatt chart display
    display_gantt_chart(gantt_chart, cur_t);

    printf("==================결과===================\n");
    Sort_PID(p, num);
    printf("PID\tArr\tEnd\tTurn\tWait\n");
    //waiting time, turn around time 설정
    for(int i = 0; i<num; i++) {
        p[i].turnaround_time = p[i].end_time - p[i].arrival_time;
        total_turn += p[i].turnaround_time;
        total_wait += p[i].waiting_time;
        //결과 출력
        printf("%d\t%d\t%d\t%d\t%d\n", 
            p[i].pid, p[i].arrival_time, p[i].end_time, 
            p[i].turnaround_time, p[i].waiting_time);
    }

    avg_turn = (double)total_turn/num;
    eval_turn[5] = avg_turn;
    avg_wait = (double)total_wait/num;
    eval_wait[5] = avg_wait;
    printf("\nAverage Turnaround Time: %.2f\n",avg_turn);
    printf("Average Waiting Time: %.2f\n",avg_wait);
}



/*=================queue 함수==================*/
void initQueue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}
int isEmpty(Queue *q) {
    return q->front == q->rear;
}
void enqueue(Queue *q, int p_i) {
    q->p_i[q->rear] = p_i;
    q->rear++;
}
int dequeue(Queue *q) {
    if (isEmpty(q)) {
        return -1;
    }
    int ret = q->p_i[q->front];
    q -> front++;
    return ret;
}

/*==============시스템 환경 설정==================*/
void config() {
    initQueue(&Ready_Q);
    initQueue(&Wait_Q);

    cur_t = 0;
    running = 1; 
    cur_i = 0;
    cpu_flag = 0;
    IO_i = 0;
    IO_running = 0;
    num_fin = 0;
    total_turn = 0;
    total_wait = 0;
    avg_turn = 0;
    avg_wait = 0;
    memset(inserted, 0, sizeof(inserted));
    memset(arrived, 0, sizeof(arrived));

    memset(gantt_chart, 0, sizeof(gantt_chart)); //차트 초기화
}



/*===============gnatt chart====================*/
void display_gantt_chart(int gantt_chart[], int end_time) {
    printf("\n=================Gantt Chart==================\n");
    printf("|");

    int j = 0;

    for(int i = 0; i<= end_time; i++) {
        if(gantt_chart[i] != gantt_chart[j]) {
            int len = i-j;

            for(int k =0; k<len; k++) printf(" ");
            if(gantt_chart[j]== 0) {
                printf("\bII"); //idle
            } else {
                printf("\bP%d", gantt_chart[j]);
            }
            for(int k =0; k<len; k++) printf(" ");
            printf("\b");
            printf("|");
            j = i;
        }
    }
    //마지막 출력 추가
    int len = end_time + 1 - j;
    for(int k = 0; k < len; k++) printf(" ");
    if(gantt_chart[j] == 0) {
        printf("II");
    } else {
        printf("P%d", gantt_chart[j]);
    }
    for(int k = 0; k < len; k++) printf(" ");
    printf("|");

    j = 0;
    int i = 0;
    printf("\n0");
    for(i = 1; i <= end_time; i++) {
        if( gantt_chart[i] != gantt_chart[j]) {
            int len = i - j;
            for(int k = 0; k < len; k++) printf("  ");
            if(i>=10) printf("\b");
            printf("%d", i);
            j = i;
        }
    }
    len = end_time + 1 - j;
    for(int k = 0; k < len; k++) {printf("   ");}
    if(i>=10) printf("\b");
    printf("%d", i);
    
    printf("\n\n");
}

/*===========copy process ==============*/
Process *Copy_process(Process *p, int num) {
    Process *copy = malloc(sizeof(Process) * num);
    memcpy(copy, p, sizeof(Process) * num);
    return copy;
}


/*=======sort PID=============*/
void Sort_PID(Process *p, int num){
    for(int i = 0; i<num-1; i++) {
        for(int j = i+1; j<num; j++) {
            if(p[i].pid > p[j].pid) {
                Process t = p[i];
                p[i] = p[j];
                p[j] = t;
            }
        }
    }
}

void Sort_Arriv(Process *p, int num) {
    for(int i = 0; i<num-1; i++) {
        for(int j = i+1; j<num; j++) {
            if(p[i].arrival_time > p[j].arrival_time) {
                Process t = p[i];
                p[i] = p[j];
                p[j] = t;
            }
        }
    }
}


/*=================evaluation =========================*/
void evaluation() {
    int count = 0;
    int index[6];

    printf("\n1. FCFS: \n");
    if(FCFS_F == 0) {
        printf("FCFS 스케줄러를 실행해주시기 바랍니다.\n\n");
    } else {
        printf("Average Turnaround Time: %.2f\n", eval_turn[0]);
        printf("Average Waiting Time: %.2f\n\n", eval_wait[0]);
        index[count] = 0;
        count++;
    }

    printf("2. Non-Preemptive SJF: \n");
    if(NP_SJF_F == 0) {
        printf("NP_SJF 스케줄러를 실행해주시기 바랍니다.\n\n");
    } else {
        printf("Average Turnaround Time: %.2f\n", eval_turn[1]);
        printf("Average Waiting Time: %.2f\n\n", eval_wait[1]);
        index[count] = 1;
        count++;
    }

    printf("3. Preemptive SJF: \n");
    if(P_SJF_F == 0) {
        printf("P_SJF 스케줄러를 실행해주시기 바랍니다.\n\n");
    } else {
        printf("Average Turnaround Time: %.2f\n", eval_turn[2]);
        printf("Average Waiting Time: %.2f\n\n", eval_wait[2]);
        index[count] = 2;
        count++;
    }

    printf("4. Non-Preemptive Priority: \n");
    if(NP_Pri_F == 0) {
        printf("NP_Priority 스케줄러를 실행해주시기 바랍니다.\n\n");
    } else {
        printf("Average Turnaround Time: %.2f\n", eval_turn[3]);
        printf("Average Waiting Time: %.2f\n\n", eval_wait[3]);
        index[count] = 3;
        count++;
    }

    printf("5. Preemptive Priority: \n");
    if(P_Pri_F == 0) {
        printf("P_Priority 스케줄러를 실행해주시기 바랍니다.\n\n");
    } else {
        printf("Average Turnaround Time: %.2f\n", eval_turn[4]);
        printf("Average Waiting Time: %.2f\n\n", eval_wait[4]);
        index[count] = 4;
        count++;
    }

    printf("6. Roudn Robin: \n");
    if(GG_F == 0) {
        printf("RR 스케줄러를 실행해주시기 바랍니다.\n\n");
    } else {
        printf("Average Turnaround Time: %.2f\n", eval_turn[5]);
        printf("Average Waiting Time: %.2f\n\n", eval_wait[5]);
        index[count] = 5;
        count++;
    }

    const char* scheduler[6] = {
        "FCFS","Non-Preemptive SJF", "Preemptive SJF", 
        "Non-Preemptive Priority", "Preemptive Priority", "Round Robin"
    };

    for(int i = 0; i<count-1; i++) {
        for(int j = i+1; j<count; j++) {
            if(eval_turn[index[i]] > eval_turn[index[j]]) {
                int t = index[i];
                index[i] = index[j];
                index[j] = t;
            }
        }
    }
    printf("\n\nAverage Turnaround Time Ranking: \n");
    for(int i = 0; i<count; i++) {
        printf("%d: %s (%.2f)\n", i+1, scheduler[index[i]], eval_turn[index[i]]);
    }

    
    for(int i = 0; i<count-1; i++) {
        for(int j = i+1; j<count; j++) {
            if(eval_wait[index[i]] > eval_wait[index[j]]) {
                int t = index[i];
                index[i] = index[j];
                index[j] = t;
            }
        }
    }
    printf("\n\nAverage Waiting Time Ranking: \n");
    for(int i = 0; i<count; i++) {
        printf("%d: %s (%.2f)\n", i+1, scheduler[index[i]], eval_wait[index[i]]);
    }

}