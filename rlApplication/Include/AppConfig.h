#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

#define PRODUCTION_CODE 	0x80
#define USER_CODE 			0x40
#define BATTERY_MODE 		0x20

#define RTC_CHECK_MODE 		0x41
#define RTC_NO_CHECK_MODE	0x21
#define RTC_WRITE_MODE		0x31



#define EARTH_TAMPER_THRESHOLD  		5
#define EARTH_MAG_TAMPER_THRESHOLD  	2
#define BAT_EARTH_TAMPER_THRESHOLD  	20
#define NM_THRESHOLD_VOLTAGE   			11500      // we should check rc tamper if we reduce this further
#define COM_THRESHOLD_VOLTAGE   		11500      // disable communication below this voltage
#define MIN_THRESHOLD_VOLTAGE   		6000      // we should check rc tamper if we reduce this further
#define SLEEP_MODE_THRESHOLD    		8000
#define MIN_TAMPER_DETECTION_POWER		1000	//100 watts i.e 50 ma 
#define EARTH_TAMPER_I_THRESHOLD		50			//in ma
#define REFERENCEVOLTAGE 				24UL	//24000/1000
#define IMAX_POWER						720000UL
#define IMAX_CURRENT					30000
#define NM_CURRENT_THRESHOLD			900
#define OVER_LOAD_THRESHOLD				72000
#define OVER_VOLTAGE_THRESHOLD			28800
#define LOW_VOLTAGE_THRESHOLD			19200
#define IMAX_CURRENT_NM					50000
#define IMAX_FALSE_CURRENT_NM			75000UL
#define NM_MIN_PH_CURRENT_THRESHOLD		800
#define NM_MIN_NU_CURRENT_THRESHOLD		800

#define CALIB_LENGTH					20
#define REF_U							24000UL
#define REF_I							5000UL
#define REF_P							120000UL
#define POWER_FAIL_DET_THRESHOLD		10	//1.0V
#define POWER_FAIL_DET_THRESHOLD_LEVEL1	1690	//120.0V

#define STARTING_POWER_THRESHOLD_L	160
#define STARTING_APPPOWER_THRESHOLD_L	220
#define STARTING_POWER_THRESHOLD_CONST	240
#define STARTING_POWER_THRESHOLD_U	360
#define NO_LOAD_CURRENT_THRESHOLD	2
#define CONST_LOW_POWER_THRESHOLD	280
#define COVER_OPEN_WAIT_TIME		0x400

#define METER_CONSTANT				32	// for 2 decimal
#define FORCE_POWER_FAIL_THRESHOLD	POWER_FAIL_DET_THRESHOLD	//10000	//100V
#define EXIT_VOLTAGE_THRESHOLD		11000	//110V

#define RESTORE_VALUE_PFAIL_NORMAL	0
#define RESTORE_VALUE_PFAIL_RESET	5


#define METER_RATING_5_30		(1)
#define POWER_BASE_EARTH_DETECT	(0)

#define RCD_TAMPER_TERMINAL3	(1)
#define RCD_TAMPER_TERMINAL4	(1)
#define NM_HARD_TEST			(0)
#define LOG_NM_INFO				(1)
#define HARMONICS_LIMIT			3
#define HARMONICS_WAIT_TIME		5
#define ONE_SECOND_PULSE_VAL	112500UL
#define IS_DC_MAG_ENABLE		(1)


#define CT_TYPE_KASHYAP				(0)
#define CT_COMPENSATION_LS_LP		(1200)	//1.2A
#define CT_COMPENSATION_LS_CV		(5)		//0.5%
#define CT_COMPENSATION_US_LP		(20000)	//20A
#define CT_COMPENSATION_US_CV		(6)		//0.6%
#define IS_COVER_BLINK				(0)
#define R_SYMBOL_FOR_COOPEN			(1)
#define NM_BLINK_ND_U_THRESHOLD		(20000)
// tender mode controls



// meter utility
//----------------------------------------------
#define IRDA_TYPE_METER_HP			(0)
#define IRDA_TYPE_METER_AP			(0)
#define IRDA_TYPE_METER_AP_WARNGL	(0)
#define BOARD_TARGET_MVVNL			(0)
#define BOARD_TARGET_JABALPUR		(1)
#if (defined(BOARD_TARGET_JABALPUR) && (BOARD_TARGET_JABALPUR==1))
#define BOARD_TARGET_INDORE			(1)
#else
#define BOARD_TARGET_INDORE			(0)
#endif
#define GUJRAT_METER_0			(0)
#define IS_TARGET2_TENDER_0		(0)
//----------------------------------------------
#define IS_9600_BAUD			(1)
#define IS_2400_BAUD			(0)



