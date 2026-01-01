#ifndef _APP_EEPROM_H
#define _APP_EEPROM_H

#include "AppConfig.h"
	 
#define EEP_PAGE_LENGTH         64

#define CAL_CONST				0


#define PRE_BILL_SINGLE_ACTION	54		// 4 single action date,1- execution times
#define PASSIVE_SEA_ACT_TIME	59		// 4 bytes activation date,1- activation date set indication 0xAB
#define PASSIVE_ZONE_TIME       64     	//(((2*8=16) bytes)=16) bytes  
#define PASSIVE_ZONE_NO         80     	// 1 byte Max Zone No.+8 zones's no= 9 bytes
#define PASSIVE_ZONE_TIME2      89     	//(((2*8=16) bytes)=16) bytes  
#define PASSIVE_ZONE_NO2        105     // 1 byte Max Zone No.+8 zones's no= 9 bytes
#define PASSIVE_ZONE_SEASON     114     // 2 (4 bytes)season+ 2 season no=7 bytes  


#define DLMS_LLS_SECRET_ADDR 	140    //18=(16 byte+2 bytes crc )
#define DLMS_HLS_SECRET_ADDR	158    //18=(16 byte+ 2 bytes crc)
#define TOD_CALANDER_NAME		176		// 64 (Calander Name+ Season Name)
#define TOD_PCALANDER_NAME		240		// 64 (Calander Name+ Season Name)

#define BAT_NM_LOCK_COUNT_LOC	304
#define KNO_LOC					384
#define AUTO_MODE_DP			576
#define PUSH_MODE_DP			704

#define SERIAL_NO_LOC           960     //20 bytes = 16 bytes serial number including :+ 2 bytes mfg month, year
#define MD_IP_LOC               1076     //2 bytes
#define TAMPER_LOG_TIME_LOC     1078     //16 bytes
#define TAMPER_REC_TIME_LOC     1094     //16 bytes
#define LS_IP                 	1110     //1 bytes
#define ZONE_TIME               1111     //(((2*8=16) bytes)=16) bytes  
#define ZONE_NO                 1127     // 1 byte Max Zone No.+8 zones's no= 9 bytes
#define ZONE_TIME2              1136     //(((2*8=16) bytes)=16) bytes  
#define ZONE_NO2                1152     // 1 byte Max Zone No.+8 zones's no= 9 bytes
#define ZONE_SEASON             1161     // 2 (4 bytes)season+ 2 season no=7 bytes  
#define PRE_BILL_DATE           1168     //2 bytes =1 byte for day + 1byte billing types-0- Each,1 -Odd,2-Even 

#define CAL_CONST_2				1216
#define PROD_STATUS_LOC			1246
#define INTERVAL_KWH_LOC       	1256


#define INS_SAVE_BEFORE_LOC     2048    //256 bytes
#define TAMPER_SAVE_BEFORE_LOC  2304    //60 bytes


#define INS_SAVE_CURRENT_LOC    2368    //24 bytes

#define INS_SAVE_AFTER_LOC      2432    //256 bytes
#define TAMPER_SAVE_AFTER_LOC   2688    //60 bytes


#define BILL_DATA_LOC           2752   	//42*12=512)

#define TOD_BILL_LOC            3264   	//(161*12=1932) bytes 

#define PFAIL_INS_SAVE_LOC      5248   // 64 max bytes

#define SLIDING_MD_KW           5350     // 4*3 byte
#define SLIDING_MD_KVA          5370     // 4*3 byte

#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 0))

#define TAMPER_C_RELATED_LOC    6464   	//(100*25=2500  ~=2560)
#define TAMPER_OTHER_LOC		9024    //(129*25=3225  ~=3264)
#define TAMPER_NON_ROLLOVER_LOC 12288   //(1*25=  ~=64)
#define TAMPER_U_RELATED_LOC	12352
#endif
#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 1))
#define TAMPER_EARTH_LOC		5312
#define TAMPER_CO_LOC			6080
#define TAMPER_LU_LOC			6848
#define TAMPER_OL_LOC			7616
#define TAMPER_OU_LOC			8384
#define TAMPER_NM_LOC			9152	
#define TAMPER_MAG_LOC			9920
#define TAMPER_REV_LOC			10688
#define TAMPER_ND_LOC			11456

#endif


#define NM_INFO_SHT_LOC			12416
#define NM_INFO_CT_LOC			12420
#define NM_INFO_FALSE_LOC		12424
#define IS_MEMORY_CLEARED		12428
#define UPDATE_MD_INTERVAL_LOC	12432	// 4 bytes (2 bytes md + 2 bytes CRC)
#define RTC_BAT_STATUS_LOC		12440

#define SOFT_CHANGE_LOG_LOC     12736   //(5*20=100)	bytes
#define DAILY_KWH_LOC           12864   //(12*90=1088)bytes
#define POWER_FAIL_LOC          13952   //(100*8)=832
#define LOAD_SURVEY_LOC         14784   //(292*60) = 17520; (340*70==27160)

#define METER_KIT_ID            42049     //16 Bytes required
#define EEP_START_LOC           0	//INTERVAL_KWH_LOC

#if (defined(BOARD_TARGET_JABALPUR) && (BOARD_TARGET_JABALPUR == 1))
#define EEP_END_LOC             41984
#else
#define EEP_END_LOC             32448
#endif

 	

#endif
