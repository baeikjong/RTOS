/*******************************************************************************
**	File:
**
**		07_MUTEX.c
**
**  Contents:
**		uC/OS-II programming
**
**	History:
**		Date        Name              Description
**		------------------------------------------------------------------------
**
*******************************************************************************/
#include "includes.h"

// filename
char* filename = "07_MUTEX";	

/* ...........................................................................
 *
 * 태스크 스택 정의
 * ===================
 */
 
int tickets;		// MAX value( 1000 million )
int backupTickets;
int T1_Ticket=0, T2_Ticket= 0;

OS_STK TaskStartStk[TASK_STK_SIZE]; /* START task stack */
OS_STK Task1Stk[TASK_STK_SIZE]; /* Task #1 stack */
OS_STK Task2Stk[TASK_STK_SIZE]; /* Task #2 stack */
//OS_STK Task3Stk[TASK_STK_SIZE]; /* Task #3 stack */
//OS_STK Task4Stk[TASK_STK_SIZE]; /* Task #4 stack */

/* ...........................................................................
 *
 * 사용자 메모리 정의
 * ===================
 */
TASK_USER_DATA  TaskUserData[7];

/* ...........................................................................
 *
 * 이벤트 컨트롤 & 사용자 정의 블럭 정의
 * ===================
 */
#define TICKET_1BILLION		1000000000
#define TICKET_1MILLION		1000000
#define TICKET_10000		10000
#define TICKET_1000			1000

OS_EVENT *mid;

extern void seedrand(int seed);
extern int randomnumber(void);
int buyTicket(void);
// 1초마다 콘솔에 동작중을 표시(.)
extern INT32U USE_OSTimeTickHook;

/* ...........................................................................
 *
 * 태스크 함수 원형
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  Task1(void *pdata);
void  Task2(void *pdata);        
void  Task3(void *pdata);
void  Task4(void *pdata);

//
// 어플리케이션 메인 루틴
//
int main(void)
{
	OSInit(); /* microC/OS-II 초기화 */

    PC_ElapsedInit(); /* Initialized elapsed time measurement     */

	strcpy(TaskUserData[TASK_START_ID].TaskName, "TaskStart");
    
	// TASK CREATE
	OSTaskCreateExt(TaskStart,
	               (void *)0,
	               &TaskStartStk[TASK_STK_SIZE-1],
	               TASK_START_PRIO,
	               TASK_START_ID,
	               &TaskStartStk[0],
	               TASK_STK_SIZE,
	               &TaskUserData[TASK_START_ID],
	               OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSStart(); /* start scheduler */

	return 0;    
}

