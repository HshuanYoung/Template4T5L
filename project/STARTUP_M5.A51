
$NOMOD51

R11_Module_Enable           EQU     1 ;0表示不启用R11模块 1表示启用R11模块

IF R11_Module_Enable == 1
;以下为配置选项
;------------------------------------------------------------------------------
Single_Double_Flag		    EQU     0	;0表示双缓存  1表示单缓存	
Bit16_Flag		            EQU     1	;0表示8位  1表示16位	
Display_Debug_Message_Flag	EQU     1	;0表示不显示调试信息  1表示显示调试信息
XRL_Position		        EQU     0	;０表示校验在最前面　１表示校验在最后面	
Align_16KB		            EQU     1	;０表示不补全16KB    1表示补全16KB		
Isr_XRL			            EQU     1

IF XRL_Position == 1	
	JpegLaber_OS_ADR1_H16   EQU 	0000H	
	JpegLaber_OS_ADR1_L16   EQU 	8000H
	JpegLaber_OS_ADR2_H16   EQU 	0001H	
	JpegLaber_OS_ADR2_L16   EQU 	4000H
	JpegData_ADR1_H16       EQU     (JpegLaber_OS_ADR1_H16)	
	JpegData_ADR1_L16       EQU     (JpegLaber_OS_ADR1_L16+1)
	JpegData_ADR2_H16       EQU     (JpegLaber_OS_ADR2_H16)	
	JpegData_ADR2_L16       EQU     (JpegLaber_OS_ADR2_L16+1)	
ELSE	
	JpegLaber_OS_ADR1_H16   EQU 	0000H	
	JpegLaber_OS_ADR1_L16   EQU 	8000H
	JpegLaber_OS_ADR2_H16   EQU 	0001H	
	JpegLaber_OS_ADR2_L16   EQU 	4000H
	JpegData_ADR1_H16       EQU     (JpegLaber_OS_ADR1_H16)	
	JpegData_ADR1_L16       EQU     (JpegLaber_OS_ADR1_L16)
	JpegData_ADR2_H16       EQU     (JpegLaber_OS_ADR2_H16)	
	JpegData_ADR2_L16       EQU     (JpegLaber_OS_ADR2_L16)	
ENDIF	
	Icon_Overlay_SP         EQU     06A0H
	Icon_Overlay_VP	        EQU     Icon_Overlay_SP+0
	Icon_Overlay_X	        EQU     Icon_Overlay_SP+1
	Icon_Overlay_Y	        EQU     Icon_Overlay_SP+2
	Icon_Overlay_L	        EQU     Icon_Overlay_SP+3
	Icon_Overlay_H	        EQU     Icon_Overlay_SP+4
	Icon_Overlay_Mode	    EQU     Icon_Overlay_SP+5
	Icon_Overlay_Page	    EQU     Icon_Overlay_SP+6
ENDIF  ;R11_Module_Enable