#if (defined(IRDA_TYPE_METER_AP_WARNGL) && (IRDA_TYPE_METER_AP_WARNGL==1))
#define IS_DISPLAY_KEPT_ON				(1)
#define IS_DISPLAY_AUTO_SCROLL_ON		(1)
#define BATTERY_LCD_DISPLAY_DURATION	(187200UL)
#define DISPLAY_PUSH_MODE2_HOLD_TIME	(10)
#else
#define IS_DISPLAY_KEPT_ON				(0)
#define IS_DISPLAY_AUTO_SCROLL_ON		(0)
#define BATTERY_LCD_DISPLAY_DURATION	(0)
#define DISPLAY_PUSH_MODE2_HOLD_TIME	(60)

#endif


#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP==1))

	#define POWER_FAIL_PERSIT_TIME	(60)
	#define IS_ADD_NM_CURRENTS		(0)
	#define IS_PHASE_I_AVG			(0)
	#define IS_NU_I_AVG				(0)
	#define IS_EFF_I_AVG			(1)
	#define IS_EXPORT_METER			(0)
	
	#define IS_MD_2DECIMAL			(1)
	#define IS_MD_3DECIMAL			(0)
	
	//------------------------------------------------------------
	#if (defined(IS_TARGET2_TENDER_0) && (IS_TARGET2_TENDER_0==0))
	#define IRDA_COM_ENABLE			(1)
	#define UART_0_ENABLE			(0)
	#define UART_2_ENABLE			(1)
	#define OLD_AVG_U_AVG_C_LOGIC	(0)
	#define IS_DISPLAY_CONFIGURABLE	(1)	
	#define IS_DLMS_ENABLED_EEPL	(0)
	#define EEPL_NUMBER_OF_SEASON	(1)
	#define ISDO_HARMONICS_EEPL		(0)
	#define APPLY_DC_IMMUNITY_ENABLE	(1)
	#define ENABLE_CT_COMPENSATION	(1)
	#define IS_LCD_OFF_AFTER_REL_20SECS	(0)
	#define TENDER_MODE_NO_LOAD_TIME	(0)
	#if (defined(IRDA_TYPE_METER_AP_WARNGL) && (IRDA_TYPE_METER_AP_WARNGL==1))	
		#define IS_UP_DWN_DISPLAY	(0)
	#endif
	#if (defined(IRDA_TYPE_METER_AP_WARNGL) && (IRDA_TYPE_METER_AP_WARNGL==0))	
		#define IS_UP_DWN_DISPLAY	(1)
	#endif	
	#endif
	//------------------------------------------------------------
	
	//------------------------------------------------------------
	#if (defined(IS_TARGET2_TENDER_0) && (IS_TARGET2_TENDER_0==1))
	#define IRDA_COM_ENABLE			(1)
	#define UART_0_ENABLE			(0)
	#define UART_2_ENABLE			(1)
	#define OLD_AVG_U_AVG_C_LOGIC	(0)
	#define IS_DISPLAY_CONFIGURABLE	(1)	
	#define IS_DLMS_ENABLED_EEPL	(0)
	#define EEPL_NUMBER_OF_SEASON	(1)
	#define ISDO_HARMONICS_EEPL		(0)
	#define APPLY_DC_IMMUNITY_ENABLE	(0)
	#define ENABLE_CT_COMPENSATION	(0)
	#define IS_LCD_OFF_AFTER_REL_20SECS	(0)
	#define TENDER_MODE_NO_LOAD_TIME	(1)
	#if (defined(IRDA_TYPE_METER_AP_WARNGL) && (IRDA_TYPE_METER_AP_WARNGL==1))	
		#define IS_UP_DWN_DISPLAY	(0)
	#endif
	#if (defined(IRDA_TYPE_METER_AP_WARNGL) && (IRDA_TYPE_METER_AP_WARNGL==0))	
		#define IS_UP_DWN_DISPLAY	(1)
	#endif
	#endif
	//------------------------------------------------------------
#endif

