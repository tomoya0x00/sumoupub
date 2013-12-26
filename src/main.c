/*
  自立型相撲ロボットKADAM6用プログラム

  Time-stamp: <2009-10-09 15:38:39 Tomoya>
*/

//#define	DEBUG_FLOOR             /* 床センサの動作デバック */
//#define	DEBUG_START             /* 立ち会いデバッグ */
#define LINUX                   /* Linux 以外でコンパイルする場合はコメントアウト */

#ifdef LINUX                    /* Linux 上でコンパイルする場合 */
#include "../include/3052.h"
#include "../include/sci1.c"
#include "../include/sci1_printf.c"
#else  /* それ以外の環境でコンパイルする場合 */
#include <3052.h>
#include <sci1.c>
#include <sci1_printf.c>
#endif /* Def: LINUX */

#include <stdlib.h>
#include "main.h"

/* メイン */
int main(void)
{
    int start_cnt;
	int a;

    init();                     /* 初期化 */
	irq_di();					/* IRQ 割り込み不許可 */
	EI;							/* 割り込み有効 */
	stop(1, 1);					/* 両モータを空転 */

    if(HEX1 == 0x0f)
        Battle();		/* モニターモードであれば */
	
    if(PUSH)                    /* リセット時の動作 */
    {
        START_FLAG = 0;
        irq_ei();               /* IRQ割り込み有効 */
		LED = 1;
		if(!SENSORS)			/* センサーに反応が無ければ敵を探す */
			drive(100, -100, 0);
        Battle();
    }

    FLAG_SET;                   /* 旗をセット */
    LED = 1;                    /* LEDを点灯 */
    while(!PUSH);               /* PUSHが離されるまで待つ */
    wait(20);                   /* チャタリング防止 */
    while(PUSH && START_FLAG);  /* 開始信号を待つ */
    if(START_FLAG)              /* 遠隔スタートでなければ */
    {
        for(start_cnt = 0; start_cnt < 10; start_cnt++)
        {
            LED = ~LED;         /* LEDを点滅 */
	        wait(45);
        }
        LED = 0;                /* LEDを消灯 */
        START_FLAG = 0;
    }
	
	if((HEX1 >= 0x07) && (HEX1 <= 0x0d))
		wait(10);				/* 待ち長めなら */
	FLAG_GO;					/* 旗を降ろす（100ms） */
	if(((HEX1 == 0x02) || (HEX1 == 0x03))) /* 待ち短めなら */
		wait(5);
	else						/* 待ち普通なら */
		wait(15);

	if(HEX1 == 0x0e)			/* サーボ制御モードであれば */
	{
		LED = 1;
		while(1);
	}
		

    irq_ei();                   /* IRQ割り込み有効 */
	
#ifdef DEBUG_FLOOR
    drive(100, 100, 0);
    while(1);
#endif /* Def: DEBUG_FLOOR */
	
    Start();                    /* 立ち会い */

#ifdef DEBUG_START
	//	wait(20);
    stop(1, 1);
    while(1);
#endif /* Def: DEBUG_START */
    
    Battle();                   /* 主動作 */
}

/* 立ち会い動作選択 */
void Start(void)
{
    switch(HEX0)
    {
		/* A前置き */
        case 0x00:	drive(100, 100, 0);		break; /* A方向へ進む */
        case 0x01:	drive(-100, -100, 0);	break; /* B方向へ進む */
		case 0x02:  side_at0_r();			break; /* 右横攻撃 */
		case 0x03:  side_at0_l();			break; /* 左横攻撃 */
		case 0x04:  turn_at0_r();			break; /* 右後攻撃 */
		case 0x05:  turn_at0_l();			break; /* 左後攻撃 */
		case 0x06:  rear_at0_r();			break; /* 右向後退 */
		case 0x07:  rear_at0_l();			break; /* 左向後退 */
		/* A右置き */
		case 0x08:  side_at1_r();			break; /* 右横攻撃 */
		case 0x09:  side_at1_l();			break; /* 左横攻撃 */
		case 0x0a:  rear_at1_r();			break; /* 中間支点右回り込み */
		case 0x0b:  rear_at1_l();			break; /* 中間支点左回り込み */
		case 0x0c:  rear_at2_r();			break; /* 旗より支点右回り込み */
		case 0x0d:  rear_at2_l();			break; /* 旗より支点左回り込み */
		case 0x0e:  turn_at3_r();			break; /* 旗を支点に右回り込み */
		case 0x0f:  turn_at3_l();			break; /* 旗を支点に左回り込み */
		default:	drive(100, 100, 0);		break; /* 普通はA方向に進む */
    }
    
    return;
}