;		--------------------------------------------------------------------------
;  This file is part of the C51 Compiler package
;  Copyright (c) 1988-2005 Keil Elektronik GmbH and Keil Software, Inc.
;  Version 8.01
;
;  *** <<< Use Configuration Wizard in Context Menu >>> ***
;------------------------------------------------------------------------------
;  STARTUP.A51:  This code is executed after processor reset.
;
;  To translate this file use A51 with the following invocation:
;
;     A51 STARTUP.A51
;
;  To link the modified STARTUP.OBJ file to your application use the following
;  Lx51 invocation:
;
;     Lx51 your object file list, STARTUP.OBJ  controls
;
;------------------------------------------------------------------------------
;
;  User-defined <h> Power-On Initialization of Memory
;
;  With the following EQU statements the initialization of memory
;  at processor reset can be defined:
;
; <o> IDATALEN: IDATA memory size <0x0-0x100>
;     <i> Note: The absolute start-address of IDATA memory is always 0
;     <i>       The IDATA space overlaps physically the DATA and BIT areas.
IDATALEN        EQU     80H
;
; <o> XDATASTART: XDATA memory start address <0x0-0xFFFF>
;     <i> The absolute start address of XDATA memory
XDATASTART      EQU     0
;
; <o> XDATALEN: XDATA memory size <0x0-0xFFFF>
;     <i> The length of XDATA memory in bytes.
XDATALEN        EQU     0
;
; <o> PDATASTART: PDATA memory start address <0x0-0xFFFF>
;     <i> The absolute start address of PDATA memory
PDATASTART      EQU     0H
;
; <o> PDATALEN: PDATA memory size <0x0-0xFF>
;     <i> The length of PDATA memory in bytes.
PDATALEN        EQU     0H
;
;</h>
;------------------------------------------------------------------------------
;
;<h> Reentrant Stack Initialization
;
;  The following EQU statements define the stack pointer for reentrant
;  functions and initialized it:
;
; <h> Stack Space for reentrant functions in the SMALL model.
;  <q> IBPSTACK: Enable SMALL model reentrant stack
;     <i> Stack space for reentrant functions in the SMALL model.
IBPSTACK        EQU     0       ; set to 1 if small reentrant is used.
;  <o> IBPSTACKTOP: End address of SMALL model stack <0x0-0xFF>
;     <i> Set the top of the stack to the highest location.
IBPSTACKTOP     EQU     0xFF +1     ; default 0FFH+1
; </h>
;
; <h> Stack Space for reentrant functions in the LARGE model.
;  <q> XBPSTACK: Enable LARGE model reentrant stack
;     <i> Stack space for reentrant functions in the LARGE model.
XBPSTACK        EQU     0       ; set to 1 if large reentrant is used.
;  <o> XBPSTACKTOP: End address of LARGE model stack <0x0-0xFFFF>
;     <i> Set the top of the stack to the highest location.
XBPSTACKTOP     EQU     0xFFFF +1   ; default 0FFFFH+1
; </h>
;
; <h> Stack Space for reentrant functions in the COMPACT model.
;  <q> PBPSTACK: Enable COMPACT model reentrant stack
;     <i> Stack space for reentrant functions in the COMPACT model.
PBPSTACK        EQU     0       ; set to 1 if compact reentrant is used.
;
;   <o> PBPSTACKTOP: End address of COMPACT model stack <0x0-0xFFFF>
;     <i> Set the top of the stack to the highest location.
PBPSTACKTOP     EQU     0xFF +1     ; default 0FFH+1
; </h>
;</h>
;------------------------------------------------------------------------------
;
;  Memory Page for Using the Compact Model with 64 KByte xdata RAM
;  <e>Compact Model Page Definition
;
;  <i>Define the XDATA page used for PDATA variables.
;  <i>PPAGE must conform with the PPAGE set in the linker invocation.
;
; Enable pdata memory page initalization
PPAGEENABLE     EQU     0       ; set to 1 if pdata object are used.
;
; <o> PPAGE number <0x0-0xFF>
; <i> uppermost 256-byte address of the page used for PDATA variables.
PPAGE           EQU     0
;
; <o> SFR address which supplies uppermost address byte <0x0-0xFF>
; <i> most 8051 variants use P2 as uppermost address byte
PPAGE_SFR       DATA    0A0H


    P0	DATA	80H
	SP	DATA	81H
	DPL	DATA	82H
	DPH	DATA	83H
	PCON	DATA	87H
	TCON	DATA	88H
		TF1	BIT	TCON.7
		TR1	BIT	TCON.6
		TF0	BIT	TCON.5
		TR0	BIT	TCON.4
		IE1	BIT	TCON.3
		IT1	BIT	TCON.2
		IE0	BIT	TCON.1
		IT0	BIT	TCON.0
	TMOD	DATA	89H
	TH0	DATA	8CH
	TL0	DATA	8AH
	TH1	DATA	8DH
	TL1	DATA	8BH

	CKCON	DATA	8EH
	P1	DATA	90H

	DPC	DATA	93H
	PAGESEL	DATA	94H	
	D_PAGESEL	DATA	95H	

	SCON0	DATA	98H	
		TI0	BIT	SCON0.1
		RI0	BIT	SCON0.0
	SBUF0	DATA	99H
	SREL0H	DATA	0BAH
	SREL0L	DATA	0AAH

	SCON1	DATA	9BH	
	SBUF1	DATA	9CH
	SREL1H	DATA	0BBH
	SREL1L	DATA	09DH

	IEN2	DATA	9AH
	P2	DATA	0A0H
	IEN0	DATA	0A8H
	EA	BIT	IEN0.7	
                ET2     BIT     IEN0.5
                ES0     BIT     IEN0.4
                ET1     BIT     IEN0.3
	EX1	BIT	IEN0.2
                ET0     BIT     IEN0.1
	EX0	BIT	IEN0.0	

	IP0	DATA	0A9H
	P3	DATA	0B0H
	IEN1	DATA	0B8H
                ES3R    BIT     IEN1.5
                ES3T    BIT     IEN1.4
                ES2R    BIT     IEN1.3
                ES2T    BIT     IEN1.2
                ECAN    BIT     IEN1.1

	IP1	DATA	0B9H
	IRCON2	DATA	0BFH
	IRCON	DATA	0C0H
		TF2	BIT	IRCON.6
	T2CON	DATA	0C8H
	TRL2H	DATA	0CBH
	TRL2L	DATA	0CAH
	TH2	DATA	0CDH
	TL2	DATA	0CCH
	
	PSW	DATA	0D0H
	CY	BIT	PSW.7
	AC	BIT	PSW.6
	F0	BIT	PSW.5
	RS1	BIT	PSW.4
	RS0	BIT	PSW.3
	OV	BIT	PSW.2
	F1	BIT	PSW.1
	P	BIT	PSW.0
	ADCON	DATA	0D8H          
	ACC	DATA	0E0H
	B	DATA	0F0H

    MUX_SEL	DATA	0C9H