#if (defined(BOARD_TARGET_MVVNL) && (BOARD_TARGET_MVVNL==1))
#define POWER_FAIL_PERSIT_TIME	(60)
#define IS_ADD_NM_CURRENTS		(0)
#define IS_PHASE_I_AVG			(0)
#define IS_NU_I_AVG				(0)
#define IS_EFF_I_AVG			(1)
#define IS_EXPORT_METER			(0)	
#define IS_MD_2DECIMAL			(1)
#define IS_MD_3DECIMAL			(0)	
#define IRDA_COM_ENABLE			(0)
#define UART_0_ENABLE			(1)
#define UART_2_ENABLE			(1)
#define OLD_AVG_U_AVG_C_LOGIC	(0)
#define IS_DISPLAY_CONFIGURABLE	(0)	
#define IS_DLMS_ENABLED_EEPL	(1)
#define EEPL_NUMBER_OF_SEASON	(2)
#define ISDO_HARMONICS_EEPL		(1)
#define APPLY_DC_IMMUNITY_ENABLE	(1)
#define ENABLE_CT_COMPENSATION	(1)
#define IS_LCD_OFF_AFTER_REL_20SECS	(1)
#define TENDER_MODE_NO_LOAD_TIME	(0)
#define IS_UP_DWN_DISPLAY	(0)

#endif
#if (defined(BOARD_TARGET_JABALPUR) && (BOARD_TARGET_JABALPUR==1))
#define POWER_FAIL_PERSIT_TIME	(300)
#define IS_ADD_NM_CURRENTS		(1)
#define IS_PHASE_I_AVG			(1)
#define IS_NU_I_AVG				(1)
#define IS_EFF_I_AVG			(0)
#define IS_EXPORT_METER			(0)	
#if (defined(BOARD_TARGET_INDORE) && (BOARD_TARGET_INDORE==1))
#define IS_MD_2DECIMAL			(1)
#define IS_MD_3DECIMAL			(0)	
#else
#define IS_MD_2DECIMAL			(0)
#define IS_MD_3DECIMAL			(1)	
#endif
#define IRDA_COM_ENABLE			(0)
#define UART_0_ENABLE			(1)
#define UART_2_ENABLE			(1)
#define UART2_TX_FUN_ENABLE		(0)
#define OLD_AVG_U_AVG_C_LOGIC	(0)
#define IS_DISPLAY_CONFIGURABLE	(0)	
#define IS_DLMS_ENABLED_EEPL	(1)
#define EEPL_NUMBER_OF_SEASON	(1)
#define ISDO_HARMONICS_EEPL		(0)
#define APPLY_DC_IMMUNITY_ENABLE	(0)
#define ENABLE_CT_COMPENSATION	(1)
#define IS_LCD_OFF_AFTER_REL_20SECS	(1)
#define TENDER_MODE_NO_LOAD_TIME	(0)
#define IS_UP_DWN_DISPLAY	(0)

#endif

#if (defined(GUJRAT_METER_0) && (GUJRAT_METER_0==1))

#define POWER_FAIL_PERSIT_TIME	(60)
#define IS_ADD_NM_CURRENTS		(0)	
#define IS_PHASE_I_AVG			(0)
#define IS_NU_I_AVG				(0)
#define IS_EFF_I_AVG			(0)
#define IS_EXPORT_METER			(1)	
#define IS_MD_2DECIMAL			(1)
#define IS_MD_3DECIMAL			(0)	
#define IRDA_COM_ENABLE			(0)
#define UART_0_ENABLE			(1)
#define UART_2_ENABLE			(1)
#define OLD_AVG_U_AVG_C_LOGIC	(0)
#define IS_DISPLAY_CONFIGURABLE	(0)	
#define IS_DLMS_ENABLED_EEPL	(1)
#define EEPL_NUMBER_OF_SEASON	(1)
#define ISDO_HARMONICS_EEPL		(1)
#define APPLY_DC_IMMUNITY_ENABLE	(1)
#define ENABLE_CT_COMPENSATION	(1)
#define IS_LCD_OFF_AFTER_REL_20SECS	(1)
#define TENDER_MODE_NO_LOAD_TIME	(0)

#endif

//pcb version
#define PCB_VER_4_6_2			(0)
#define PCB_VER_4_6_6			(0)
#define PCB_VER_4_6_8			(1)

#if (defined(IS_EFF_I_AVG) && (IS_EFF_I_AVG==1))
#if (defined(IS_PHASE_I_AVG) && (IS_PHASE_I_AVG==1))
	#error "Avg properly not defined. only one should be defined."
#endif
#endif

#endif