/* A前置き 右横攻撃 */
void side_at0_r(void)
{
	drive(-100, 100, 2);
	drive(100, 70, 0);

	return;
}

/* A前置き 左横攻撃 */
void side_at0_l(void)
{
	drive(100, -100, 2);
	drive(70, 100, 0);


	return;
}

/* A前置き 急右横攻撃 */
void turn_at0_r(void)
{
	drive(-100, 100, 3);
	drive(100, 65, 0);
	
	return;
}

/* A前置き 急左横攻撃 */
void turn_at0_l(void)
{
	drive(100, -100, 3);
	drive(65, 100, 0);

	return;
}

/* A前置き その場回転 右横攻撃 */
void rear_at0_r(void)
{
    drive(-100, 100, 4);
	if(HEX1 == 0x01)			/* 対低速なら待つ */
		{
			stop(1, 1);
			wait(20);
		}
	drive(100, 55, 20);
	drive(100, 70, 0);

	return;
}

/* A前置き その場回転 左横攻撃 */
void rear_at0_l(void)
{
    drive(-100, 100, 4);
	if(HEX1 == 0x01)			/* 対低速なら待つ */
		{
			stop(1, 1);
			wait(20);
		}
	drive(-100, -55, 20);
    drive(-100, -70, 0);
	
	return;
}

/* A右置き 右横攻撃 */
void side_at1_r(void)
{
    drive(100, -100, 7);
	drive(100, 75, 0);

	return;
}

/* A右置き 左横攻撃 */
void side_at1_l(void)
{
    drive(-100, 100, 7);
    drive(-100, -75, 0);

	return;
}

/* A右置き 中間支点 右回り込み */
void rear_at1_r(void)
{
	drive(100, 45, 18);
	drive(100, 55, 0);

	return;
}


/* A右置き 中間支点 左回り込み */
void rear_at1_l(void)
{
	drive(-100, -45, 18);
	drive(-100, -55, 0);
	
	return;
}

/* A右置き 旗より支点 右回り込み */
void rear_at2_r(void)
{
	drive(100, 55, 22);
	drive(100, 65, 0);

	return;
}


/* A右置き 旗より支点 左回り込み */
void rear_at2_l(void)
{
	drive(-100, -55, 22);
	drive(-100, -65, 0);
	
	return;
}

/* A右置き 旗を支点に右回り込み */
void turn_at3_r(void)
{
	if(HEX1 == 0x01)
		drive(100, 65, 40);		/* 対低速ならさらに回り込む */
	else
		drive(100, 65, 33);
	if(HEX1 == 0x05)			/* センサ無視モードなら */
		drive(100, 60, 0);
	else
		drive(100, 40, 0);

	return;
}

/* A右置き 旗を支点に左回り込み */
void turn_at3_l(void)
{
	if(HEX1 == 0x01)
		drive(-100, -65, 40);	/* 対低速ならさらに回り込む */
	else
		drive(-100, -65, 33);
	if(HEX1 == 0x05)			/* センサ無視モードなら */
		drive(-100, -60, 0);
	else
		drive(-100, -40, 0);

	return;
}

/* 主動作選択 */
void Battle(void)
{
    switch(HEX1)
    {
		case 0x00:  normal_h();		break; /* 対高速 */
		case 0x01:  retry_start();normal_h();   break; /* 対低速型 リトライ */
		case 0x02:  normal_h();    	break; /* 待ち短め　対高速 */
		case 0x03:  retry_start();normal_h();     	break; /* 待ち短め　対低速 */
		case 0x04:  flag_m();     	break; /* 対両旗 回り込み */
		case 0x05:  while(1);  		break; /* センサ無視モード */
//		case 0x05:  flag_center();  break; /* 対両旗 正面 */
		case 0x06:	tachiai();		break; /* 立ち会い優先モード */
		case 0x07:  normal_h();		break; /* 対高速 */
		case 0x08:  retry_start();normal_h();   break; /* 対低速型 リトライ */
		case 0x09:  flag_r();    	break; /* 対右旗 */
		case 0x0a:  flag_l();     	break; /* 対左旗 */
		case 0x0b:  flag_m();     	break; /* 対V字旗 */
		case 0x0c:  flag_center();   	break; /* 対一文字旗 */
		case 0x0d:	tachiai();		break; /* 立ち会い優先モード */
		case 0x0e:  servo_cl();   	break; /* サーボ制御モード */
		case 0x0f:  monitor();    	break; /* モニターモード */
		default:    normal_h();   	break; /* 普通は対高速 */
    }

	return;
}

