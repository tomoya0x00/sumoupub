;;; 1CH H-Bridge Motor-Driver Input PullDown
;;; PIC16F84 10MHz 1cycle 4clock 400nsec
;;; Time-stamp: <2008-09-03 19:37:06 Tomoya>
;;;
;;; RB0~RB3 FET
;;; RA0-CW/CCW, RA1-BRK, RA2-PWM
;;; RA0:        HI/LOW -> CW/CCW
;;; PA1:        HI/LOW -> NO_BRK/BRK
;;; PA2:        HI/LOW -> PWM/STOP


        list p=16f84            ; プロセッサの指定

        #include "p16f84.inc"   ; 定義ファイルの読み取り

        ;; コンフィグレーションビットの設定
        ;; _CP_OFF		コピープロテクト無効
        ;; _PWRTE_ON	電源投入直後のリセット期間を有効
        ;; _WDT_OFF		ウォッチドッグタイマ無効
        ;; _HS_OSC		オシレータの指定(4~20MHz)
        __config _CP_OFF & _PWRTE_ON & _WDT_OFF & _HS_OSC

;;; データセグメント

BRK		EQU		B'00001010'     ; ブレーキ時の出力
CW		EQU		B'00001001'     ; 正転時の出力
CCW		EQU		B'00000110'     ; 逆転時の出力
WAIT	EQU		D'3'            ; デッドウェイトタイム(800nsec * x)

CNT		EQU		H'000C'         ; デッドウェイトタイム用カウンタ

;;; コードセグメント

        ;; リセットで0番地から始まる
        org		0x00            ; メモリ番地指定
        goto	init

        ;; 割り込みで4番地から始まる
        org		0x04            ; メモリ番地指定

        ;; メインのプログラムは8番地から書き始める
        org		0x08            ; メモリ番地指定

;;; 初期化 入出力の設定
init
		;; 出力データラッチへライトして初期化
        CLRF	PORTB           ; PORTBをクリア

        bsf		STATUS, RP0		; データメモリのバンク1を選択

        ;; RA0~2を入力にセット
        movlw	B'00000111'
        movwf	TRISA

        ;; RB0~4を出力にセット
        movlw	B'00000000'
        movwf	TRISB
      
        bcf		STATUS, RP0     ; データメモリのバンク0を選択

;;; メインルーチン
main_loop
        btfss	PORTA, 1        ; BRKの入力をチェック
        goto	motor_brk       ; LOWであれば，BRAKE
        btfss	PORTA, 2        ; PWMの入力をチェック
        goto	motor_stop      ; LOWであれば，STOP
        btfsc	PORTA, 0        ; CW/CCWの入力をチェック
        goto	motor_cw        ; HIであれば，CW
        goto	motor_ccw       ; LOWであれば，CCW
        
;;; モータブレーキ
motor_brk
        ;; PORTBに出力中の信号がストップ信号か否か
        CLRW
        addwf	PORTB, 0
        btfsc   STATUS, Z
        goto	brk_main ; ストップ信号であれば，デッドタイムウェイトを無視
        
        ;; PORTBとブレーキ信号を比較
        movlw	BRK
        subwf	PORTB, 0
        btfss   STATUS, Z
        call	dead_time_wait  ; 異なっていればデッドタイムウェイトへ
brk_main
        ;; PORTBにブレーキ信号を出力
        movlw	BRK
        movwf	PORTB
        
        goto	main_loop       ; メインルーチンにジャンプ

;;; モータストップ
motor_stop
        ;; PORTBにストップ信号を出力
        CLRF	PORTB
        
        goto	main_loop       ; メインルーチンにジャンプ

;;; モータ正転
motor_cw
        ;; PORTBに出力中の信号がストップ信号か否か
        CLRW
        addwf	PORTB, 0
        btfsc   STATUS, Z
        goto	cw_main ; ストップ信号であれば，デッドタイムウェイトを無視

        ;; PORTBと正転信号を比較
        movlw	CW
        subwf	PORTB, 0
        btfss   STATUS, Z
        call	dead_time_wait  ; 異なっていればデッドタイムウェイトへ
cw_main
        ;; PORTBに正転信号を出力
        movlw	CW
        movwf	PORTB
        
        goto	main_loop       ; メインルーチンにジャンプ

;;; モータ逆転
motor_ccw
        ;; PORTBに出力中の信号がストップ信号か否か
        CLRW
        addwf	PORTB, 0
        btfsc   STATUS, Z
        goto	ccw_main ; ストップ信号であれば，デッドタイムウェイトを無視

        ;; PORTBとブレーキ信号を比較
        movlw	CCW
        subwf	PORTB, 0
        btfss   STATUS, Z
        call	dead_time_wait  ; 異なっていればデッドタイムウェイトへ
ccw_main
        ;; PORTBに逆転信号を出力
        movlw	CCW
        movwf	PORTB
        
        goto	main_loop       ; メインルーチンにジャンプ

;;; デッドタイムウェイト
dead_time_wait
        ;; PORTBにストップ信号を出力
        CLRF	PORTB

        ;; カウンタに値を代入
        movlw	WAIT
        movwf	CNT
wait_loop ;; ウェイト用ループ(1ループで800nsecウェイト)
        decfsz 	CNT,1            ; カウンタをデクリメント カウンタの値をチェック
        goto	wait_loop          ; カウンタがゼロでなければループ
        
        return

        end
