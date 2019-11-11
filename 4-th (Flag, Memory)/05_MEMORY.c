/*******************************************************************************
**	File:
**
**		05_MEMORY.c
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
char* filename = "05_MEMORY";	

/* ...........................................................................
 *
 * �½�ũ ���� ����
 * ===================
 */
OS_STK TaskStartStk[TASK_STK_SIZE]; /* START task stack */
//OS_STK Task1Stk[TASK_STK_SIZE]; /* Task #1 stack */
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
char *block, *buffer, *buffer2;
OS_MEM *pVMem;
INT8U allocBuf[100][64];

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
	OS_MEM_DATA mem_data;	/* memory block control structure */

	pdata = pdata;				/* �����Ϸ� ��� ���� ���� */

	// �ý��� �ʱ�ȭ(UART, LCD, TimeTick)
	InitSystem();	

	OSStatInit();				/* Initialize uC/OS-II's ��� �Լ� */

	// Task Statistics Title Display
	TaskStatisticsTitleDisplay(filename);

	/* TODO #1: 
		create a memory partition 
	    use pVMem and allocBuf[][] */
#if 1
	pVMem = OSMemCreate(&allocBuf[0][0], 100, 64, &err);
	if (err) printf("OSMemCreate error found, code[0x%X]\n",err);
	printf(" \n 111 \n");
 
#endif // TODO #1

	/* TODO #2: 
		get a memory block
	    use block */
#if 1
    block = OSMemGet(pVMem, &err);
    if (err) printf("OSMemGet error found, code[0x%X]\n",err);
	printf(" \n 222 \n");
#endif // TODO #2

	// �޸� ���� ����Ÿ('A') ����
*block = 'A';
	printf("Before *block[0x%08x] = %c\n",(int)&block[0], block[0]);

    err = OSMemQuery(pVMem, &mem_data);
    if (err) printf("OSMemQuery error found, code[0x%X]\n",err);

	/* TODO #2: 
		release a memory block */
*block = 'D';
	printf("After *block[0x%08x] = %c\n",(int)&block[0], block[0]);

#if 1
    err = OSMemPut(pVMem, (char *)block);

    if (err) printf("OSMemPut error found, code[0x%X]\n",err);
	printf(" \n 333 \n");
#endif // TODO #2

	printf("After *block[0x%08x] = %c\n",(int)&block[0], block[0]);
	
	/* delete self task */
	err = OSTaskDel(OS_PRIO_SELF);
	if (err) printf("OSTaskDel(OS_PRIO_SELF) error found, code[0x%X]\n",err);
}

/*-----------------------------------------------------------------------------
 * Program : 05_MEMORY.C
-----------------------------------------------------------------------------*/
