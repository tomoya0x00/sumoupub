/*
  ��Ω�����Х�ܥå�KADAM6�ѥץ����

  Time-stamp: <2009-10-09 15:38:39 Tomoya>
*/

//#define	DEBUG_FLOOR             /* �����󥵤�ư��ǥХå� */
//#define	DEBUG_START             /* Ω���񤤥ǥХå� */
#define LINUX                   /* Linux �ʳ��ǥ���ѥ��뤹����ϥ����ȥ����� */

#ifdef LINUX                    /* Linux ��ǥ���ѥ��뤹���� */
#include "../include/3052.h"
#include "../include/sci1.c"
#include "../include/sci1_printf.c"
#else  /* ����ʳ��δĶ��ǥ���ѥ��뤹���� */
#include <3052.h>
#include <sci1.c>
#include <sci1_printf.c>
#endif /* Def: LINUX */

#include <stdlib.h>
#include "main.h"

/* �ᥤ�� */
int main(void)
{
    int start_cnt;
	int a;

    init();                     /* ����� */
	irq_di();					/* IRQ �������Ե��� */
	EI;							/* ������ͭ�� */
	stop(1, 1);					/* ξ�⡼�����ž */

    if(HEX1 == 0x0f)
        Battle();		/* ��˥����⡼�ɤǤ���� */
	
    if(PUSH)                    /* �ꥻ�åȻ���ư�� */
    {
        START_FLAG = 0;
        irq_ei();               /* IRQ������ͭ�� */
		LED = 1;
		if(!SENSORS)			/* ���󥵡���ȿ����̵�����Ũ��õ�� */
			drive(100, -100, 0);
        Battle();
    }

    FLAG_SET;                   /* ���򥻥å� */
    LED = 1;                    /* LED������ */
    while(!PUSH);               /* PUSH��Υ�����ޤ��Ԥ� */
    wait(20);                   /* ���㥿����ɻ� */
    while(PUSH && START_FLAG);  /* ���Ͽ�����Ԥ� */
    if(START_FLAG)              /* ��֥������ȤǤʤ���� */
    {
        for(start_cnt = 0; start_cnt < 10; start_cnt++)
        {
            LED = ~LED;         /* LED������ */
	        wait(45);
        }
        LED = 0;                /* LED����� */
        START_FLAG = 0;
    }
	
	if((HEX1 >= 0x07) && (HEX1 <= 0x0d))
		wait(10);				/* �Ԥ�Ĺ��ʤ� */
	FLAG_GO;					/* ����ߤ���100ms�� */
	if(((HEX1 == 0x02) || (HEX1 == 0x03))) /* �Ԥ�û��ʤ� */
		wait(5);
	else						/* �Ԥ����̤ʤ� */
		wait(15);

	if(HEX1 == 0x0e)			/* ����������⡼�ɤǤ���� */
	{
		LED = 1;
		while(1);
	}
		

    irq_ei();                   /* IRQ������ͭ�� */
	
#ifdef DEBUG_FLOOR
    drive(100, 100, 0);
    while(1);
#endif /* Def: DEBUG_FLOOR */
	
    Start();                    /* Ω���� */

#ifdef DEBUG_START
	//	wait(20);
    stop(1, 1);
    while(1);
#endif /* Def: DEBUG_START */
    
    Battle();                   /* ��ư�� */
}

/* Ω����ư������ */
void Start(void)
{
    switch(HEX0)
    {
		/* A���֤� */
        case 0x00:	drive(100, 100, 0);		break; /* A�����ؿʤ� */
        case 0x01:	drive(-100, -100, 0);	break; /* B�����ؿʤ� */
		case 0x02:  side_at0_r();			break; /* �������� */
		case 0x03:  side_at0_l();			break; /* �������� */
		case 0x04:  turn_at0_r();			break; /* ���并�� */
		case 0x05:  turn_at0_l();			break; /* ���并�� */
		case 0x06:  rear_at0_r();			break; /* �������� */
		case 0x07:  rear_at0_l();			break; /* �������� */
		/* A���֤� */
		case 0x08:  side_at1_r();			break; /* �������� */
		case 0x09:  side_at1_l();			break; /* �������� */
		case 0x0a:  rear_at1_r();			break; /* ��ֻ����������� */
		case 0x0b:  rear_at1_l();			break; /* ��ֻ����������� */
		case 0x0c:  rear_at2_r();			break; /* ���������������� */
		case 0x0d:  rear_at2_l();			break; /* ���������������� */
		case 0x0e:  turn_at3_r();			break; /* ��������˱������� */
		case 0x0f:  turn_at3_l();			break; /* ��������˺������� */
		default:	drive(100, 100, 0);		break; /* ���̤�A�����˿ʤ� */
    }
    
    return;
}

/* A���֤� �������� */
void side_at0_r(void)
{
	drive(-100, 100, 2);
	drive(100, 70, 0);

	return;
}

