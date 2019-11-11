/*******************************************************************************
**	File:
**
**		02_TIME.c
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
char* filename = "02_TIME";

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
struct tm {
  int tm_msec;  /* milli seconds */
  int tm_sec;   /* seconds after the minute, 0 to 60
                   (0 - 60 allows for the occasional leap second) */
  int tm_min;   /* minutes after the hour, 0 to 59 */
  int tm_hour;  /* hours since midnight, 0 to 23 */
};
// 1�ʸ��� �ֿܼ� �������� ǥ��(.)
extern INT32U USE_OSTimeTickHook;

//
// OSTimeDlyCustom
//
void OSTimeDlyCustom( struct tm *t )
{
	OSTimeDlyHMSM (t->tm_hour, t->tm_min, t->tm_sec, t->tm_msec);
}

/* ...........................................................................
 *
 * �½�ũ �Լ� ����
 * ===================
 */
void  TaskStart(void *pdata); /* Function prototypes of Startup task */
void  Task1(void *pdata);
void  Task2(void *pdata);       
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
	
	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */

	// �ý��� �ʱ�ȭ(UART, LCD, TimeTick)
	InitSystem();	

	OSStatInit();				/* Initialize uC/OS-II's ��� �Լ� */

	// Task Statistics Title Display
	TaskStatisticsTitleDisplay(filename);

	// 1�ʸ��� �ֿܼ� �������� ǥ��(.)
	USE_OSTimeTickHook = 1; // refer to 'OS_CPU_C.C'

    strcpy(TaskUserData[TASK_1_ID].TaskName, "Task1");
    OSTaskCreateExt(Task1,
                    (void *)0,
                    &Task1Stk[TASK_STK_SIZE - 1],
                    TASK_1_PRIO,
                    TASK_1_ID,
                    &Task1Stk[0],
                    TASK_STK_SIZE,
                    &TaskUserData[TASK_1_ID],
                    0);

	if (err) printf("OSTaskCreate error found, code[0x%X]\n",err);

#if 0 // No comment
	err = OSTaskSuspend(OS_PRIO_SELF);
    	if (err) printf("OSTaskSuspend error found, code[0x%X]\n",err);
#endif // TODO #3

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
	struct tm time_s;	// USER defined time structure

	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */

    printf("\n\nTask1 TASK created,\n");

	/* TODO #1: 
		Wait 5 Second from now on
		use 'OSTimeDly()' */
	printf("Wait 5 Second From now on\n");
#if 1
    OSTimeDly(500);
#endif // TODO #1
	printf("finished\n\n");

	/* TODO #2: 
		Wait 10 Second from now on
		use 'OSTimeDlyCustom()' : 5 line */
	printf("Wait 10 Second From now on\n");
#if 1
    time_s.tm_msec = 999;
    time_s.tm_sec = 9;
    time_s.tm_min = 0;
    time_s.tm_hour = 0;
	OSTimeDlyCustom(&time_s);
#endif // TODO #2
	printf("finished\n");

#if 1 // No comment
	err = OSTaskSuspend(OS_PRIO_SELF);
    	if (err) printf("OSTaskSuspend error found, code[0x%X]\n",err);
#endif // TODO #3

	// 1�ʸ��� �ܼ� ��� '����'
	USE_OSTimeTickHook = 0; // refer to 'OS_CPU_C.C'

	printf("\n\ntask1 terminated,\n");
	err = OSTaskDel(OS_PRIO_SELF);	/* delete self task */
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

/*-----------------------------------------------------------------------------
 * Program : 02_TIMER.C
-----------------------------------------------------------------------------*/