/* 対高速 */
void normal_h(void)
{
    while(1)
    {
/* P2-0	MA A前左 */
/* P2-1	MA A前右 */
/* P2-2	MA A斜め左 */
/* P2-3	MA A斜め右 */
/* P2-4	MA B前左 */
/* P2-5	MA B前右 */
/* P2-6	MA B斜め左 */
/* P2-7	MA B斜め右 */
        switch(MA)
        {
			case 0xfc:  drive(100, 100, 0);		break; /* A前右, A前左 */
			case 0xf8:  drive(100, 80, 0);		break; /* A前右, A前左, A斜め左 */
			case 0xfe:  drive(100, 75, 0);		break; /* A前左 */
			case 0xfa:  drive(100, 50, 0);		break; /* A前左, A斜め左 */
			case 0xfb:  drive(100, 30, 0);		break; /* A斜め左 */
			case 0xf4:  drive(80, 100, 0);		break; /* A前右, A前左, A斜め右 */
			case 0xfd:  drive(75, 100, 0);		break; /* A前右 */
			case 0xf5:  drive(50, 100, 0);		break; /* A前右, A斜め右 */
			case 0xf7:  drive(30, 100, 0);		break; /* A斜め右 */
			case 0xcf:  drive(-100, -100, 0);	break; /* B前右, B前左 */
			case 0x8f:  drive(-80, -100, 0);	break; /* B前右, B前左, B斜め左 */
			case 0xef:  drive(-75, -100, 0);	break; /* B前左 */
			case 0xaf:  drive(-50, -100, 0);	break; /* B前左, B斜め左 */
			case 0xbf:  drive(-30, -100, 0);	break; /* B斜め左 */
			case 0x4f:  drive(-100, -80, 0);	break; /* B前右, B前左, B斜め右 */
			case 0xdf:  drive(-100, -75, 0);	break; /* B前右 */
			case 0x5f:  drive(-100, -50, 0);	break; /* B前右, B斜め右 */
			case 0x7f:  drive(-100, -30, 0);	break; /* B斜め右 */
			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);  break; /* A右 */
					default: break;
                }
                break;
            }
        }
    }    
}

/* 対右旗用 */
void flag_r(void)
{
    while(1)
    {
        switch(MA)
        {
/* P2-0	MA A前左 */
/* P2-1	MA A前右 */
/* P2-2	MA A斜め左 */
/* P2-3	MA A斜め右 */
/* P2-4	MA B前左 */
/* P2-5	MA B前右 */
/* P2-6	MA B斜め左 */
/* P2-7	MA B斜め右 */
			case 0xfc:	drive(100, 100, 0);	break; /* A前右, A前左 */
			case 0xf8:  drive(100, 80, 0);	break; /* A前右, A前左, A斜め左 */
			case 0xfe:  drive(100, 50, 0);	break; /* A前左 */
			case 0xfa:  drive(100, 0, 0);	break; /* A前左, A斜め左 */
			case 0xfb:  drive(100, 0, 0);	break; /* A斜め左 */
			case 0xf4:  drive(80, 100, 0);	break; /* A前右, A前左, A斜め右 */
			case 0xfd:  drive(50, 100, 0);	break; /* A前右 */
			case 0xf5:  drive(0, 100, 0);	break; /* A前右, A斜め右 */
			case 0xf7:  drive(0, 100, 0);	break; /* A斜め右 */
			case 0xcf:			/* B前右, B前左 */
            {
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);	break;	/* A右 */
					default: 	drive(-100, -100, 0);	break;
                }
                break;
            }
			case 0x8f:			/* B前右, B前左, B斜め左 */
	        {
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);	break;	/* A右 */
					default: 	drive(-80, -100, 0);	break;
                }
                break;
            }
			case 0xef:  drive(-50, -100, 0);	break; /* B前左 */
			case 0xaf:  drive(0, -100, 0);		break; /* B前左, B斜め左 */
			case 0xbf:  drive(0, -100, 0);		break; /* B斜め左 */
			case 0x4f:			/* B前右, B前左, B斜め右 */
	        {           
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);	break;	/* A右 */
					default: 	drive(-100, -80, 0);	break;
                }
                break;
            }
			case 0xdf:  drive(-100, -50, 0);	break; /* B前右 */
			case 0x5f:  drive(-100, 0, 0);		break; /* B前右, B斜め右 */
			case 0x7f:  drive(-100, 0, 0);		break; /* B斜め右 */
			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);	break;  /* A右 */
					default: break;
                }
                break;
            }
        }
    }    
}