/* A���֤� �������� */
void side_at0_l(void)
{
	drive(100, -100, 2);
	drive(70, 100, 0);


	return;
}

/* A���֤� �ޱ������� */
void turn_at0_r(void)
{
	drive(-100, 100, 3);
	drive(100, 65, 0);
	
	return;
}

/* A���֤� �޺������� */
void turn_at0_l(void)
{
	drive(100, -100, 3);
	drive(65, 100, 0);

	return;
}

/* A���֤� ���ξ��ž �������� */
void rear_at0_r(void)
{
    drive(-100, 100, 4);
	if(HEX1 == 0x01)			/* ����®�ʤ��Ԥ� */
		{
			stop(1, 1);
			wait(20);
		}
	drive(100, 55, 20);
	drive(100, 70, 0);

	return;
}

/* A���֤� ���ξ��ž �������� */
void rear_at0_l(void)
{
    drive(-100, 100, 4);
	if(HEX1 == 0x01)			/* ����®�ʤ��Ԥ� */
		{
			stop(1, 1);
			wait(20);
		}
	drive(-100, -55, 20);
    drive(-100, -70, 0);
	
	return;
}

/* A���֤� �������� */
void side_at1_r(void)
{
    drive(100, -100, 7);
	drive(100, 75, 0);

	return;
}

/* A���֤� �������� */
void side_at1_l(void)
{
    drive(-100, 100, 7);
    drive(-100, -75, 0);

	return;
}

/* A���֤� ��ֻ��� �������� */
void rear_at1_r(void)
{
	drive(100, 45, 18);
	drive(100, 55, 0);

	return;
}


/* A���֤� ��ֻ��� �������� */
void rear_at1_l(void)
{
	drive(-100, -45, 18);
	drive(-100, -55, 0);
	
	return;
}

/* A���֤� �������� �������� */
void rear_at2_r(void)
{
	drive(100, 55, 22);
	drive(100, 65, 0);

	return;
}


/* A���֤� �������� �������� */
void rear_at2_l(void)
{
	drive(-100, -55, 22);
	drive(-100, -65, 0);
	
	return;
}

/* A���֤� ��������˱������� */
void turn_at3_r(void)
{
	if(HEX1 == 0x01)
		drive(100, 65, 40);		/* ����®�ʤ餵��˲����� */
	else
		drive(100, 65, 33);
	if(HEX1 == 0x05)			/* ����̵��⡼�ɤʤ� */
		drive(100, 60, 0);
	else
		drive(100, 40, 0);

	return;
}

/* A���֤� ��������˺������� */
void turn_at3_l(void)
{
	if(HEX1 == 0x01)
		drive(-100, -65, 40);	/* ����®�ʤ餵��˲����� */
	else
		drive(-100, -65, 33);
	if(HEX1 == 0x05)			/* ����̵��⡼�ɤʤ� */
		drive(-100, -60, 0);
	else
		drive(-100, -40, 0);

	return;
}

/* ��ư������ */
void Battle(void)
{
    switch(HEX1)
    {
		case 0x00:  normal_h();		break; /* �й�® */
		case 0x01:  retry_start();normal_h();   break; /* ����®�� ��ȥ饤 */
		case 0x02:  normal_h();    	break; /* �Ԥ�û�ᡡ�й�® */
		case 0x03:  retry_start();normal_h();     	break; /* �Ԥ�û�ᡡ����® */
		case 0x04:  flag_m();     	break; /* ��ξ�� ������ */
		case 0x05:  while(1);  		break; /* ����̵��⡼�� */
//		case 0x05:  flag_center();  break; /* ��ξ�� ���� */
		case 0x06:	tachiai();		break; /* Ω����ͥ��⡼�� */
		case 0x07:  normal_h();		break; /* �й�® */
		case 0x08:  retry_start();normal_h();   break; /* ����®�� ��ȥ饤 */
		case 0x09:  flag_r();    	break; /* �б��� */
		case 0x0a:  flag_l();     	break; /* �к��� */
		case 0x0b:  flag_m();     	break; /* ��V���� */
		case 0x0c:  flag_center();   	break; /* �а�ʸ���� */
		case 0x0d:	tachiai();		break; /* Ω����ͥ��⡼�� */
		case 0x0e:  servo_cl();   	break; /* ����������⡼�� */
		case 0x0f:  monitor();    	break; /* ��˥����⡼�� */
		default:    normal_h();   	break; /* ���̤��й�® */
    }

	return;
}

