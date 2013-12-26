/*
  自立型相撲ロボットKADAM6用プログラム
  
  Time-stamp: <2009-10-04 18:41:25 Tomoya>
*/

#ifdef LINUX                    /* Linux 上でコンパイルする場合 */
#include "../include/3052.h"
#else  /* それ以外の環境でコンパイルする場合 */
#include <3052.h>
#endif /* Def: LINUX */

#define MONITOR_WAIT	20      /* モニターモードの更新間隔 */
#define AWAITE_TIME		1000    /* await の調整定数 */
#define RETRY_DEF       60      /* リトライの周期（10ms単位） */
#define RETRY_TIME      120     /* リトライの時間 */
#define RETRY_STOP      250     /* リトライ時の停止時間基準 */
#define RETRY_CNT_MAX 	4       /* リトライ時の最大停止時間 */


#define PWM_CYCLE		260      /* PWMの周期 */
#define SERVO_CYCLE		60000     /* サーボモータの PWM 周期 */

#define Motor1_CW   	PB.DR.BIT.B4 /* モータ1の回転方向(CW) */
#define Motor2_CW   	PB.DR.BIT.B5 /* モータ2の回転方向(CW) */

#define Motor1_BRK  	PB.DR.BIT.B6 /* モータ1の動作モード選択 */
#define Motor2_BRK  	PB.DR.BIT.B7 /* モータ2の動作モード選択 */

#define Motor1_PWM  	ITU0.GRB  /* モータ1のPWMデューティー */
#define Motor2_PWM  	ITU1.GRB  /* モータ2のPWMデューティー */

#define SERVO_PWM 		ITU2.GRB  /* サーボのPWMデューティー */

/* 8個のMAセンサ */
#define MA				P2.DR.BYTE  /* H > L */

/* 右のMAセンサ */
#define MA_SIDE 		((P4.DR.BYTE) & 0x01) /* H > L */

/* HEXスイッチ */
#define HEX0 			((~P4.DR.BYTE >> 4) & 0x0f) /* L > H */
#define HEX1 			((~P3.DR.BYTE) & 0x0f) /* L > H */

/* 床センサ */
#define FLOOR 			((P8.DR.BYTE) & 0x0f) /* H > L */

/* マイクロスイッチ */
#define SW_A			P4.DR.BIT.B2 /* H > L */
#define SW_Z			P4.DR.BIT.B3 /* H > L */

/* 押しボタンスイッチ */
#define PUSH			P3.DR.BIT.B5 /* H > L */
// 全センサチェック用
#define SENSORS 		(~((MA == 0xff) && (MA_SIDE == 0x01)))

/* LED */ 
#define LED				P3.DR.BIT.B7

/* タイマ用カウンタクリア */ 
#define WAIT_CLR		wait_cnt = 0;await_cnt = 0;

/* 旗のセット */
#define FLAG_SET		for(SERVO_PWM = 5900; SERVO_PWM >= 4900; SERVO_PWM -= 100) wait(1);

/* 旗の展開 */
#define FLAG_GO			for(SERVO_PWM = 4900; SERVO_PWM <= 5900; SERVO_PWM += 100) wait(1);

long wait_cnt;                  /* ウェイト用カウンタ */
long await_cnt;                 /* ウェイト用カウンタ */
long retry_cnt;                 /* リトライ用カウンタ */
int START_FLAG = 1;             /* 遠隔スタート用フラグ */
int RETRY_IGNORE = 1;           /* リトライを無視する回数 */

void Start(void);               /* 立ち会い動作 */

// A前置き
void side_at0_r(void);
void side_at0_l(void);
void turn_at0_r(void);
void turn_at0_l(void);
void rear_at0_r(void);
void rear_at0_l(void);
// A右置き
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

void Battle(void);              /* 主動作 */

void normal_h(void);            /* 対高速 */
void flag_r(void);              /* 対右旗 */
void flag_l(void);              /* 対左旗 */
void flag_m(void);              /* 対両旗 回り込み */
void flag_center(void);         /* 対両旗 正面 */
void tachiai(void);             /* 立ち会い優先モード */

void init(void);                /* 各種初期化 */
void monitor(void);             /* シリアル通信でモニタリング */
void servo_cl(void);            /* サーボ制御モード */

void irq_ei(void);				/* IRQ の割り込み有効 */
void irq_di(void);				/* IRQ の割り込み無効 */
int limit(int *, int, int);     /* 数値範囲制限 */
void Motor1(int, int);          /* モータ1駆動 */
void Motor2(int, int);          /* モータ2駆動 */
void wait(long);                /* 10ms タイマー */
void await(long);               /* タイマー */
void retry_start(void);         /* リトライ開始 */
void retry_stop(void);          /* リトライ停止 */
void retry(void);               /* リトライ */
void drive(int, int, long);     /* 両モータ駆動 */
void stop(int, int);            /* 両モータ停止 */