;dgus配置
;---------------------------------------------------------
	RAMMODE	DATA	0F8H
	APP_REQ	BIT	RAMMODE.7
	APP_EN	BIT	RAMMODE.6
	APP_RW	BIT	RAMMODE.5	;1=READ 0=WRITE
	APP_ACK	BIT	RAMMODE.4
	ADR_H	DATA	0F1H
	ADR_M	DATA	0F2H
	ADR_L	DATA	0F3H
	ADR_INC DATA	0F4H
	DATA3	DATA	0FAH
	DATA2	DATA	0FBH
	DATA1	DATA	0FCH
	DATA0	DATA	0FDH


IF R11_Module_Enable == 1
;调试信息定义开始，对应文件为r11_common.h
;------------------------------------------------------------------------------
EX1_Len	 	            EQU 20H
Picture_Count 	        EQU 021H
buf_tail 	            EQU 022H
buf_tail_H 	            EQU 022H
buf_tail_L 	            EQU 023H
buf_os_tail	            EQU 24H
buf_os_tail_H 	        EQU 24H
buf_os_tail_L 	        EQU 25H
Packet_Count 	        EQU 026H	
Packet_TotalCount	    EQU 027H	
	
JpegLaber_OS_ADR	    EQU 028H
JpegLaber_OS_ADR_3	    EQU 028H
JpegLaber_OS_ADR_2	    EQU 029H
JpegLaber_OS_ADR_1	    EQU 02AH
JpegLaber_OS_ADR_0	    EQU 02BH	
JpegLaber_DGUSII_VP 	EQU 0x2C
JpegLaber_DGUSII_VP_3 	EQU 0x2C
JpegLaber_DGUSII_VP_2 	EQU 0x2D
JpegLaber_DGUSII_VP_1	EQU 0x2E
JpegLaber_DGUSII_VP_0	EQU 0x2F
	
Other_Cmd_Count 			EQU 30H
Miss_Align_16KB_Count 		EQU 31H
NotDeal_OverBuffer_Count 	EQU 32H
Over_Max_16KB_Count 		EQU 33H
CRC_ERR_Count 				EQU 34H
data_write_f 				EQU 35H	
state 						EQU 36H
Kind 						EQU 37H
	
Icon_Num 					EQU 38H	
Max_16KB_Count				EQU 39H	
Big_Small_Flag				EQU 3AH	
Rx3_Len						EQU 3BH
END_CRC 					EQU 3CH
Dgus_Need_Restart 			EQU 3DH	
Restart_Dgus_Time 			EQU 3EH	
TimeOut_Count 				EQU 3FH	
	