/* �й�® */
void normal_h(void)
{
    while(1)
    {
/* P2-0	MA A���� */
/* P2-1	MA A���� */
/* P2-2	MA A�ФẸ */
/* P2-3	MA A�Фᱦ */
/* P2-4	MA B���� */
/* P2-5	MA B���� */
/* P2-6	MA B�ФẸ */
/* P2-7	MA B�Фᱦ */
        switch(MA)
        {
			case 0xfc:  drive(100, 100, 0);		break; /* A����, A���� */
			case 0xf8:  drive(100, 80, 0);		break; /* A����, A����, A�ФẸ */
			case 0xfe:  drive(100, 75, 0);		break; /* A���� */
			case 0xfa:  drive(100, 50, 0);		break; /* A����, A�ФẸ */
			case 0xfb:  drive(100, 30, 0);		break; /* A�ФẸ */
			case 0xf4:  drive(80, 100, 0);		break; /* A����, A����, A�Фᱦ */
			case 0xfd:  drive(75, 100, 0);		break; /* A���� */
			case 0xf5:  drive(50, 100, 0);		break; /* A����, A�Фᱦ */
			case 0xf7:  drive(30, 100, 0);		break; /* A�Фᱦ */
			case 0xcf:  drive(-100, -100, 0);	break; /* B����, B���� */
			case 0x8f:  drive(-80, -100, 0);	break; /* B����, B����, B�ФẸ */
			case 0xef:  drive(-75, -100, 0);	break; /* B���� */
			case 0xaf:  drive(-50, -100, 0);	break; /* B����, B�ФẸ */
			case 0xbf:  drive(-30, -100, 0);	break; /* B�ФẸ */
			case 0x4f:  drive(-100, -80, 0);	break; /* B����, B����, B�Фᱦ */
			case 0xdf:  drive(-100, -75, 0);	break; /* B���� */
			case 0x5f:  drive(-100, -50, 0);	break; /* B����, B�Фᱦ */
			case 0x7f:  drive(-100, -30, 0);	break; /* B�Фᱦ */
			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);  break; /* A�� */
					default: break;
                }
                break;
            }
        }
    }    
}

/* �б����� */
void flag_r(void)
{
    while(1)
    {
        switch(MA)
        {
/* P2-0	MA A���� */
/* P2-1	MA A���� */
/* P2-2	MA A�ФẸ */
/* P2-3	MA A�Фᱦ */
/* P2-4	MA B���� */
/* P2-5	MA B���� */
/* P2-6	MA B�ФẸ */
/* P2-7	MA B�Фᱦ */
			case 0xfc:	drive(100, 100, 0);	break; /* A����, A���� */
			case 0xf8:  drive(100, 80, 0);	break; /* A����, A����, A�ФẸ */
			case 0xfe:  drive(100, 50, 0);	break; /* A���� */
			case 0xfa:  drive(100, 0, 0);	break; /* A����, A�ФẸ */
			case 0xfb:  drive(100, 0, 0);	break; /* A�ФẸ */
			case 0xf4:  drive(80, 100, 0);	break; /* A����, A����, A�Фᱦ */
			case 0xfd:  drive(50, 100, 0);	break; /* A���� */
			case 0xf5:  drive(0, 100, 0);	break; /* A����, A�Фᱦ */
			case 0xf7:  drive(0, 100, 0);	break; /* A�Фᱦ */
			case 0xcf:			/* B����, B���� */
            {
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);	break;	/* A�� */
					default: 	drive(-100, -100, 0);	break;
                }
                break;
            }
			case 0x8f:			/* B����, B����, B�ФẸ */
	        {
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);	break;	/* A�� */
					default: 	drive(-80, -100, 0);	break;
                }
                break;
            }
			case 0xef:  drive(-50, -100, 0);	break; /* B���� */
			case 0xaf:  drive(0, -100, 0);		break; /* B����, B�ФẸ */
			case 0xbf:  drive(0, -100, 0);		break; /* B�ФẸ */
			case 0x4f:			/* B����, B����, B�Фᱦ */
	        {           
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);	break;	/* A�� */
					default: 	drive(-100, -80, 0);	break;
                }
                break;
            }
			case 0xdf:  drive(-100, -50, 0);	break; /* B���� */
			case 0x5f:  drive(-100, 0, 0);		break; /* B����, B�Фᱦ */
			case 0x7f:  drive(-100, 0, 0);		break; /* B�Фᱦ */
			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);	break;  /* A�� */
					default: break;
                }
                break;
            }
        }
    }    
}