/* 対左旗用 */
void flag_l(void)
{
    while(1)
    {
        switch(MA)
        {
/* P2-0	MA A前左 */
/* P2-1	MA A前右 */
/* P2-2	MA A斜め左 */
/* P2-3	MA A斜め右 */
/* P2-4	MA B前左 */
/* P2-5	MA B前右 */
/* P2-6	MA B斜め左 */
/* P2-7	MA B斜め右 */
			case 0xfc:			/* A前右, A前左 */
            {
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0); break;	/* A右 */
					default: 	drive(100, 100, 0);	 break;
                }
                break;
            }
			case 0xf8:			/* A前右, A前左, A斜め左 */
            {
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0); break;	/* A右 */
					default: 	drive(100, 80, 0);	 break;
                }
                break;
            }
			case 0xfe:  drive(100, 50, 0);		break; /* A前左 */
			case 0xfa:  drive(100, 0, 0);		break; /* A前左, A斜め左 */
			case 0xfb:  drive(100, 0, 0);		break; /* A斜め左 */
			case 0xf4:			/* A前右, A前左, A斜め右 */
			{
				
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0); break;	/* A右 */
					default: 	drive(80, 100, 0);	 break;
                }
                break;
			}
			case 0xfd:  drive(50, 100, 0);		break; /* A前右 */
			case 0xf5:  drive(0, 100, 0);		break; /* A前右, A斜め右 */
			case 0xf7:  drive(0, 100, 0);		break; /* A斜め右 */
			case 0xcf:  drive(-100, -100, 0);   break; /* B前右, B前左 */
			case 0x8f:	drive(-80, -100, 0);	break; /* B前右, B前左, B斜め左 */
			case 0xef:  drive(-50, -100, 0);	break; /* B前左 */
			case 0xaf:  drive(0, -100, 0);		break; /* B前左, B斜め左 */
			case 0xbf:  drive(0, -100, 0);		break; /* B斜め左 */
			case 0x4f:  drive(-100, -80, 0); 	break; /* B前右, B前左, B斜め右 */
			case 0xdf:  drive(-100, -50, 0);	break; /* B前右 */
			case 0x5f:  drive(-100, 0, 0);		break; /* B前右, B斜め右 */
			case 0x7f:  drive(-100, 0, 0);		break; /* B斜め右 */
			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);  break; /* A右 */
					default: break;
                }
                break;
            }
        }
    }    
}

/* 対両旗 回り込み */
void flag_m(void)
{
    while(1)
    {
/* P2-0	MA A前左 */
/* P2-1	MA A前右 */
/* P2-2	MA A斜め左 */
/* P2-3	MA A斜め右 */
/* P2-4	MA B前左 */
/* P2-5	MA B前右 */
/* P2-6	MA B斜め左 */
/* P2-7	MA B斜め右 */
        switch(MA)
        {
			case 0xfc:  drive(100, 100, 0);		break; /* A前右, A前左 */
			case 0xfe:  drive(100, 70, 0);		break; /* A前左 */
			case 0xfa:  drive(100, 50, 0);		break; /* A前左, A斜め左 */
			case 0xfb:  drive(100, 0, 0);		break; /* A斜め左 */
			case 0xfd:  drive(70, 100, 0);		break; /* A前右 */
			case 0xf5:  drive(50, 100, 0);		break; /* A前右, A斜め右 */
			case 0xf7:  drive(0, 100, 0);		break; /* A斜め右 */
			case 0xf0:			/* A前右, A前左, A斜め右, A斜め左 */ 
				{
					drive(-100, 100, 2);
					drive(100, 30, 32);
					drive(100, 100, 0);
					break;
				}
			case 0xcf:  drive(-100, -100, 0);	break; /* B前右, B前左 */
			case 0xef:  drive(-70, -100, 0);	break; /* B前左 */
			case 0xaf:  drive(-50, -100, 0);	break; /* B前左, B斜め左 */
			case 0xbf:  drive(0, -100, 0);		break; /* B斜め左 */
			case 0xdf:  drive(-100, -70, 0);	break; /* B前右 */
			case 0x5f:  drive(-100, -50, 0);	break; /* B前右, B斜め右 */
			case 0x7f:  drive(-100, 0, 0);		break; /* B斜め右 */
			case 0x0f:			/* B前右, B前左, B斜め右, B斜め左 */ 
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
					case 0x00:  drive(100, -100, 0);  break; /* A右 */
					default: break;
                }
                break;
            }
        }
    }    
}

