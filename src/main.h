/*
  ��Ω�����Х�ܥå�KADAM6�ѥץ����
  
  Time-stamp: <2009-10-04 18:41:25 Tomoya>
*/

#ifdef LINUX                    /* Linux ��ǥ���ѥ��뤹���� */
#include "../include/3052.h"
#else  /* ����ʳ��δĶ��ǥ���ѥ��뤹���� */
#include <3052.h>
#endif /* Def: LINUX */

#define MONITOR_WAIT	20      /* ��˥����⡼�ɤι����ֳ� */
#define AWAITE_TIME		1000    /* await ��Ĵ����� */
#define RETRY_DEF       60      /* ��ȥ饤�μ�����10msñ�̡� */
#define RETRY_TIME      120     /* ��ȥ饤�λ��� */
#define RETRY_STOP      250     /* ��ȥ饤������߻��ִ�� */
#define RETRY_CNT_MAX 	4       /* ��ȥ饤���κ�����߻��� */


#define PWM_CYCLE		260      /* PWM�μ��� */
#define SERVO_CYCLE		60000     /* �����ܥ⡼���� PWM ���� */

#define Motor1_CW   	PB.DR.BIT.B4 /* �⡼��1�β�ž����(CW) */
#define Motor2_CW   	PB.DR.BIT.B5 /* �⡼��2�β�ž����(CW) */

#define Motor1_BRK  	PB.DR.BIT.B6 /* �⡼��1��ư��⡼������ */
#define Motor2_BRK  	PB.DR.BIT.B7 /* �⡼��2��ư��⡼������ */

#define Motor1_PWM  	ITU0.GRB  /* �⡼��1��PWM�ǥ塼�ƥ��� */
#define Motor2_PWM  	ITU1.GRB  /* �⡼��2��PWM�ǥ塼�ƥ��� */

#define SERVO_PWM 		ITU2.GRB  /* �����ܤ�PWM�ǥ塼�ƥ��� */

/* 8�Ĥ�MA���� */
#define MA				P2.DR.BYTE  /* H > L */

/* ����MA���� */
#define MA_SIDE 		((P4.DR.BYTE) & 0x01) /* H > L */

/* HEX�����å� */
#define HEX0 			((~P4.DR.BYTE >> 4) & 0x0f) /* L > H */
#define HEX1 			((~P3.DR.BYTE) & 0x0f) /* L > H */

/* ������ */
#define FLOOR 			((P8.DR.BYTE) & 0x0f) /* H > L */

/* �ޥ��������å� */
#define SW_A			P4.DR.BIT.B2 /* H > L */
#define SW_Z			P4.DR.BIT.B3 /* H > L */

/* �����ܥ��󥹥��å� */
#define PUSH			P3.DR.BIT.B5 /* H > L */
// �����󥵥����å���
#define SENSORS 		(~((MA == 0xff) && (MA_SIDE == 0x01)))

/* LED */ 
#define LED				P3.DR.BIT.B7

/* �������ѥ����󥿥��ꥢ */ 
#define WAIT_CLR		wait_cnt = 0;await_cnt = 0;

/* ���Υ��å� */
#define FLAG_SET		for(SERVO_PWM = 5900; SERVO_PWM >= 4900; SERVO_PWM -= 100) wait(1);

/* ����Ÿ�� */
#define FLAG_GO			for(SERVO_PWM = 4900; SERVO_PWM <= 5900; SERVO_PWM += 100) wait(1);

long wait_cnt;                  /* ���������ѥ����� */
long await_cnt;                 /* ���������ѥ����� */
long retry_cnt;                 /* ��ȥ饤�ѥ����� */
int START_FLAG = 1;             /* ��֥��������ѥե饰 */
int RETRY_IGNORE = 1;           /* ��ȥ饤��̵�뤹���� */

void Start(void);               /* Ω����ư�� */

// A���֤�
void side_at0_r(void);
void side_at0_l(void);
void turn_at0_r(void);
void turn_at0_l(void);
void rear_at0_r(void);
void rear_at0_l(void);
// A���֤�
void side_at1_r(void);
void side_at1_l(void);
void turn_at1_r(void);
void turn_at1_l(void);
void rear_at1_r(void);
void rear_at1_l(void);
void rear_at2_r(void);
void rear_at2_l(void);
void turn_at3_r(void);
void turn_at3_l(void);

void Battle(void);              /* ��ư�� */

void normal_h(void);            /* �й�® */
void flag_r(void);              /* �б��� */
void flag_l(void);              /* �к��� */
void flag_m(void);              /* ��ξ�� ������ */
void flag_center(void);         /* ��ξ�� ���� */
void tachiai(void);             /* Ω����ͥ��⡼�� */

void init(void);                /* �Ƽ����� */
void monitor(void);             /* ���ꥢ���̿��ǥ�˥���� */
void servo_cl(void);            /* ����������⡼�� */

void irq_ei(void);				/* IRQ �γ�����ͭ�� */
void irq_di(void);				/* IRQ �γ�����̵�� */
int limit(int *, int, int);     /* �����ϰ����� */
void Motor1(int, int);          /* �⡼��1��ư */
void Motor2(int, int);          /* �⡼��2��ư */
void wait(long);                /* 10ms �����ޡ� */
void await(long);               /* �����ޡ� */
void retry_start(void);         /* ��ȥ饤���� */
void retry_stop(void);          /* ��ȥ饤��� */
void retry(void);               /* ��ȥ饤 */
void drive(int, int, long);     /* ξ�⡼����ư */
void stop(int, int);            /* ξ�⡼����� */