/* �к����� */
void flag_l(void)
{
    while(1)
    {
        switch(MA)
        {
/* P2-0	MA A���� */
/* P2-1	MA A���� */
/* P2-2	MA A�ФẸ */
/* P2-3	MA A�Фᱦ */
/* P2-4	MA B���� */
/* P2-5	MA B���� */
/* P2-6	MA B�ФẸ */
/* P2-7	MA B�Фᱦ */
			case 0xfc:			/* A����, A���� */
            {
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0); break;	/* A�� */
					default: 	drive(100, 100, 0);	 break;
                }
                break;
            }
			case 0xf8:			/* A����, A����, A�ФẸ */
            {
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0); break;	/* A�� */
					default: 	drive(100, 80, 0);	 break;
                }
                break;
            }
			case 0xfe:  drive(100, 50, 0);		break; /* A���� */
			case 0xfa:  drive(100, 0, 0);		break; /* A����, A�ФẸ */
			case 0xfb:  drive(100, 0, 0);		break; /* A�ФẸ */
			case 0xf4:			/* A����, A����, A�Фᱦ */
			{
				
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0); break;	/* A�� */
					default: 	drive(80, 100, 0);	 break;
                }
                break;
			}
			case 0xfd:  drive(50, 100, 0);		break; /* A���� */
			case 0xf5:  drive(0, 100, 0);		break; /* A����, A�Фᱦ */
			case 0xf7:  drive(0, 100, 0);		break; /* A�Фᱦ */
			case 0xcf:  drive(-100, -100, 0);   break; /* B����, B���� */
			case 0x8f:	drive(-80, -100, 0);	break; /* B����, B����, B�ФẸ */
			case 0xef:  drive(-50, -100, 0);	break; /* B���� */
			case 0xaf:  drive(0, -100, 0);		break; /* B����, B�ФẸ */
			case 0xbf:  drive(0, -100, 0);		break; /* B�ФẸ */
			case 0x4f:  drive(-100, -80, 0); 	break; /* B����, B����, B�Фᱦ */
			case 0xdf:  drive(-100, -50, 0);	break; /* B���� */
			case 0x5f:  drive(-100, 0, 0);		break; /* B����, B�Фᱦ */
			case 0x7f:  drive(-100, 0, 0);		break; /* B�Фᱦ */
			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);  break; /* A�� */
					default: break;
                }
                break;
            }
        }
    }    
}

/* ��ξ�� ������ */
void flag_m(void)
{
    while(1)
    {
/* P2-0	MA A���� */
/* P2-1	MA A���� */
/* P2-2	MA A�ФẸ */
/* P2-3	MA A�Фᱦ */
/* P2-4	MA B���� */
/* P2-5	MA B���� */
/* P2-6	MA B�ФẸ */
/* P2-7	MA B�Фᱦ */
        switch(MA)
        {
			case 0xfc:  drive(100, 100, 0);		break; /* A����, A���� */
			case 0xfe:  drive(100, 70, 0);		break; /* A���� */
			case 0xfa:  drive(100, 50, 0);		break; /* A����, A�ФẸ */
			case 0xfb:  drive(100, 0, 0);		break; /* A�ФẸ */
			case 0xfd:  drive(70, 100, 0);		break; /* A���� */
			case 0xf5:  drive(50, 100, 0);		break; /* A����, A�Фᱦ */
			case 0xf7:  drive(0, 100, 0);		break; /* A�Фᱦ */
			case 0xf0:			/* A����, A����, A�Фᱦ, A�ФẸ */ 
				{
					drive(-100, 100, 2);
					drive(100, 30, 32);
					drive(100, 100, 0);
					break;
				}
			case 0xcf:  drive(-100, -100, 0);	break; /* B����, B���� */
			case 0xef:  drive(-70, -100, 0);	break; /* B���� */
			case 0xaf:  drive(-50, -100, 0);	break; /* B����, B�ФẸ */
			case 0xbf:  drive(0, -100, 0);		break; /* B�ФẸ */
			case 0xdf:  drive(-100, -70, 0);	break; /* B���� */
			case 0x5f:  drive(-100, -50, 0);	break; /* B����, B�Фᱦ */
			case 0x7f:  drive(-100, 0, 0);		break; /* B�Фᱦ */
			case 0x0f:			/* B����, B����, B�Фᱦ, B�ФẸ */ 
				{
					drive(100, -100, 2);
					drive(-100, -30, 32);
					drive(-100, -100, 0);
					break;
				}

			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);  break; /* A�� */
					default: break;
                }
                break;
            }
        }
    }    
}