TimeOut 					EQU 40H
Jpeg_Word_Len 				EQU 41H
Jpeg_Word_Len_H				EQU 41H	
Jpeg_Word_Len_L				EQU 42H		
Bit8_16_Flag				EQU 46H
 


?PR?Display_Debug_Message SEGMENT CODE
PUBLIC Display_Debug_Message
RSEG ?PR?Display_Debug_Message

?PR?Judge_Packet_Count SEGMENT CODE; //  ??????????
PUBLIC Judge_Packet_Count;     //????
RSEG ?PR?Judge_Packet_Count;    //????l?????????	

Display_Debug_Message:	

	MOV	ADR_H,#00H
	MOV	ADR_M,#03H	//06F0H~06FFH
	MOV	ADR_L,#78H
	MOV	ADR_INC,#01H
	MOV	RAMMODE,#8FH
	JNB	APP_ACK,$
	MOV	R2,#8
	MOV	R1,#20H
	Debug_Data_Diplay:	
	MOV     DATA3,@R1
	INC	R1
	MOV     DATA2,@R1
	INC	R1
	MOV     DATA1,@R1
	INC	R1
	MOV     DATA0,@R1
	INC	R1
	SETB    APP_EN
	JB      APP_EN,$
	DJNZ	R2,Debug_Data_Diplay
	MOV	RAMMODE,#00
	RET

CSEG    AT      0x0003
LJMP inter_extern0_1_fun

CSEG    AT      0x100
inter_extern0_1_fun:
	PUSH	ACC
	PUSH	DPH
	PUSH	DPL
IF Bit16_Flag  == 1	
	MOV	A,P2
	PUSH	P1
	MOV 	DPH,buf_tail_H
	MOV 	DPL,buf_tail_L
	MOVX 	@DPTR,A
	INC 	DPTR
IF Isr_XRL  == 1		
	XRL	END_CRC,A
ENDIF
	POP	ACC
	MOVX 	@DPTR,A
	INC 	DPTR
	
IF Isr_XRL  == 1		
	XRL	END_CRC,A
ENDIF
ELSE
	MOV	A,P1
	MOV 	DPH,buf_tail_H
	MOV 	DPL,buf_tail_L
	MOVX 	@DPTR,A
	INC 	DPTR
IF Isr_XRL  == 1		
	XRL	END_CRC,A
ENDIF
ENDIF
	

	MOV	buf_tail_H,DPH
	SETB 	buf_tail_H.7
	MOV 	buf_tail_L,DPL

	POP DPL
	POP DPH
	POP ACC
end_11:	RETI


Judge_Packet_Count:		


Judge_Packet_Count1:

	MOV		A,Packet_Count
	JZ		Packet_Count_Zero
	Packet_Count_Not_Zero:
	MOV		ADR_H,JpegLaber_OS_ADR+1
	MOV		ADR_M,JpegLaber_OS_ADR+2
	MOV		ADR_L,JpegLaber_OS_ADR+3		
	MOV		ADR_INC,#01H
	MOV		RAMMODE,#8FH
	JNB		APP_ACK,$
	MOV		DPH,buf_os_tail
	MOV		DPL,buf_os_tail+1
	MOV		R2,#32
	Wr_Vp_Loop1:
	MOV	R3,#128	
	Wr_Vp_Loop2:
	MOVX	A,@DPTR
	INC		DPTR
	MOV     DATA0,A
IF Isr_XRL  <> 1		
	XRL	END_CRC,A
ENDIF	
	
	
	MOVX	A,@DPTR
	INC		DPTR        	
	MOV     DATA1,A	
IF Isr_XRL  <> 1		
	XRL	END_CRC,A
ENDIF
	
	MOVX	A,@DPTR
	INC		DPTR	
	MOV     DATA2,A
IF Isr_XRL  <> 1		
	XRL	END_CRC,A
ENDIF

	MOVX	A,@DPTR
	INC		DPTR	
	MOV     DATA3,A		
