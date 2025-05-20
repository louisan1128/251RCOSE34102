//2021320161 안승환 OS-Term Project CPU Scheduling Simulator

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    int end;
    int io_flag;
} Process;


typedef struct {
    int data;
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

//queue 함수
void initQueue(Queue *q);
int isEmpty(Queue *q);
void enqueue(Queue *q, int data);
int dequeue(Queue *q);


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
                printf("몇개의 프로세스를 생성 할까요...?\n");
                printf("숫자만 적어주세요\n\n");
                scanf("%d", &num_process);

                process = Create_Process(num_process);

                printf("==================생성된 프로세스===================\n");
                Display_Process(process, num_process);
                break;
            case 2:
                printf("==================프로세스 보기=====================\n");
                if (process == NULL) {
                    printf("프로세스 먼저 생성 해주세요");
                } else {
                    Display_Process(process, num_process);
                }
                break;
            case 3:
                printf("==================FCFS 실행 중...===================\n");
                if(process == NULL) {
                    printf("프로세스 먼저 생성 해주세요");
                } else {
                    FCFS(process, num_process);
                }
                break;
            case 4:
                printf("Non-Preemptive SJF 실행 중...\n");
                break;
            case 5:
                printf("Preemptive SJF 실행 중...\n");
                break;
            case 6:
                printf("Non-Preemptive Priority 실행 중...\n");
                break;
            case 7:
                printf("Preemptive Priority 실행 중...\n");
                break;
            case 8:
                printf("Time Quantum 입력: ");
                scanf("%d", &quantum);
                printf("Round Robin 실행 중...\n");
                break;
            case 9:
                if(process == NULL) {
                    printf("프로세스를 먼저 만들어 주세요!");
                } else {
                    printf("성능 평가 중...\n");
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
    printf("======= Scheduler Menu =======\n");
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
    printf("!주의사항! 프로세스 생성 후 다른 메뉴 실행 바랍니다");
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
        p[i].io_burst_time = (rand() % 10) + 1;   // 1~10
        p[i].io_request_time = rand() % p[i].cpu_burst_time;
        p[i].priority = (rand() % 10) + 1;       // 1~10
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].end = 0;
        p[i].io_flag = 0;
    }

    return p;
}

/*===============프로세스 표시===============*/
void Display_Process(Process *p, int num) {
    if (p == NULL) {
        printf("프로세스가 없습니다 새로 생성해주세요\n");
    } else {
        printf("\n\t\tPID\tArriv\tCPU\tI/O_R\tI/O_B\tPri\n");
        for(int i = 0; i<num; i++) {
            printf("Process %d\t %d\t %d\t %d\t %d\t %d\t %d\n", 
                i+1, p[i].pid, p[i].arrival_time, p[i].cpu_burst_time,  
                p[i].io_request_time, p[i].io_burst_time, p[i].priority);
        }
        printf("\n\n");
    }
}

/*================FCFS=====================*/
void FCFS(Process *p, int num) {
    //arrival time 으로 정렬
    for(int i = 0; i<num-1; i++) {
        for(int j = i+1; j<num; j++) {
            if(p[i].arrival_time > p[j].arrival_time) {
                Process t = p[i];
                p[i] = p[j];
                p[j] = p[i];
            }
        }
    }


    //결과 출력

}





/*=================queue 함수==================*/
void initQueue(Queue *q) {
    q->front = q->rear = -1;
}
int isEmpty(Queue *q) {
    return q->front == q-> rear;
}
void enqueue(Queue *q, int data) {
    q->rear++;
    q->data;
}
int dequeue(Queue *q) {
    Process *p = p;
}