/* ��ξ�� ���� */
void flag_center(void)
{
	    while(1)
    {
/* P2-0	MA A���� */
/* P2-1	MA A���� */
/* P2-2	MA A�ФẸ */
/* P2-3	MA A�Фᱦ */
/* P2-4	MA B���� */
/* P2-5	MA B���� */
/* P2-6	MA B�ФẸ */
/* P2-7	MA B�Фᱦ */
        switch(MA)
        {
			case 0xfc:  drive(100, 100, 0);		break; /* A����, A���� */
			case 0xfe:  drive(100, 70, 0);		break; /* A���� */
			case 0xfa:  drive(100, 50, 0);		break; /* A����, A�ФẸ */
			case 0xfb:  drive(100, 0, 0);		break; /* A�ФẸ */
			case 0xfd:  drive(70, 100, 0);		break; /* A���� */
			case 0xf5:  drive(50, 100, 0);		break; /* A����, A�Фᱦ */
			case 0xf7:  drive(0, 100, 0);		break; /* A�Фᱦ */
			case 0xf0:	drive(100, 100, 0);		break; /* A����, A����, A�Фᱦ, A�ФẸ */ 
			case 0xcf:  drive(-100, -100, 0);	break; /* B����, B���� */
			case 0xef:  drive(-70, -100, 0);	break; /* B���� */
			case 0xaf:  drive(-50, -100, 0);	break; /* B����, B�ФẸ */
			case 0xbf:  drive(0, -100, 0);		break; /* B�ФẸ */
			case 0xdf:  drive(-100, -70, 0);	break; /* B���� */
			case 0x5f:  drive(-100, -50, 0);	break; /* B����, B�Фᱦ */
			case 0x7f:  drive(-100, 0, 0);		break; /* B�Фᱦ */
			case 0x0f:	drive(-100, -100, 0);	break; /* B����, B����, B�Фᱦ, B�ФẸ */ 
			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);  break; /* A�� */
					default: break;
                }
                break;
            }
        }
    }    
}

/* Ω����ͥ��⡼�� */
void tachiai(void)
{
	while(1)
    {
/* P2-0	MA A���� */
/* P2-1	MA A���� */
/* P2-2	MA A�ФẸ */
/* P2-3	MA A�Фᱦ */
/* P2-4	MA B���� */
/* P2-5	MA B���� */
/* P2-6	MA B�ФẸ */
/* P2-7	MA B�Фᱦ */
        switch(MA)
        {
			case 0xfc:  drive(100, 100, 0);		break; /* A����, A���� */
			case 0xfe:  drive(100, 70, 0);		break; /* A���� */
			case 0xfb:  drive(100, 20, 0);		break; /* A�ФẸ */
			case 0xfd:  drive(70, 100, 0);		break; /* A���� */
			case 0xf7:  drive(20, 100, 0);		break; /* A�Фᱦ */
			case 0xcf:  drive(-100, -100, 0);	break; /* B����, B���� */
			case 0xef:  drive(-70, -100, 0);	break; /* B���� */
			case 0xbf:  drive(-20, -100, 0);	break; /* B�ФẸ */
			case 0xdf:  drive(-100, -70, 0);	break; /* B���� */
			case 0x7f:  drive(-100, -20, 0);	break; /* B�Фᱦ */
			default:	break;
        }
    }    
}

/* ���ꥢ���̿��ǥ�˥���� */
void monitor(void)
{
    char txb[10], rxb[10];

    SCI1_INIT(br57600, txb, 10, rxb, 10); /* ���ꥢ��ݡ��Ƚ���� */
	irq_di();					/* IRQ �������Ե��� */
    EI;                         /* ������ͭ�� */
    
    while(1)
    {
        SCI1_PRINTF("\033[2J"); /* �����̾õ� */
        SCI1_PRINTF("-------------------\n\r");
        SCI1_PRINTF("MA\t| 0x%2x\n\r", MA);
        SCI1_PRINTF("MA\t| 0b%8b\n\r", MA);
        SCI1_PRINTF("MA_SIDE | 0x%2x\n\r", MA_SIDE);
        SCI1_PRINTF("HEX0\t| 0x%2x\n\r", HEX0);
        SCI1_PRINTF("HEX1\t| 0x%2x\n\r", HEX1);
        SCI1_PRINTF("FLOOR\t| 0x%2x\n\r", FLOOR);
        SCI1_PRINTF("FLOOR\t| 0x%8b\n\r", FLOOR);
        SCI1_PRINTF("SW_A\t| %4d\n\r", SW_A);
        SCI1_PRINTF("SW_Z\t| %4d\n\r", SW_Z);
        SCI1_PRINTF("PUSH\t| %4d\n\r", PUSH);
        SCI1_PRINTF("-------------------\n\r");
        wait(MONITOR_WAIT); /* ɽ�������ֳ� */
    }
}

/* ����������⡼�� */
void servo_cl(void)
{
	irq_di();					/* IRQ �������Ե��� */
	while(1)
	{
		while(PUSH);
		FLAG_GO;
		wait(5);
		while(PUSH);
		FLAG_SET;
		wait(5);
	}		
}


