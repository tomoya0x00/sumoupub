;;; 1CH H-Bridge Motor-Driver Input PullDown
;;; PIC16F84 10MHz 1cycle 4clock 400nsec
;;; Time-stamp: <2008-09-03 19:37:06 Tomoya>
;;;
;;; RB0~RB3 FET
;;; RA0-CW/CCW, RA1-BRK, RA2-PWM
;;; RA0:        HI/LOW -> CW/CCW
;;; PA1:        HI/LOW -> NO_BRK/BRK
;;; PA2:        HI/LOW -> PWM/STOP


        list p=16f84            ; �v���Z�b�T�̎w��

        #include "p16f84.inc"   ; ��`�t�@�C���̓ǂݎ��

        ;; �R���t�B�O���[�V�����r�b�g�̐ݒ�
        ;; _CP_OFF		�R�s�[�v���e�N�g����
        ;; _PWRTE_ON	�d����������̃��Z�b�g���Ԃ�L��
        ;; _WDT_OFF		�E�H�b�`�h�b�O�^�C�}����
        ;; _HS_OSC		�I�V���[�^�̎w��(4~20MHz)
        __config _CP_OFF & _PWRTE_ON & _WDT_OFF & _HS_OSC

;;; �f�[�^�Z�O�����g

BRK		EQU		B'00001010'     ; �u���[�L���̏o��
CW		EQU		B'00001001'     ; ���]���̏o��
CCW		EQU		B'00000110'     ; �t�]���̏o��
WAIT	EQU		D'3'            ; �f�b�h�E�F�C�g�^�C��(800nsec * x)

CNT		EQU		H'000C'         ; �f�b�h�E�F�C�g�^�C���p�J�E���^

;;; �R�[�h�Z�O�����g

        ;; ���Z�b�g��0�Ԓn����n�܂�
        org		0x00            ; �������Ԓn�w��
        goto	init

        ;; ���荞�݂�4�Ԓn����n�܂�
        org		0x04            ; �������Ԓn�w��

        ;; ���C���̃v���O������8�Ԓn���珑���n�߂�
        org		0x08            ; �������Ԓn�w��

;;; ������ ���o�͂̐ݒ�
init
		;; �o�̓f�[�^���b�`�փ��C�g���ď�����
        CLRF	PORTB           ; PORTB���N���A

        bsf		STATUS, RP0		; �f�[�^�������̃o���N1��I��

        ;; RA0~2����͂ɃZ�b�g
        movlw	B'00000111'
        movwf	TRISA

        ;; RB0~4���o�͂ɃZ�b�g
        movlw	B'00000000'
        movwf	TRISB
      
        bcf		STATUS, RP0     ; �f�[�^�������̃o���N0��I��

;;; ���C�����[�`��
main_loop
        btfss	PORTA, 1        ; BRK�̓��͂��`�F�b�N
        goto	motor_brk       ; LOW�ł���΁CBRAKE
        btfss	PORTA, 2        ; PWM�̓��͂��`�F�b�N
        goto	motor_stop      ; LOW�ł���΁CSTOP
        btfsc	PORTA, 0        ; CW/CCW�̓��͂��`�F�b�N
        goto	motor_cw        ; HI�ł���΁CCW
        goto	motor_ccw       ; LOW�ł���΁CCCW
        
;;; ���[�^�u���[�L
motor_brk
        ;; PORTB�ɏo�͒��̐M�����X�g�b�v�M�����ۂ�
        CLRW
        addwf	PORTB, 0
        btfsc   STATUS, Z
        goto	brk_main ; �X�g�b�v�M���ł���΁C�f�b�h�^�C���E�F�C�g�𖳎�
        
        ;; PORTB�ƃu���[�L�M�����r
        movlw	BRK
        subwf	PORTB, 0
        btfss   STATUS, Z
        call	dead_time_wait  ; �قȂ��Ă���΃f�b�h�^�C���E�F�C�g��
brk_main
        ;; PORTB�Ƀu���[�L�M�����o��
        movlw	BRK
        movwf	PORTB
        
        goto	main_loop       ; ���C�����[�`���ɃW�����v

;;; ���[�^�X�g�b�v
motor_stop
        ;; PORTB�ɃX�g�b�v�M�����o��
        CLRF	PORTB
        
        goto	main_loop       ; ���C�����[�`���ɃW�����v

;;; ���[�^���]
motor_cw
        ;; PORTB�ɏo�͒��̐M�����X�g�b�v�M�����ۂ�
        CLRW
        addwf	PORTB, 0
        btfsc   STATUS, Z
        goto	cw_main ; �X�g�b�v�M���ł���΁C�f�b�h�^�C���E�F�C�g�𖳎�

        ;; PORTB�Ɛ��]�M�����r
        movlw	CW
        subwf	PORTB, 0
        btfss   STATUS, Z
        call	dead_time_wait  ; �قȂ��Ă���΃f�b�h�^�C���E�F�C�g��
cw_main
        ;; PORTB�ɐ��]�M�����o��
        movlw	CW
        movwf	PORTB
        
        goto	main_loop       ; ���C�����[�`���ɃW�����v

;;; ���[�^�t�]
motor_ccw
        ;; PORTB�ɏo�͒��̐M�����X�g�b�v�M�����ۂ�
        CLRW
        addwf	PORTB, 0
        btfsc   STATUS, Z
        goto	ccw_main ; �X�g�b�v�M���ł���΁C�f�b�h�^�C���E�F�C�g�𖳎�

        ;; PORTB�ƃu���[�L�M�����r
        movlw	CCW
        subwf	PORTB, 0
        btfss   STATUS, Z
        call	dead_time_wait  ; �قȂ��Ă���΃f�b�h�^�C���E�F�C�g��
ccw_main
        ;; PORTB�ɋt�]�M�����o��
        movlw	CCW
        movwf	PORTB
        
        goto	main_loop       ; ���C�����[�`���ɃW�����v

;;; �f�b�h�^�C���E�F�C�g
dead_time_wait
        ;; PORTB�ɃX�g�b�v�M�����o��
        CLRF	PORTB

        ;; �J�E���^�ɒl����
        movlw	WAIT
        movwf	CNT
wait_loop ;; �E�F�C�g�p���[�v(1���[�v��800nsec�E�F�C�g)
        decfsz 	CNT,1            ; �J�E���^���f�N�������g �J�E���^�̒l���`�F�b�N
        goto	wait_loop          ; �J�E���^���[���łȂ���΃��[�v
        
        return

        end
