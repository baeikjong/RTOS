/*******************************************************************************
**	File:
**
**		01_TASKMAN.c
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
char* filename = "01_TASKMAN";

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

extern INT32U USE_OSTimeTickHook;

/* ...........................................................................
 *
 * �̺�Ʈ ��Ʈ�� & ����� ���� �� ����
 * ===================
 */
struct Param_types {	/* struct for parameter passing to task */
       char *msg;
       int  P1,P2;
} Param_Tbl; 
int	task1timer,task2timer;
extern void seedrand(int seed);
extern int randomnumber(void);

/* ...........................................................................
 *
 * �½�ũ �Լ� ����
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  Task1(void *pdata);
void  Task2(struct Param_types *Param);        
void  Task3(void *pdata);
void  Task4(void *pdata);

//
// ���ø����̼� ���� ��ƾ
//
int main(void)
{
	OSInit(); /* microC/OS-II �ʱ�ȭ */

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
// �½�ũ 'START'
//
void  TaskStart (void *pdata)
{
	INT8U err;
	struct Param_types *Param;
	
	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */
	
	USE_OSTimeTickHook = 1;
	
	// �ý��� �ʱ�ȭ(UART, LCD, TimeTick)
	InitSystem();	
 	// Task Statistics Title Display
	TaskStatisticsTitleDisplay(filename);
	OSStatInit();				/* Initialize uC/OS-II's ��� �Լ� */
	// TASK CREATE
	/* TODO #1: 
		Task1�� ����
		use 'OSTaskCreate' */
#if 1
	err = OSTaskCreate(Task1,
                 (void *)0,
                 &Task1Stk[TASK_STK_SIZE - 1],
                 TASK_1_PRIO
                 );
	if (err) printf("OSTaskCreate error found, code[0x%X]\n",err);
#endif // TODO #1

	// Set TASK2 parameter
	Param = &Param_Tbl;		/* get parameter tbl addr */
	Param->P1 = 111111;		/* set parameter */
	Param->P2 = 222222;
	strcpy(TaskUserData[TASK_2_ID].TaskName, "task2");

	// TASK CREATE
	err = OSTaskCreateExt((void(*)(void *))Task2,
	                (void *)Param,
	                &Task2Stk[TASK_STK_SIZE - 1],
	                TASK_2_PRIO,
	                TASK_2_ID,
	                &Task2Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_2_ID],
	                0);
	                
   	if (err) printf("OSTaskCreateExt error found, code[0x%X]\n",err);

	/* TODO #2: 
		Task1�� ����
		use 'OSTaskSuspend' */
#if 1
	err = OSTaskSuspend(TASK_1_PRIO);
    	if (err) printf("OSTaskSuspend error found, code[0x%X]\n",err);
#endif // TODO #2

	/* TODO #4: 
		Task1�� �켱 ������ 'TASK_3_PRIO' ���� ����
		use 'OSTaskChangePrio' and 'OSTaskResume' */
#if 1
	err = OSTaskChangePrio(TASK_1_PRIO, TASK_3_PRIO);
    	if (err) printf("OSTaskChangePrio error found, code[0x%X]\n",err);
    err = OSTaskResume(TASK_3_PRIO);
    	if (err) printf("OSTaskResume error found, code[0x%X]\n",err);
#endif // TODO #4

	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}
//
// �½�ũ 1
//
void Task1(void *pdata)
{
	INT8U err;
	OS_TCB task_data;

	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */

    printf("\n\nTask1 TASK created,\n");

	err = OSTaskQuery(OS_PRIO_SELF, &task_data);
    	if (err) printf("OSTaskQuery error found, code[0x%X]\n",err);

	printf("\n-------  Task1 information -------\n");
	printf("task1 id = %d \n",task_data.OSTCBPrio);
	printf("task1 priority = %d \n",task_data.OSTCBPrio);
	printf("task1 status = %d \n",task_data.OSTCBStat);
	printf("----------------------------------\n");

	while(1) {
	/* TODO #3: 
		�ڵ带 ���� �Ͽ� ����
		OSTimeDly() �ڵ带 �ּ� ó���� �� �� ����� �����Ѵ� */
#if 1 // No comment
		OSTimeDly(100L);
		printf("a"); fflush(stdout);	// ���� 'a' ���
#endif // TODO #3

		task1timer++;
	}
}

//
// �½�ũ 2
//
void Task2(struct Param_types *Param)
{
    printf("Task2 TASK created,\n");

	printf("\n-------  Task2 parameter passed from main --------\n");
	printf("task2 first parameter = %d \n",Param->P1);
	printf("task2 second parameter = %d \n",Param->P2);
	printf("--------------------------------------------------\n");

	while(1) {
	/* TODO #3: 
		�ڵ带 ���� �Ͽ� ����
		OSTimeDly() �ڵ带 �ּ� ó���� �� �� ����� �����Ѵ� */
#if 1 // No comment
		OSTimeDly(100L);
		printf("b"); fflush(stdout);	// ���� 'b' ���
#endif // TODO #3

		task2timer++;
	}
}

/*-----------------------------------------------------------------------------
 * Program : 01_TASKMAN.C
-----------------------------------------------------------------------------*/