IF Isr_XRL  <> 1		
	XRL	END_CRC,A
ENDIF
	

	
	
	SETB    APP_EN
	JB      APP_EN,$
	DJNZ	R3,Wr_Vp_Loop2
	DJNZ	R2,Wr_Vp_Loop1
	MOV		JpegLaber_OS_ADR+1,ADR_H
	MOV		JpegLaber_OS_ADR+2,ADR_M
	MOV		JpegLaber_OS_ADR+3,ADR_L
	MOV		RAMMODE,#00H 

	MOV		buf_os_tail,DPH
	MOV		buf_os_tail+1,DPL
	SETB	buf_os_tail.7		
	DEC		Packet_Count	
	;SJMP	Judge_Packet_Count1		
Packet_Count_Zero:
	RET

ENDIF ;R11_Module_Enable

;------------------------------------------------------------------------------

                NAME    ?C_STARTUP

?C_C51STARTUP   SEGMENT   CODE
?STACK          SEGMENT   IDATA

                RSEG    ?STACK
                DS      20

                EXTRN CODE (?C_START)
                PUBLIC  ?C_STARTUP

                CSEG    AT      0
?C_STARTUP:     LJMP    STARTUP1


	CSEG    AT  0F8H
	DB  0FFH,0FFH	;??? 00H 00H
	DB  'DWINT5'
		
		
		
		

                RSEG    ?C_C51STARTUP

STARTUP1:
	MOV  	PAGESEL,#01H
	MOV  	D_PAGESEL,#02H
	MOV	CKCON,#00H
	MOV	DPC,#00H
	MOV	T2CON,#70H
	MOV	MUX_SEL,#60H	
	MOV	RAMMODE,#00H	
	
IF IDATALEN <> 0
                MOV     R0,#IDATALEN - 1
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
ENDIF

IF XDATALEN <> 0
                MOV     DPTR,#XDATASTART
                MOV     R7,#LOW (XDATALEN)
  IF (LOW (XDATALEN)) <> 0
                MOV     R6,#(HIGH (XDATALEN)) +1
  ELSE
                MOV     R6,#HIGH (XDATALEN)
  ENDIF
                CLR     A
XDATALOOP:      MOVX    @DPTR,A
                INC     DPTR
                DJNZ    R7,XDATALOOP
                DJNZ    R6,XDATALOOP
ENDIF

IF PPAGEENABLE <> 0
                MOV     PPAGE_SFR,#PPAGE
ENDIF

IF PDATALEN <> 0
                MOV     R0,#LOW (PDATASTART)
                MOV     R7,#LOW (PDATALEN)
                CLR     A
PDATALOOP:      MOVX    @R0,A
                INC     R0
                DJNZ    R7,PDATALOOP
ENDIF

IF IBPSTACK <> 0
EXTRN DATA (?C_IBP)

                MOV     ?C_IBP,#LOW IBPSTACKTOP
ENDIF

IF XBPSTACK <> 0
EXTRN DATA (?C_XBP)

                MOV     ?C_XBP,#HIGH XBPSTACKTOP
                MOV     ?C_XBP+1,#LOW XBPSTACKTOP
ENDIF

IF PBPSTACK <> 0
EXTRN DATA (?C_PBP)
                MOV     ?C_PBP,#LOW PBPSTACKTOP
ENDIF

                MOV     SP,#?STACK-1

; This code is required if you use L51_BANK.A51 with Banking Mode 4
;<h> Code Banking
; <q> Select Bank 0 for L51_BANK.A51 Mode 4
#if 0
;     <i> Initialize bank mechanism to code bank 0 when using L51_BANK.A51 with Banking Mode 4.
EXTRN CODE (?B_SWITCH0)
                CALL    ?B_SWITCH0      ; init bank mechanism to code bank 0
#endif
;</h>


IF R11_Module_Enable == 1
		IF Bit16_Flag  == 1	
			MOV		Bit8_16_Flag,#02H
		ELSE
			MOV		Bit8_16_Flag,#01H
		ENDIF
ENDIF ;R11_Module_Enable
        LJMP    ?C_START

        END


