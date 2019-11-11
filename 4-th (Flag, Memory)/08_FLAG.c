/*******************************************************************************
**	File:
**
**		08_FLAG.c
**
**  Contents:
**		uC/OS-II programming
**
**	History:
**		Date        Name              Description
**		------------------------------------------------------------------------
**
*******************************************************************************/
#include "../../include/includes.h"

// filename
char* filename = "08_FLAG";

/* ...........................................................................
 *
 * �½�ũ ���� ����
 * ===================
 */
OS_STK TaskStartStk[TASK_STK_SIZE]; /* START task stack */
OS_STK Task1Stk[TASK_STK_SIZE]; /* Task #1 stack */
//OS_STK Task2Stk[TASK_STK_SIZE]; /* Task #2 stack */
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
#define ENGINE_OIL_PRES_OK 0x01 
#define ENGINE_OIL_TEMP_OK 0x02 
#define ENGINE_START 0x04 

OS_FLAG_GRP *group_id; 

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
	OS_FLAGS flag_value;
	OS_FLAG_GRP *pgrp; // event flag group
	
	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */

	// �ý��� �ʱ�ȭ(UART, LCD, TimeTick)
	InitSystem();	

	// Task Statistics Title Display
	TaskStatisticsTitleDisplay(filename);

	OSStatInit();				/* Initialize uC/OS-II's ��� �Լ� */

	/* TODO #1: 
		create a event flag
	    use group_id and initial value= 0*/
#if 1
    group_id = OSFlagCreate(0x00, &err);
    if (err) printf("OSFlagCreate error found, code[0x%X]\n",err);
#endif // TODO #1

	// ���ؽ� ����
    flag_value = OSFlagQuery(group_id, &err);
    if (err) printf("OSFlagQuery error found, code[0x%X]\n",err);

	strcpy(TaskUserData[TASK_1_ID].TaskName, "task1");

	// TASK CREATE
	err = OSTaskCreateExt(Task1,
	                (void *)0,
	                &Task1Stk[TASK_STK_SIZE - 1],
	                TASK_1_PRIO,
	                TASK_1_ID,
	                &Task1Stk[0],
	                TASK_STK_SIZE,
	                &TaskUserData[TASK_1_ID],
	                0);
    if (err) printf("OSTaskCreateExt error found, code[0x%X]\n",err);

	for(;;) {
	/* TODO #2:
		get flag 'ENGINE_OIL_PRES_OK' OR 'ENGINE_OIL_TEMP_OK' from TASK2 */
#if 1
		flag_value = OSFlagPend(group_id, ENGINE_OIL_PRES_OK + ENGINE_OIL_TEMP_OK, 
			OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);
		if (err) printf("OSFlagPend error found, code[0x%X]\n",err);
#endif // TODO #2

		printf(".");
	}

	// �̺�Ʈ �÷��� ����
	pgrp = OSFlagDel(group_id, OS_DEL_ALWAYS, &err); 
	if (pgrp == (OS_FLAG_GRP *)0) { 
	/* The event flag group was deleted */ 
	} 

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
  	
	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */

    printf("\n\nTask1 created,\n");

	for(;;) {
	/* TODO #2: 
		post flag 'ENGINE_OIL_PRES_OK' to TASK1 */
#if 1
		OSFlagPost(group_id, ENGINE_OIL_PRES_OK, OS_FLAG_SET, &err); 
		if (err) printf("OSFlagPost error found, code[0x%X]\n",err);
#endif // TODO #2

		OSTimeDly(100L);
	}
}

/*-----------------------------------------------------------------------------
 * Program : 08_FLAG.C
-----------------------------------------------------------------------------*/