/* �Ƽ����� */
void init(void)
{
    P3.DDR = 0x80;              /* LED �ؽ��Ͻ��� */
    PA.DDR = 0xff;              /* PortA ���ƽ������� */
    PB.DDR = 0xff;              /* PortB ���ƽ������� */

    P2.PCR.BYTE = 0xff;         /* Port2 �ץ륢�å����� */
    P4.PCR.BYTE = 0xff;         /* Port4 �ץ륢�å����� */

    /* �� DC �⡼����ư���� */
    ITU.TSTR.BIT.STR0 = 0;      /* �����ޥ����󥿤���� */
    ITU0.TCNT = 0;              /* �����ޥ����󥿤��ͤ򥯥ꥢ */
    ITU.TMDR.BIT.PWM0 = 1;      /* ITU0 PWM �⡼��ư�� */
    ITU0.TCR.BIT.TPSC = 1;      /* ��ư���ȿ��� 1/1 �ǥ������ */
    ITU0.TCR.BIT.CCLR = 1;    	/* GRA �Υ���ڥ��ޥå��� TCNT ���ꥢ */
    ITU0.GRA = PWM_CYCLE; 		/* ���ȿ������� 25MHz / (2 * 260) = 49KHz */
    ITU0.GRB = 0;              	/* �ǥ塼�ƥ���������� */
    ITU0.TIOR.BIT.IOB = 0;      /* TIOCB0 �ν��Ϥ��Ե��� */
    
    ITU.TSTR.BIT.STR0 = 1;      /* �����ޥ����󥿤򥹥����� */

    /* �� DC �⡼����ư���� */
    ITU.TSTR.BIT.STR1 = 0;      /* �����ޥ����󥿤���� */
    ITU1.TCNT = 0;              /* �����ޥ����󥿤��ͤ򥯥ꥢ */
    ITU.TMDR.BIT.PWM1 = 1;      /* ITU1 PWM �⡼��ư�� */
    ITU1.TCR.BIT.TPSC = 1;      /* ��ư���ȿ��� 1/2 �ǥ������ */
    ITU1.TCR.BIT.CCLR = 1;    	/* GRA �Υ���ڥ��ޥå��� TCNT ���ꥢ */
    ITU1.GRA = PWM_CYCLE;     	/* ���ȿ������� 25MHz / (2 * 260) = 49KHz */
    ITU1.GRB = 0;              	/* �ǥ塼�ƥ���������� */
    ITU1.TIOR.BIT.IOB = 0;      /* TIOCB1 �ν��Ϥ��Ե��� */

    ITU.TSTR.BIT.STR1 = 1;      /* �����ޥ����󥿤򥹥����� */

    /* �����ܥ⡼����ư���� */
    ITU.TSTR.BIT.STR2 = 0;      /* �����ޥ����󥿤���� */
    ITU2.TCNT = 0;              /* �����ޥ����󥿤��ͤ򥯥ꥢ */
    ITU.TMDR.BIT.PWM2 = 1;      /* ITU2 PWM �⡼��ư�� */
    ITU2.TCR.BIT.TPSC = 3;      /* ��ư���ȿ��� 1/8 �ǥ������ */
    ITU2.TCR.BIT.CCLR = 1;    	/* GRA �Υ���ڥ��ޥå��� TCNT ���ꥢ */
    ITU2.GRA = SERVO_CYCLE; 	/* ���������� 25MHz / (8 * 60000) = 52.1Hz ���� 19ms */
    ITU2.GRB = 6000;			/* �ǥ塼�ƥ���������� */

    ITU.TSTR.BIT.STR2 = 1;      /* �����ޥ����󥿤򥹥����� */

    /* 10ms �����ޡ��ν��� */
    ITU3.TCR.BIT.CCLR = 1;    	/* GRA �Υ���ڥ��ޥå��� TCNT ���ꥢ */
    ITU3.TCR.BIT.TPSC = 3;      /* ��ư���ȿ��� 1/8 �ǥ������ */
    ITU.TSTR.BIT.STR3 = 0;      /* �����ޥ����󥿤���� */
    ITU3.TCNT = 0;              /* �����ޥ����󥿤��ͤ򥯥ꥢ */
    ITU3.TIER.BIT.IMIEA = 1;    /* GRA �Υ���ڥ��ޥå��ǳ����� */
    ITU3.GRA = 31249; 			/* ���������� 25MHz / (8 * 31250) = 100Hz ���� 10msec */

    /* 10ms �����ޡ��ν��� */
    ITU4.TCR.BIT.CCLR = 1;    	/* GRA �Υ���ڥ��ޥå��� TCNT ���ꥢ */
    ITU4.TCR.BIT.TPSC = 3;      /* ��ư���ȿ��� 1/8 �ǥ������ */
    ITU.TSTR.BIT.STR4 = 0;      /* �����ޥ����󥿤���� */
    ITU4.TCNT = 0;              /* �����ޥ����󥿤��ͤ򥯥ꥢ */
    ITU4.TIER.BIT.IMIEA = 1;    /* GRA �Υ���ڥ��ޥå��ǳ����� */
    ITU4.GRA = 31249; 			/* ���������� 25MHz / (8 * 31250) = 100Hz ���� 10msec */

	/* �����ߤ����� */
    INTC.ISCR.BIT.IRQ0SC = 1;   /* IRQ0 Ω�����ꥨ�å��ǳ����� */
    INTC.ISCR.BIT.IRQ1SC = 1;   /* IRQ1 Ω�����ꥨ�å��ǳ����� */
    INTC.ISCR.BIT.IRQ2SC = 1;   /* IRQ2 Ω�����ꥨ�å��ǳ����� */
    INTC.ISCR.BIT.IRQ3SC = 1;   /* IRQ3 Ω�����ꥨ�å��ǳ����� */
    SYSCR.BIT.NMIEG = 1;        /* NMI ��Ω���夬�ꥨ�å��ǳ����� */
    
    return;
}

