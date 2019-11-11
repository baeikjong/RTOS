/*******************************************************************************
**	File:
**
**		06_SEM.c
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
char* filename = "06_SEM";	
	
/* ...........................................................................
 *
 * �½�ũ ���� ����
 * ===================
 */
OS_STK TaskStartStk[TASK_STK_SIZE]; /* START task stack */
OS_STK Task1Stk[TASK_STK_SIZE]; /* Task #1 stack */
OS_STK Task2Stk[TASK_STK_SIZE]; /* Task #2 stack */
//OS_STK Task3Stk[TASK_STK_SIZE]; /* Task #3 stack */
//OS_STK Task4Stk[TASK_STK_SIZE]; /* Task #4 stack */

/* ...........................................................................
 *
 * ����� �޸� ����
 * ===================
 */
TASK_USER_DATA  TaskUserData[7];

/* ...........................................................................
 *
 * �̺�Ʈ ��Ʈ�� & ����� ���� �� ����
 * ===================
 */
OS_EVENT *sem_id;	/* Semaphore Pointer */

extern INT32U USE_OSTimeTickHook;

/* ...........................................................................
 *
 * �½�ũ �Լ� ����
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  spost(void *pdata);
void  Task2(void *pdata);          
void  Task3(void *pdata);
void  Task4(void *pdata);

//
// ���ø����̼� ���� ��ƾ
//
int main(void)
{
	OSInit(); /* microC/OS-II �ʱ�ȭ */

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
// �½�ũ 'START'
//
void  TaskStart (void *pdata)
{
	INT8U err;
	int sem_init;
	OS_SEM_DATA sem_val;	// semaphore control
	int loops;

	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */
	
	USE_OSTimeTickHook = 0; 
	
	// �ý��� �ʱ�ȭ(UART, LCD, TimeTick)
	InitSystem();	

	// Task Statistics Title Display
	TaskStatisticsTitleDisplay(filename);

	OSStatInit();				/* Initialize uC/OS-II's ��� �Լ� */

	// �������� �ʱⰪ
	sem_init = 1;

	/* TODO #1: 
		create a binary semaphore
	    use sem_id and sem_init */
#if 1
    sem_id = OSSemCreate(sem_init);
#endif // TODO #1

	// �������� ����
    err = OSSemQuery(sem_id, &sem_val);
    if (err) printf("OSSemQuery error found, code[0x%X]\n",err);

	strcpy(TaskUserData[TASK_1_ID].TaskName, "spost");

	// TASK CREATE
	OSTaskCreateExt(spost,
	                (void *)0,
	                &Task1Stk[TASK_STK_SIZE - 1],
	                TASK_1_PRIO,
	                TASK_1_ID,
	                &Task1Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_1_ID],
	                0);
	// loop counter
	loops = 10;


    for(;;) {
		// �������� ���
		OSSemPend(sem_id, 0, &err);
        if (err) printf(" main OSSemPend error found, code[0x%X]\n",err);

		printf(" main %d ",loops);

    	err = OSSemQuery(sem_id, &sem_val);
    	if (err) printf("OSSemQuery error found, code[0x%X]\n",err);

		if(loops == 0)
			break; // exit

		loops--;

        err = OSSemPost(sem_id);
        if (err) printf(" main OSSemPost error found, code[0x%X]\n",err);
 
       OSTimeDly(100L);

	}

	/* TODO #3: 
		delete a semaphore */
#if 1
    //sem_id = OSSemDel(sem_id, OS_DEL_ALWAYS, &err);
    //if (err) printf("OSSemDel error found, code[0x%X]\n",err);
#endif // TODO #3

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

//
// �½�ũ 'spost'
//
void spost(void *pdata)
{
	INT8U err=0;
	
	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */

	int loops=20;

    printf("spost TASK created,\n");

	for(;;) {

	OSSemPend(sem_id, 0, &err);
        if (err) printf(" spot OSSemPend error found, code[0x%X]\n",err);

	/* TODO #2: 
		post to a semaphore */
#if 1
        err = OSSemPost(sem_id);
#endif // TODO #2
        if (err) printf(" spot OSSemPost error found, code[0x%X]\n",err);
        OSTimeDly(100L);

		loops--;
		printf(" spot %d \n ",loops);

 	}
}

/*-----------------------------------------------------------------------------
 * Program : 06_SEM.C
-----------------------------------------------------------------------------*/