//
// 태스크 'START'
//
void  TaskStart (void *pdata)
{
	INT8U err;
	OS_MUTEX_DATA mutex_data;	// mutex control
	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */
	
	// 시스템 초기화(UART, LCD, TimeTick)
	InitSystem();	

	// Task Statistics Title Display
	TaskStatisticsTitleDisplay(filename);

	OSStatInit();				/* Initialize uC/OS-II's 통계 함수 */

	// 1초마다 콘솔에 동작중을 표시(.)
	USE_OSTimeTickHook = 1; // refer to 'OS_CPU_C.C'

	/* TODO #1: 
		create a mutex
	    use mid and priority= 3*/
#if 1
	mid = OSMutexCreate(3, &err);
    if (err) printf("OSMutexCreate error found, code[0x%X]\n",err);
#endif // TODO #1

	// 뮤텍스 쿼리
    err = OSMutexQuery(mid, &mutex_data);
    if (err) printf("OSMutexQuery error found, code[0x%X]\n",err);

	//
	// set random seed
	//
	seedrand( 45 );
	
	// Set Tickets
	tickets= backupTickets = 10 * TICKET_1MILLION; // MAX value( 1 million )

	// TASK CREATE
    strcpy(TaskUserData[TASK_1_ID].TaskName, "Task1");
    err = OSTaskCreateExt(Task1,
                    (void *)0,
                    &Task1Stk[TASK_STK_SIZE - 1],
                    TASK_1_PRIO,
                    TASK_1_ID,
                    &Task1Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_1_ID],
                    0);
	if (err) printf("OSTaskCreate error found, code[0x%X]\n",err);

	// TASK CREATE
    strcpy(TaskUserData[TASK_2_ID].TaskName, "Task2");
    err = OSTaskCreateExt(Task2,
                    (void *)0,
                    &Task2Stk[TASK_STK_SIZE - 1],
                    TASK_2_PRIO,
                    TASK_2_ID,
                    &Task2Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_2_ID],
                    0);
	if (err) printf("OSTaskCreate error found, code[0x%X]\n",err);

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// 태스크 1
//
void Task1(void *pdata)
{
	INT8U err;
	OS_MUTEX_DATA mutex_data;	// mutex control
	
	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

    printf("\n\nTask1 TASK created,\n");
	while(1) {
		if(buyTicket() <= 0)
		{
			T1_Ticket++;
			printf("\n[TASK1]COUNTER up to %d\n", T1_Ticket);
			/* delete other task */
			err = OSTaskDel(TASK_2_PRIO);
			break;
		}
		T1_Ticket++;
		OSTimeDly(2);
	}

	printf("[TASK1]Total Tickets = %d\n", T1_Ticket + T2_Ticket);

	if(backupTickets != (T1_Ticket + T2_Ticket))
		printf("Not good. ticket size is different\n");
	else
		printf("Good!. matched\n");

	/* TODO #3: 태스크2 에도 같은게 또 있습니다. 작성하세요
		delete a mutex */
#if 1
    mid = OSMutexDel(mid, OS_DEL_ALWAYS, &err);
    if (err) printf("OSMutexDel error found, code[0x%X]\n",err);
#endif // TODO #3

	// 뮤텍스 쿼리
    err = OSMutexQuery(mid, &mutex_data);
    if (err) printf("OSMutexQuery error found, code[0x%X]\n",err);


	// 1초마다 콘솔 출력 '중지'
	USE_OSTimeTickHook = 0; // refer to 'OS_CPU_C.C'

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// 태스크 2
//
void Task2(void *pdata)
{
	INT8U err;
	OS_MUTEX_DATA mutex_data;	// mutex control
	INT32U timeo;
	int i;

	pdata = pdata;				/* 컴파일러 경고를 막기 위함 */

    printf("Task2 TASK created,\n");
	while(1) {
		for(i=0;i<1000000;i++)
		{
			if(buyTicket()==0)
			{
				T2_Ticket++;
				printf("\n[TASK2]COUNTER up to %d\n", T2_Ticket);
				/* delete other task */
				err = OSTaskDel(TASK_1_PRIO);
				goto exit;
			}
			T2_Ticket++;
		}
		timeo = randomnumber();
		OSTimeDly(timeo % 3);
	}

exit:
	printf("[TASK2]Total Tickets = %d\n", T1_Ticket + T2_Ticket);

	if(backupTickets != (T1_Ticket + T2_Ticket))
		printf("Not good. ticket size is different\n");
	else
		printf("Good!. matched\n");

	/* TODO #3: 태스크1 에도 같은게 또 있습니다. 작성하세요
		delete a mutex */
#if 1
    mid = OSMutexDel(mid, OS_DEL_ALWAYS, &err);
    if (err) printf("OSMutexDel error found, code[0x%X]\n",err);
#endif // TODO #3

	// 뮤텍스 쿼리
    err = OSMutexQuery(mid, &mutex_data);
    if (err) printf("OSMutexQuery error found, code[0x%X]\n",err);

	// 1초마다 콘솔 출력 '중지'
	USE_OSTimeTickHook = 0; // refer to 'OS_CPU_C.C'

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// buyTicket
//
int buyTicket(void)
{
	INT8U err;

	/* TODO #2: 
		MUTEX 을 이용하여 
		공유변수(tickets)를 보호한다 */
#if 1
	// CRITICAL SECTION(ENTER)
	OSMutexPend(mid, 0, &err);
#endif // TODO #2

	tickets --;	// ticket count

#if 1
	// CRITICAL SECTION(EXIT)
	err = OSMutexPost(mid);
	if (err) printf("OSMutexPost error found, code[0x%X]\n",err);
#endif // TODO #2

	return(tickets);
}

/*-----------------------------------------------------------------------------
 * Program : 07_MUTEX.C
-----------------------------------------------------------------------------*/