/* ������A�� */
void int_irq2(void)
{
    DI;							/* �����߶ػ� */

	stop(1, 1);
	await(30);
    drive(-100, -100, 0);
    await(10);
    drive(-60, -100, 0);
    await(20);
    drive(-70, -100, 0);
	WAIT_CLR;					/* ���������ѥ����󥿤򥯥ꥢ */
    
    EI;							/* �����ߵ��� */

    return;
}

/* ������A�� */
void int_irq3(void)
{
    DI;							/* �����߶ػ� */

	stop(1, 1);
	await(30);
    drive(-100, -100, 0);
    await(10);
    drive(-100, -60, 0);
    await(20);
    drive(-100, -70, 0);
	WAIT_CLR;					/* ���������ѥ����󥿤򥯥ꥢ */

    EI;							/* �����ߵ��� */

    return;

}

/* ������B�� */
void int_irq0(void)
{
    DI;							/* �����߶ػ� */

	stop(1, 1);
	await(30);
    drive(100, 100, 0);
    await(10);
    drive(100, 60, 0);
    await(20);
    drive(100, 70, 0);
	WAIT_CLR;					/* ���������ѥ����󥿤򥯥ꥢ */

    EI;							/* �����ߵ��� */

    return;
}

/* ������B�� */
void int_irq1(void)
{
    DI;							/* �����߶ػ� */

	stop(1, 1);
	await(30);
    drive(100, 100, 0);
    await(10);
    drive(60, 100, 0);
    await(20);
    drive(70, 100, 0);
	WAIT_CLR;					/* ���������ѥ����󥿤򥯥ꥢ */

    EI;							/* �����ߵ��� */

    return;
}

/* IRQ �γ�����ͭ�� */
void irq_ei(void)
{
    INTC.IER.BIT.IRQ0E = 1;     /* IRQ0 ������ͭ�� */
    INTC.IER.BIT.IRQ1E = 1;     /* IRQ1 ������ͭ�� */
    INTC.IER.BIT.IRQ2E = 1;     /* IRQ2 ������ͭ�� */
    INTC.IER.BIT.IRQ3E = 1;     /* IRQ3 ������ͭ�� */

	return;
}

/* IRQ �γ�����̵�� */
void irq_di(void)
{
    INTC.IER.BIT.IRQ0E = 0;     /* IRQ0 ������̵�� */
    INTC.IER.BIT.IRQ1E = 0;     /* IRQ1 ������̵�� */
    INTC.IER.BIT.IRQ2E = 0;     /* IRQ2 ������̵�� */
    INTC.IER.BIT.IRQ3E = 0;     /* IRQ3 ������̵�� */

	return;
}


/* �����ϰ����� */
int limit(int *value, int hi, int lo)
{
    if(*value < lo)
        *value = lo;
    else
        if(*value > hi)
            *value = hi;
    
    return *value;
}

/* �⡼��1��ư */
void Motor1(int m1power, int m1brk)
{
    if(m1brk)
    {
        Motor1_PWM = PWM_CYCLE;
        Motor1_BRK = 0;
    }
    else
    {
        limit(&m1power, 99, -99); /* �����ϰϤ����� */
        if(m1power != 0)
        {
            if(m1power > 0)
				Motor1_CW = 0;
            else
				Motor1_CW = 1;
       }		
		Motor1_BRK = 1;
		Motor1_PWM = abs(m1power) * PWM_CYCLE / 100;
    }

	return;
}

/* �⡼��2��ư */
void Motor2(int m2power, int m2brk)
{
    if(m2brk)
    {
        Motor2_PWM = PWM_CYCLE;
        Motor2_BRK = 0;
    }
    else
    {
        limit(&m2power, 99, -99); /* �����ϰϤ����� */
        if(m2power != 0)
        {
            if(m2power < 0)
				Motor2_CW = 1;
            else
				Motor2_CW = 0;
		}
		Motor2_BRK = 1;
		Motor2_PWM = abs(m2power) * PWM_CYCLE / 100;
    }

	return;
}


/* 10ms �����ޡ� */
void wait(long num)
{
    wait_cnt = num;             /* ���������ѥ����󥿤��ͤ����� */

    ITU3.TCNT = 0;              /* �����ޥ����󥿤��ͤ򥯥ꥢ */
    ITU.TSTR.BIT.STR3 = 1;      /* �����ޥ����󥿤򥹥����� */
    while(wait_cnt > 0);        /* ���������ѥ����󥿤� 0 �ʲ��ˤʤ�ޤ��Ԥ� */
    ITU.TSTR.BIT.STR3 = 0;      /* �����ޥ����󥿤���� */

    return;
}