/* 対両旗 正面 */
void flag_center(void)
{
	    while(1)
    {
/* P2-0	MA A前左 */
/* P2-1	MA A前右 */
/* P2-2	MA A斜め左 */
/* P2-3	MA A斜め右 */
/* P2-4	MA B前左 */
/* P2-5	MA B前右 */
/* P2-6	MA B斜め左 */
/* P2-7	MA B斜め右 */
        switch(MA)
        {
			case 0xfc:  drive(100, 100, 0);		break; /* A前右, A前左 */
			case 0xfe:  drive(100, 70, 0);		break; /* A前左 */
			case 0xfa:  drive(100, 50, 0);		break; /* A前左, A斜め左 */
			case 0xfb:  drive(100, 0, 0);		break; /* A斜め左 */
			case 0xfd:  drive(70, 100, 0);		break; /* A前右 */
			case 0xf5:  drive(50, 100, 0);		break; /* A前右, A斜め右 */
			case 0xf7:  drive(0, 100, 0);		break; /* A斜め右 */
			case 0xf0:	drive(100, 100, 0);		break; /* A前右, A前左, A斜め右, A斜め左 */ 
			case 0xcf:  drive(-100, -100, 0);	break; /* B前右, B前左 */
			case 0xef:  drive(-70, -100, 0);	break; /* B前左 */
			case 0xaf:  drive(-50, -100, 0);	break; /* B前左, B斜め左 */
			case 0xbf:  drive(0, -100, 0);		break; /* B斜め左 */
			case 0xdf:  drive(-100, -70, 0);	break; /* B前右 */
			case 0x5f:  drive(-100, -50, 0);	break; /* B前右, B斜め右 */
			case 0x7f:  drive(-100, 0, 0);		break; /* B斜め右 */
			case 0x0f:	drive(-100, -100, 0);	break; /* B前右, B前左, B斜め右, B斜め左 */ 
			default:
        	{
                switch(MA_SIDE)
                {
					case 0x00:  drive(100, -100, 0);  break; /* A右 */
					default: break;
                }
                break;
            }
        }
    }    
}

/* 立ち会い優先モード */
void tachiai(void)
{
	while(1)
    {
/* P2-0	MA A前左 */
/* P2-1	MA A前右 */
/* P2-2	MA A斜め左 */
/* P2-3	MA A斜め右 */
/* P2-4	MA B前左 */
/* P2-5	MA B前右 */
/* P2-6	MA B斜め左 */
/* P2-7	MA B斜め右 */
        switch(MA)
        {
			case 0xfc:  drive(100, 100, 0);		break; /* A前右, A前左 */
			case 0xfe:  drive(100, 70, 0);		break; /* A前左 */
			case 0xfb:  drive(100, 20, 0);		break; /* A斜め左 */
			case 0xfd:  drive(70, 100, 0);		break; /* A前右 */
			case 0xf7:  drive(20, 100, 0);		break; /* A斜め右 */
			case 0xcf:  drive(-100, -100, 0);	break; /* B前右, B前左 */
			case 0xef:  drive(-70, -100, 0);	break; /* B前左 */
			case 0xbf:  drive(-20, -100, 0);	break; /* B斜め左 */
			case 0xdf:  drive(-100, -70, 0);	break; /* B前右 */
			case 0x7f:  drive(-100, -20, 0);	break; /* B斜め右 */
			default:	break;
        }
    }    
}

/* シリアル通信でモニタリング */
void monitor(void)
{
    char txb[10], rxb[10];

    SCI1_INIT(br57600, txb, 10, rxb, 10); /* シリアルポート初期化 */
	irq_di();					/* IRQ 割り込み不許可 */
    EI;                         /* 割り込み有効 */
    
    while(1)
    {
        SCI1_PRINTF("\033[2J"); /* 全画面消去 */
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
        wait(MONITOR_WAIT); /* 表示更新間隔 */
    }
}

