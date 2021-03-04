;/**
;  ******************************************************************************
;  * @file    app_lld_tests_iar_asm.s
;  * @brief   Common functionnalities for RF LLD tests that must be writen
;  *          in assembler for IAR compilator
;  ******************************************************************************
;  * @attention
;  *
;  * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
;  *
;  ******************************************************************************
;  */
;
;/**
; * @brief Active polling for a given delay
; * @param microsec the delay in us unit
; *
; * Caution: 
; *   - This function has been implemented for system clock set to 16MHz. It must
; *     be rewiewed (twice more NOP) for 32Mhz config.
; *   - This function can be interrupted if used in interruptible part of code.
; *   - Be carefull with the use of critical section that can interrupt the reel
; *     time during input microsec value.
; *   - If 0 is given as input
; */


                EXPORT  us_delay_16m
				AREA    |.text|, CODE, READONLY
					               
us_delay_16m 
loop_16m
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                SUBS  R0, R0, #1
                BHI   loop_16m
                BX    LR

	
                EXPORT  us_delay_32m
				AREA    |.text|, CODE, READONLY
					               
us_delay_32m 
loop_32m
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                nop
                SUBS  R0, R0, #1
                BHI   loop_32m
                BX    LR

               END