/* �����ޡ� */
void await(long num)
{
    for(await_cnt = AWAITE_TIME * num; await_cnt > 0; await_cnt--)
        nop;

    return;
}

/* ��ȥ饤���� */
void retry_start(void)
{
    retry_cnt = RETRY_DEF;		/* ��ȥ饤�ѥ����󥿤��ͤ����� */

    ITU4.TCNT = 0;              /* �����ޥ����󥿤��ͤ򥯥ꥢ */
    ITU.TSTR.BIT.STR4 = 1;      /* �����ޥ����󥿤򥹥����� */

    return;
}

/* ��ȥ饤��� */
void retry_stop(void)
{
	ITU.TSTR.BIT.STR4 = 0;      /* �����ޥ����󥿤���� */

	return;
}


/* ��ȥ饤 */
void retry(void)
{
	static int RETRY_CNT = 1;

	if(RETRY_IGNORE)
	{
		RETRY_IGNORE--;
		return;
	}
	
	LED = 1;					/* LED���� */
	
	switch(MA)
	{
        case 0xfc:  drive(-70, -70, 0);		break; /* A����, A���� */
        case 0xf8:  drive(-50, -70, 0);		break; /* A����, A����, A�ФẸ */
        case 0xfe:  drive(-40, -70, 0);		break; /* A���� */
        case 0xfa:  drive(-30, -70, 0);		break; /* A����, A�ФẸ */
        case 0xfb:  drive(-30, -70, 0);		break; /* A�ФẸ */
        case 0xf4:  drive(-70, -50, 0);		break; /* A����, A����, A�Фᱦ */
        case 0xfd:  drive(-70, -40, 0);		break; /* A���� */
        case 0xf5:  drive(-70, -30, 0);		break; /* A����, A�Фᱦ */
        case 0xf7:  drive(-70, -30, 0);		break; /* A�Фᱦ */
        case 0xcf:  drive(70, 70, 0);		break; /* B����, B���� */
        case 0x8f:  drive(70, 50, 0);		break; /* B����, B����, B�ФẸ */
        case 0xef:  drive(70, 40, 0);		break; /* B���� */
        case 0xaf:  drive(70, 30, 0);		break; /* B����, B�ФẸ */
        case 0xbf:  drive(70, 30, 0);		break; /* B�ФẸ */
        case 0x4f:  drive(50, 70, 0);		break; /* B����, B����, B�Фᱦ */
        case 0xdf:  drive(40, 70, 0);		break; /* B���� */
        case 0x5f:  drive(30, 70, 0);		break; /* B����, B�Фᱦ */
        case 0x7f:  drive(30, 70, 0);		break; /* B�Фᱦ */
		default:	break;
	}
    await(RETRY_TIME);
	stop(1, 1);
	await(RETRY_STOP * RETRY_CNT);

	if(RETRY_CNT < RETRY_CNT_MAX)
		RETRY_CNT++;
	
	LED = 0;					/* LED���� */
	
    return;
}

/* ξ�⡼����ư */
void drive(int m1, int m2, long time)
{
    Motor1(m1, m1 == 0);
    Motor2(m2, m2 == 0);
    if(time != 0)
    {
        wait(time);
        stop(0, 0);
    }

	return;
}

/* ξ�⡼����� */
void stop(int mstop1, int mstop2)
{
    Motor1(0, mstop1);
    Motor2(0, mstop2);

	return;
}
   
/* ����ڥ��ޥå������ߡʥ����ޡ��ѡ� */
void int_imia3(void)
{
    ITU3.TSR.BIT.IMFA = 0;      /* �����ߥե饰���ꥢ */

    if(wait_cnt > 0)
        wait_cnt--;      /* ���������ѥ����󥿤� 10ms �����ǰ����Ƥ��� */
    
    return;
}

/* ����ڥ��ޥå������ߡʥ�ȥ饤�ѡ� */
void int_imia4(void)
{
    ITU4.TSR.BIT.IMFA = 0;      /* �����ߥե饰���ꥢ */

    if(retry_cnt > 0)
        retry_cnt--;      /* ��ȥ饤�ѥ����󥿤� 10ms �����ǰ����Ƥ��� */
	if(retry_cnt == 0)
	{
		retry();		
	    retry_cnt = RETRY_DEF;		/* ��ȥ饤�ѥ����󥿤��ͤ����� */
		ITU4.TCNT = 0;              /* �����ޥ����󥿤��ͤ򥯥ꥢ */
	}

    return;
}


void int_nmi(void)
{
    if(START_FLAG)
        START_FLAG = 0;
    else
    {
		stop(1, 1);
		while(1);
    }

	return;
}