/* サーボ制御モード */
void servo_cl(void)
{
	irq_di();					/* IRQ 割り込み不許可 */
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


/* 各種初期化 */
void init(void)
{
    P3.DDR = 0x80;              /* LED へ出力準備 */
    PA.DDR = 0xff;              /* PortA 全て出力設定 */
    PB.DDR = 0xff;              /* PortB 全て出力設定 */

    P2.PCR.BYTE = 0xff;         /* Port2 プルアップ設定 */
    P4.PCR.BYTE = 0xff;         /* Port4 プルアップ設定 */

    /* 左 DC モータ駆動準備 */
    ITU.TSTR.BIT.STR0 = 0;      /* タイマカウンタを停止 */
    ITU0.TCNT = 0;              /* タイマカウンタの値をクリア */
    ITU.TMDR.BIT.PWM0 = 1;      /* ITU0 PWM モード動作 */
    ITU0.TCR.BIT.TPSC = 1;      /* 駆動周波数の 1/1 でカウント */
    ITU0.TCR.BIT.CCLR = 1;    	/* GRA のコンペアマッチで TCNT クリア */
    ITU0.GRA = PWM_CYCLE; 		/* 周波数を設定 25MHz / (2 * 260) = 49KHz */
    ITU0.GRB = 0;              	/* デューティー比を設定 */
    ITU0.TIOR.BIT.IOB = 0;      /* TIOCB0 の出力を不許可 */
    
    ITU.TSTR.BIT.STR0 = 1;      /* タイマカウンタをスタート */

    /* 右 DC モータ駆動準備 */
    ITU.TSTR.BIT.STR1 = 0;      /* タイマカウンタを停止 */
    ITU1.TCNT = 0;              /* タイマカウンタの値をクリア */
    ITU.TMDR.BIT.PWM1 = 1;      /* ITU1 PWM モード動作 */
    ITU1.TCR.BIT.TPSC = 1;      /* 駆動周波数の 1/2 でカウント */
    ITU1.TCR.BIT.CCLR = 1;    	/* GRA のコンペアマッチで TCNT クリア */
    ITU1.GRA = PWM_CYCLE;     	/* 周波数を設定 25MHz / (2 * 260) = 49KHz */
    ITU1.GRB = 0;              	/* デューティー比を設定 */
    ITU1.TIOR.BIT.IOB = 0;      /* TIOCB1 の出力を不許可 */

    ITU.TSTR.BIT.STR1 = 1;      /* タイマカウンタをスタート */

    /* サーボモータ駆動準備 */
    ITU.TSTR.BIT.STR2 = 0;      /* タイマカウンタを停止 */
    ITU2.TCNT = 0;              /* タイマカウンタの値をクリア */
    ITU.TMDR.BIT.PWM2 = 1;      /* ITU2 PWM モード動作 */
    ITU2.TCR.BIT.TPSC = 3;      /* 駆動周波数の 1/8 でカウント */
    ITU2.TCR.BIT.CCLR = 1;    	/* GRA のコンペアマッチで TCNT クリア */
    ITU2.GRA = SERVO_CYCLE; 	/* 周期を設定 25MHz / (8 * 60000) = 52.1Hz 周期 19ms */
    ITU2.GRB = 6000;			/* デューティー比を設定 */

    ITU.TSTR.BIT.STR2 = 1;      /* タイマカウンタをスタート */

    /* 10ms タイマーの準備 */
    ITU3.TCR.BIT.CCLR = 1;    	/* GRA のコンペアマッチで TCNT クリア */
    ITU3.TCR.BIT.TPSC = 3;      /* 駆動周波数の 1/8 でカウント */
    ITU.TSTR.BIT.STR3 = 0;      /* タイマカウンタを停止 */
    ITU3.TCNT = 0;              /* タイマカウンタの値をクリア */
    ITU3.TIER.BIT.IMIEA = 1;    /* GRA のコンペアマッチで割り込み */
    ITU3.GRA = 31249; 			/* 周期を設定 25MHz / (8 * 31250) = 100Hz 周期 10msec */

    /* 10ms タイマーの準備 */
    ITU4.TCR.BIT.CCLR = 1;    	/* GRA のコンペアマッチで TCNT クリア */
    ITU4.TCR.BIT.TPSC = 3;      /* 駆動周波数の 1/8 でカウント */
    ITU.TSTR.BIT.STR4 = 0;      /* タイマカウンタを停止 */
    ITU4.TCNT = 0;              /* タイマカウンタの値をクリア */
    ITU4.TIER.BIT.IMIEA = 1;    /* GRA のコンペアマッチで割り込み */
    ITU4.GRA = 31249; 			/* 周期を設定 25MHz / (8 * 31250) = 100Hz 周期 10msec */

	/* 割り込みの設定 */
    INTC.ISCR.BIT.IRQ0SC = 1;   /* IRQ0 立下がりエッジで割り込み */
    INTC.ISCR.BIT.IRQ1SC = 1;   /* IRQ1 立下がりエッジで割り込み */
    INTC.ISCR.BIT.IRQ2SC = 1;   /* IRQ2 立下がりエッジで割り込み */
    INTC.ISCR.BIT.IRQ3SC = 1;   /* IRQ3 立下がりエッジで割り込み */
    SYSCR.BIT.NMIEG = 1;        /* NMI は立ち上がりエッジで割り込み */
    
    return;
}

/* 床センサA左 */
void int_irq2(void)
{
    DI;							/* 割り込み禁止 */

	stop(1, 1);
	await(30);
    drive(-100, -100, 0);
    await(10);
    drive(-60, -100, 0);
    await(20);
    drive(-70, -100, 0);
	WAIT_CLR;					/* ウェイト用カウンタをクリア */
    
    EI;							/* 割り込み許可 */

    return;
}

/* 床センサA右 */
void int_irq3(void)
{
    DI;							/* 割り込み禁止 */

	stop(1, 1);
	await(30);
    drive(-100, -100, 0);
    await(10);
    drive(-100, -60, 0);
    await(20);
    drive(-100, -70, 0);
	WAIT_CLR;					/* ウェイト用カウンタをクリア */

    EI;							/* 割り込み許可 */

    return;

}

/* 床センサB左 */
void int_irq0(void)
{
    DI;							/* 割り込み禁止 */

	stop(1, 1);
	await(30);
    drive(100, 100, 0);
    await(10);
    drive(100, 60, 0);
    await(20);
    drive(100, 70, 0);
	WAIT_CLR;					/* ウェイト用カウンタをクリア */

    EI;							/* 割り込み許可 */

    return;
}

/* 床センサB右 */
void int_irq1(void)
{
    DI;							/* 割り込み禁止 */

	stop(1, 1);
	await(30);
    drive(100, 100, 0);
    await(10);
    drive(60, 100, 0);
    await(20);
    drive(70, 100, 0);
	WAIT_CLR;					/* ウェイト用カウンタをクリア */

    EI;							/* 割り込み許可 */

    return;
}

/* IRQ の割り込み有効 */
void irq_ei(void)
{
    INTC.IER.BIT.IRQ0E = 1;     /* IRQ0 割り込み有効 */
    INTC.IER.BIT.IRQ1E = 1;     /* IRQ1 割り込み有効 */
    INTC.IER.BIT.IRQ2E = 1;     /* IRQ2 割り込み有効 */
    INTC.IER.BIT.IRQ3E = 1;     /* IRQ3 割り込み有効 */

	return;
}

/* IRQ の割り込み無効 */
void irq_di(void)
{
    INTC.IER.BIT.IRQ0E = 0;     /* IRQ0 割り込み無効 */
    INTC.IER.BIT.IRQ1E = 0;     /* IRQ1 割り込み無効 */
    INTC.IER.BIT.IRQ2E = 0;     /* IRQ2 割り込み無効 */
    INTC.IER.BIT.IRQ3E = 0;     /* IRQ3 割り込み無効 */

	return;
}


/* 数値範囲制限 */
int limit(int *value, int hi, int lo)
{
    if(*value < lo)
        *value = lo;
    else
        if(*value > hi)
            *value = hi;
    
    return *value;
}

/* モータ1駆動 */
void Motor1(int m1power, int m1brk)
{
    if(m1brk)
    {
        Motor1_PWM = PWM_CYCLE;
        Motor1_BRK = 0;
    }
    else
    {
        limit(&m1power, 99, -99); /* 数値範囲を制限 */
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

/* モータ2駆動 */
void Motor2(int m2power, int m2brk)
{
    if(m2brk)
    {
        Motor2_PWM = PWM_CYCLE;
        Motor2_BRK = 0;
    }
    else
    {
        limit(&m2power, 99, -99); /* 数値範囲を制限 */
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


/* 10ms タイマー */
void wait(long num)
{
    wait_cnt = num;             /* ウェイト用カウンタに値を代入 */

    ITU3.TCNT = 0;              /* タイマカウンタの値をクリア */
    ITU.TSTR.BIT.STR3 = 1;      /* タイマカウンタをスタート */
    while(wait_cnt > 0);        /* ウェイト用カウンタが 0 以下になるまで待つ */
    ITU.TSTR.BIT.STR3 = 0;      /* タイマカウンタを停止 */

    return;
}

/* タイマー */
void await(long num)
{
    for(await_cnt = AWAITE_TIME * num; await_cnt > 0; await_cnt--)
        nop;

    return;
}

/* リトライ開始 */
void retry_start(void)
{
    retry_cnt = RETRY_DEF;		/* リトライ用カウンタに値を代入 */

    ITU4.TCNT = 0;              /* タイマカウンタの値をクリア */
    ITU.TSTR.BIT.STR4 = 1;      /* タイマカウンタをスタート */

    return;
}

/* リトライ停止 */
void retry_stop(void)
{
	ITU.TSTR.BIT.STR4 = 0;      /* タイマカウンタを停止 */

	return;
}


/* リトライ */
void retry(void)
{
	static int RETRY_CNT = 1;

	if(RETRY_IGNORE)
	{
		RETRY_IGNORE--;
		return;
	}
	
	LED = 1;					/* LED点灯 */
	
	switch(MA)
	{
        case 0xfc:  drive(-70, -70, 0);		break; /* A前右, A前左 */
        case 0xf8:  drive(-50, -70, 0);		break; /* A前右, A前左, A斜め左 */
        case 0xfe:  drive(-40, -70, 0);		break; /* A前左 */
        case 0xfa:  drive(-30, -70, 0);		break; /* A前左, A斜め左 */
        case 0xfb:  drive(-30, -70, 0);		break; /* A斜め左 */
        case 0xf4:  drive(-70, -50, 0);		break; /* A前右, A前左, A斜め右 */
        case 0xfd:  drive(-70, -40, 0);		break; /* A前右 */
        case 0xf5:  drive(-70, -30, 0);		break; /* A前右, A斜め右 */
        case 0xf7:  drive(-70, -30, 0);		break; /* A斜め右 */
        case 0xcf:  drive(70, 70, 0);		break; /* B前右, B前左 */
        case 0x8f:  drive(70, 50, 0);		break; /* B前右, B前左, B斜め左 */
        case 0xef:  drive(70, 40, 0);		break; /* B前左 */
        case 0xaf:  drive(70, 30, 0);		break; /* B前左, B斜め左 */
        case 0xbf:  drive(70, 30, 0);		break; /* B斜め左 */
        case 0x4f:  drive(50, 70, 0);		break; /* B前右, B前左, B斜め右 */
        case 0xdf:  drive(40, 70, 0);		break; /* B前右 */
        case 0x5f:  drive(30, 70, 0);		break; /* B前右, B斜め右 */
        case 0x7f:  drive(30, 70, 0);		break; /* B斜め右 */
		default:	break;
	}
    await(RETRY_TIME);
	stop(1, 1);
	await(RETRY_STOP * RETRY_CNT);

	if(RETRY_CNT < RETRY_CNT_MAX)
		RETRY_CNT++;
	
	LED = 0;					/* LED消灯 */
	
    return;
}

/* 両モータ駆動 */
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

/* 両モータ停止 */
void stop(int mstop1, int mstop2)
{
    Motor1(0, mstop1);
    Motor2(0, mstop2);

	return;
}
   
/* コンペアマッチ割り込み（タイマー用） */
void int_imia3(void)
{
    ITU3.TSR.BIT.IMFA = 0;      /* 割り込みフラグクリア */

    if(wait_cnt > 0)
        wait_cnt--;      /* ウェイト用カウンタを 10ms 周期で引いていく */
    
    return;
}

/* コンペアマッチ割り込み（リトライ用） */
void int_imia4(void)
{
    ITU4.TSR.BIT.IMFA = 0;      /* 割り込みフラグクリア */

    if(retry_cnt > 0)
        retry_cnt--;      /* リトライ用カウンタを 10ms 周期で引いていく */
	if(retry_cnt == 0)
	{
		retry();		
	    retry_cnt = RETRY_DEF;		/* リトライ用カウンタに値を代入 */
		ITU4.TCNT = 0;              /* タイマカウンタの値をクリア */
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
