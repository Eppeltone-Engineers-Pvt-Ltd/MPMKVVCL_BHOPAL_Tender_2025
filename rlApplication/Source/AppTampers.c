//-------- include device specific files----------------
#include "..\\..\\rlDevice\\Include\\dI2c.h"
#include "..\\..\\rlDevice\\Include\\dIOCtrl.h"
#include "..\\..\\rlDevice\\Include\\dDeltaSigmaADC.h"
#include "..\\..\\rlDevice\\Include\\dMisc.h"
#include "..\\..\\rlDevice\\Include\\dWatchDog.h"
#include "..\\..\\rlDevice\\Include\\d12bitTimer.h"
#include "..\\..\\rlDevice\\DCImmunity\\dDCImmunity.h"
#include "..\\..\\rlDevice\\DCImmunity\\rl78_sw_dc_correction.h"

//-----------------------------------------------------

//-------- include app specific files-------------------
#include "..\\Include\\AppTampers.h"
#include "..\\Include\\AppVariables.h"
#include "..\\Include\\AppEeprom.h"
#include "..\\Include\\AppMisc.h"
#include "..\\Include\\AppConfig.h"
#include "..\\Include\\AppLcd.h"
#include "..\\Include\\AppCalibration.h"
#include "..\\Include\\AppMacros.h"

//----------------dlms specific files---------------------------------
#include "..\\..\\rlDlms\\meter_app\\r_dlms_data_meter.h"				/* DLMS Data Definitions */

//#if (defined(BOARD_TARGET_JABALPUR) && (BOARD_TARGET_JABALPUR == 1))
//#define MAG_SYMBOL_SHOW_TIME	7
//#else
#define MAG_SYMBOL_SHOW_TIME	15
//#endif

#if (defined(IS_DC_MAG_ENABLE) && (IS_DC_MAG_ENABLE == 1))
#define PERMANENT_MAG_EXIT_TIME	30
#endif



//--------------Event Code for DLMS-----------------
const uint8_t dlmsEventCode[MAX_TAMPER_COUNT][2]={
{207	,208},		// NM
{67		,68},		// OL
{201	,202},		// MAG
{51		,52},		// REV
{203	,204},		// ND
{69		,70},		// EARTH
{7		,8},		// OU
{251	,0xFF},		// C-oPEN
{9		,10},		// Low U
};

#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 0))
//--------------------------------------------------
/*********************** Check tamper and display *****************************/
//0-Current Related,1-Other,2-Non Rollover
const uint8_t EventCountIndex[MAX_TAMPER_COUNT]={
1,
0,
1,
0,
1,
0,
3,
2,
3
};

const uint8_t EventLen[TAMPER_GROUP_COUNT]={
  100,								// 0- current Related
  129,								// Other
	1,								// NOn Rollover
	1,
};

const uint16_t startAdd[TAMPER_GROUP_COUNT]={
  TAMPER_C_RELATED_LOC,
  TAMPER_OTHER_LOC,
  TAMPER_NON_ROLLOVER_LOC,
  TAMPER_U_RELATED_LOC,
};
#endif

#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 1))
//--------------------------------------------------
/*********************** Check tamper and display *****************************/
//0-Current Related,1-Other,2-Non Rollover
const uint8_t EventCountIndex[MAX_TAMPER_COUNT]={
0,
1,
2,
3,
4,
5,
6,
7,
8
};

const uint8_t EventLen[MAX_TAMPER_COUNT]={
  30,								
  30,								
  30,								
  30,
  30,
  30,
  30,
  30,
  1
};

const uint16_t startAdd[MAX_TAMPER_COUNT]={
  TAMPER_NM_LOC,
  TAMPER_OL_LOC,
  TAMPER_MAG_LOC,
  TAMPER_REV_LOC,
  TAMPER_ND_LOC,
  TAMPER_EARTH_LOC,
  TAMPER_OU_LOC,
  TAMPER_CO_LOC,
  TAMPER_LU_LOC,
};
#endif

uint8_t Check_Tamper(void)
{   
	
	uint8_t status=0;
	uint8_t checkOffset=0;
   
	display_flag=0;
	CurrentTamperStatusFlag=0;
		

	
	if((TamperRecord.TamperStatusFlag & TAMPER_COVEROPEN)==0)// if cover open is not there
	{
		while(GET_COVER_STATUS)	// check cover open pin 
		{
			if(TamperTime_counter[7]++>=COVER_OPEN_WAIT_TIME)
			{
				CurrentTamperStatusFlag |= TAMPER_COVEROPEN;
				break;
			}
		}		
	}
	else
		CurrentTamperStatusFlag |= TAMPER_COVEROPEN;
	
	if(Ins.Voltage>COM_THRESHOLD_VOLTAGE)
	{
		if(GET_COM_STATUS==0)
			SWITCH_ON_COMM_VCC;
	}
	else
	{
		if(GET_COM_STATUS)
			SWITCH_OFF_COMM_VCC;
	}
	
	#if (defined(POWER_BASE_EARTH_DETECT) && (POWER_BASE_EARTH_DETECT == 1))	// EARTH TAMPER
		if(Ins.EffectiveP  > MIN_TAMPER_DETECTION_POWER) // MIN DETECTION POWER IS 100 WATTS  
		{  						
			if(Ins.NeuPower > Ins.PhPower)            
				itemp=Ins.PhPower;  
			else
				itemp=Ins.NeuPower; 

			if((((Ins.EffectiveP -itemp )*100)/Ins.EffectiveP) >= EARTH_TAMPER_THRESHOLD)// 5%
			{
				CurrentTamperStatusFlag |= TAMPER_EARTH;
				display_flag|=DISP_EARTH;
			}
			else
			{
				if((CurrentTamperStatusFlag&TAMPER_EARTH)==0)
					if((mcu_flag&POWER_STABLE_FLAG)&&(Ins.Voltage<FORCE_POWER_FAIL_THRESHOLD))
						power_fail_state|=FORCE_POWER_FAIL;
			}
			
		}			
	#else // this one is used
		CurrentTamperStatusFlag|=getEarthStatus(EARTH_TAMPER_THRESHOLD);
		if(CurrentTamperStatusFlag&TAMPER_EARTH)
			display_flag|=DISP_EARTH;
		else
		{
			if((CurrentTamperStatusFlag&TAMPER_EARTH)==0)
				if((mcu_flag&POWER_STABLE_FLAG)&&(Ins.Voltage<FORCE_POWER_FAIL_THRESHOLD))
					power_fail_state|=FORCE_POWER_FAIL;
		}
		
	#endif	

	//reverse tamper
	if(Ins.PhPower>MIN_TAMPER_DETECTION_POWER)
	{
		if(mcu_flag&PH_POWER_REV)
		{
			if((Ins.PhPower==Ins.EffectiveP)||(Ins.PhPower>10000))
			{
				#if (defined(GUJRAT_METER_0) && (GUJRAT_METER_0 == 0))
					CurrentTamperStatusFlag |= TAMPER_REV;
				#endif
				display_flag|=DISP_REV;
			}
			else
				mcu_flag&=~PH_POWER_REV;
		}
	}
	else
		mcu_flag&=~PH_POWER_REV;
	
		
	if(Ins.NeuPower>MIN_TAMPER_DETECTION_POWER)
	{
		if(mcu_flag&NU_POWER_REV)
		{
			if((Ins.NeuPower==Ins.EffectiveP)||(Ins.NeuPower>10000))
			{
				#if (defined(GUJRAT_METER_0) && (GUJRAT_METER_0 == 0))
					CurrentTamperStatusFlag |= TAMPER_REV;
				#endif
				display_flag|=DISP_REV;
			}
			else
				mcu_flag&=~NU_POWER_REV;
		}
	}
	else
		mcu_flag&=~NU_POWER_REV;

	if(CurrentTamperStatusFlag & TAMPER_EARTH)
	{
		if(Ins.Voltage<NM_THRESHOLD_VOLTAGE)
		{
			if(((10UL*peakVoltage)>(178UL*Ins.Voltage/100))&&(Ins.Voltage>100))
				CurrentTamperStatusFlag |= TAMPER_AC_DC;
			else
			{
				CurrentTamperStatusFlag |=TAMPER_NEU_MISS;
				display_flag&=~DISP_EARTH;
			}
		}
		else
		{
			#if (defined(METER_ENABLE_DC_IMUNITY_DETECTION_ON_SAMPLE) && (METER_ENABLE_DC_IMUNITY_DETECTION_ON_SAMPLE == 1))
				if(g_dc_immunity_state==0)
				{
					if(((Ins.Voltage>23500)&&(Ins.PowerFactor<95)&&(Ins.LeadLag==1))&&((display_flag & DISP_REV)==0))
					{
						if(((1000UL*peakVoltage)/Ins.Voltage)>143)
						{
							checkOffset=1;
						}
						
					}
				}
			#endif
			if(Ins.Frequency<4700||Ins.Frequency>5300)
			{
				CurrentTamperStatusFlag |= TAMPER_AC_DC;
			}
			else if(((Ins.Voltage<23600)||(checkOffset==1))&&((display_flag & DISP_REV)==0))
			{
				#if (defined(METER_ENABLE_DC_IMUNITY_DETECTION_ON_SAMPLE) && (METER_ENABLE_DC_IMUNITY_DETECTION_ON_SAMPLE == 1))
					if(g_dc_immunity_state==0)
					{
						#if (defined(RCD_TAMPER_TERMINAL3) && (RCD_TAMPER_TERMINAL3 == 1))
							if(Ins.PhCurrent<15000)
							{
								if((Ins.PhCurrent>=950)&&(Ins.NeuCurrent>(3+rlCalib.I2OffSet))&&((Ins.NeuCurrent<=50)&&(Ins.NeuPower<MIN_TAMPER_DETECTION_POWER)))
								{
									CurrentTamperStatusFlag |= TAMPER_AC_DC;		
								}
							}
						#endif
						#if (defined(RCD_TAMPER_TERMINAL4) && (RCD_TAMPER_TERMINAL4 == 1))
						{
							if((Ins.PhPower<MIN_TAMPER_DETECTION_POWER)&&((Ins.PowerFactor>55)||(checkOffset==1)))
							{
								
								if((Ins.NeuCurrent>=950)&&(Ins.PhCurrent<50)&&(Ins.NeuCurrent<15000))
								{
									if((Ins.Voltage>14200 && Ins.Voltage<14600) ||(Ins.Voltage>=15500))
									{
										if(Ins.PhCurrent>(5+rlCalib.I1OffSet))
											CurrentTamperStatusFlag |= TAMPER_AC_DC;
									}
									else
									{
										if(Ins.PhCurrent>=(3+rlCalib.I1OffSet))
											CurrentTamperStatusFlag |= TAMPER_AC_DC;
										
									}										
								}
							}
									
						}
						#endif
					}
				#endif
				
			}
		}
	}

	
	if((mcu_flag&POWER_STABLE_FLAG)==0)
	{
		permanent_mag_wait_time=0;
		mag_hold_time=0;
	}
	else
	{
		if((Ins.Voltage>12000)&&(mcu_flag&PROCESS_MAG_FLAG))
		{
			mag_hold_time++;
		}
		else
		{
			mag_hold_time=0;
			cum_mag_toggle_count=0;
			mcu_flag&=~SHOW_MAG_SYMBOL;
		}
		
		#if (defined(IS_DC_MAG_ENABLE) && (IS_DC_MAG_ENABLE == 1))
			if((Ins.Voltage>12000)&&(GET_MAG_STATUS))	
			{
				permanent_mag_wait_time++;
				mcu_flag|=SHOW_MAG_SYMBOL;
			}
			else
				permanent_mag_wait_time=0;
		#else
			permanent_mag_wait_time=0;
		#endif
		
		if(((TamperRecord.TamperStatusFlag&TAMPER_MAG)&&(IS_PRODUCTION_MODE==0)))
		{
			permanent_mag_wait_time=PERMANENT_MAG_HOLD_TIME+1;
			mcu_flag|=SHOW_MAG_SYMBOL;
		}
		
		if((mag_hold_time>0)||(permanent_mag_wait_time>0))
		{
			mcu_flag&=~(EXPORT_MODE|PH_POWER_REV|NU_POWER_REV);
			
			CurrentTamperStatusFlag&=~(TAMPER_EARTH|TAMPER_REV);
			CurrentTamperStatusFlag |=TamperRecord.TamperStatusFlag;
			CurrentTamperStatusFlag&=~(TAMPER_NEU_MISS|TAMPER_AC_DC|TAMPER_MAG);
			
			display_flag&=~(DISP_EARTH|DISP_REV|DISP_NM);
			if((mag_hold_time>MAG_SYMBOL_SHOW_TIME)||(permanent_mag_wait_time>MAG_SYMBOL_SHOW_TIME))
				if(mcu_flag&SHOW_MAG_SYMBOL)
					display_flag|=DISP_MAG;
		}
			
		if(((mag_hold_time>MAG_HOLD_TIME)&&(cum_mag_toggle_count>3))||((permanent_mag_wait_time>PERMANENT_MAG_HOLD_TIME)))
		{
				
				if(cum_mag_toggle_count>3)
				{
					CurrentTamperStatusFlag|=TAMPER_MAG;
					if(TamperTime_counter[2]<(MAG_HOLD_TIME*2))
						TamperTime_counter[2]=(MAG_HOLD_TIME*2)+1;
						
					Ins.PhCurrent=IMAX_CURRENT;
					Ins.NeuCurrent=IMAX_CURRENT;
					Ins.EffectiveI=IMAX_CURRENT;
					Ins.PhPower=IMAX_POWER;
					Ins.NeuPower=IMAX_POWER;
					Ins.PowerFactor=100;
					Ins.Voltage=24000;
					Ins.EffectiveP=IMAX_POWER;
					Ins.AppPower=IMAX_POWER;
				}
				cum_mag_toggle_count=4;
				status=1;
				apparentPulsePower=IMAX_POWER;
				activePulsePower=apparentPulsePower;
				
		}
		else
		{
			if(((mag_hold_time==0))&&((CurrentTamperStatusFlag&TAMPER_NEU_MISS)||(CurrentTamperStatusFlag & TAMPER_AC_DC)))
			{
				CurrentTamperStatusFlag&=~TAMPER_EARTH;
				CurrentTamperStatusFlag |=TamperRecord.TamperStatusFlag;
				//if(Ins.Voltage>REF_U)
				//	apparentPulsePower=(uint32_t)Ins.Voltage*Ins.EffectiveI/1000;
				//else
				apparentPulsePower=REFERENCEVOLTAGE*Ins.EffectiveI;
					
				activePulsePower=apparentPulsePower;
				display_flag|=DISP_NM;
				status=1;
			}
			else if(Ins.EffectiveI<EARTH_TAMPER_I_THRESHOLD)
				CurrentTamperStatusFlag |=(TamperRecord.TamperStatusFlag&(TAMPER_EARTH|TAMPER_REV|TAMPER_AC_DC));

		}
	}
	
	#if (defined(GUJRAT_METER_0) && (GUJRAT_METER_0 == 1))
	{
		if(Ins.EffectiveP>STARTING_POWER_THRESHOLD_L)
		{
			if(((display_flag&DISP_REV)&&((mcu_flag&EXPORT_MODE)==0))||(((display_flag&DISP_REV)==0)&&(mcu_flag&EXPORT_MODE)))
			{
				ClearPulseAccumulation();
				if(display_flag&DISP_REV)
					mcu_flag|=EXPORT_MODE;
				else
					mcu_flag&=~EXPORT_MODE;
			}	
		}
		
	}
	#endif
	
	if((mcu_flag&POWER_STABLE_FLAG)==0)
		display_flag=0x00;
	
	return status;
	
}
/*--------------------------------------------------------------------------*/

void Tamperlog(void)
{   
	uint8_t i,save_flag=0;
	uint16_t TamperType=1;
#if (defined(BOARD_TARGET_JABALPUR) && (BOARD_TARGET_JABALPUR == 1))		
uint16_t TAMPER_EVENT_OCC_TIME[MAX_TAMPER_COUNT]={300,300,30,300,300,300,300,60,300};
uint16_t TAMPER_EVENT_REC_TIME[MAX_TAMPER_COUNT]={300,300,30,300,300,300,300,60,300};
#endif

#if (defined(BOARD_TARGET_MVVNL) && (BOARD_TARGET_MVVNL == 1))	
	uint16_t TAMPER_EVENT_OCC_TIME[MAX_TAMPER_COUNT]={60,60,60,60,60,60,60,60,60};
	uint16_t TAMPER_EVENT_REC_TIME[MAX_TAMPER_COUNT]={60,60,56,60,60,60,60,60,60};	
#endif
#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 1)&& (IRDA_TYPE_METER_AP == 0))	
	uint16_t TAMPER_EVENT_OCC_TIME[MAX_TAMPER_COUNT]={60,60,180,60,60,60,60,60,60};
	uint16_t TAMPER_EVENT_REC_TIME[MAX_TAMPER_COUNT]={60,60,26,60,60,60,60,60,60};	
#endif
#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 1)&& (IRDA_TYPE_METER_AP == 1))	
	uint16_t TAMPER_EVENT_OCC_TIME[MAX_TAMPER_COUNT]={60,60,60,60,60,60,60,60,60};
	uint16_t TAMPER_EVENT_REC_TIME[MAX_TAMPER_COUNT]={60,60,56,60,60,60,60,60,60};	
#endif

	uint16_t loc=0;
	uint8_t eventtype;
     
	for(i=0;i<MAX_TAMPER_COUNT;i++)
	{   

		if(((TamperRecord.TamperStatusFlag & TamperType) ^ (CurrentTamperStatusFlag & TamperType))>0)
		{   

			TamperTime_counter[i]++;

			eventtype = (TamperRecord.TamperStatusFlag ^ TamperType) & TamperType ?0:1;  // occurence or recovery

			//-------------------------------------------------------------

			if(((TamperTime_counter[i] >= TAMPER_EVENT_OCC_TIME[i])&&(eventtype==0))||((TamperTime_counter[i] >= TAMPER_EVENT_REC_TIME[i])&&(eventtype==1)))
			{ 
				if(TamperType==TAMPER_COVEROPEN)
					setParaBuff(DISPLAY_MODE_AUTO_COVER);
					
				SaveEEPROM(PARA_WRITE_BEFORE_STATE,PARA_TYPE_TAMPER );
				save_flag=1;
				mcu_flag&= ~COMM_RECEIVE_ENABLE; 
				TamperRecord.TamperStatusFlag ^= TamperType;

				makeByte(InsSave.timeCounter,0,4);		// 4 bytes
				makeByte(dlmsEventCode[i][eventtype],4,2); 					  	// 2  byte
				#if (defined(BOARD_TARGET_JABALPUR) && (BOARD_TARGET_JABALPUR == 1))		
				makeByte(Ins.PhCurrent,6,4);									// 4 bytes
				#else
				makeByte(Ins.EffectiveI,6,4);
				#endif
				makeByte(Ins.NeuCurrent,10,4);									// 4 bytes


				makeByte(Ins.Voltage,14,2);										// 2 bytes
				if((Ins.LeadLag==1)&&((display_flag&DISP_NM)==0)&&(Ins.PowerFactor!=100))
					makeByte(0x80|Ins.PowerFactor ,16,1);  								// 1 bytes
				else
					makeByte(Ins.PowerFactor ,16,1);  								// 1 bytes
					
				makeByte(InsSave.CumkWh+InsSave.ZkWhCounter/METER_CONSTANT,17,4);           // 4 bytes
				#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))
				makeByte(InsSave.ExpCumkWh+InsSave.ExpZkWhCounter/METER_CONSTANT,21,4);           // 4 bytes
				#else
				makeByte(Ins.EffectiveI,21,4);
				#endif
				TamperTime_counter[i] = 0;

				if(TamperRecord.eventcounts[EventCountIndex[i]]>=EventLen[EventCountIndex[i]])
					TamperRecord.eventcounts[EventCountIndex[i]]=0x00;

				loc=(startAdd[EventCountIndex[i]]+TamperRecord.eventcounts[EventCountIndex[i]]*TAMPER_DATA_LENGTH);	
				write_eeprom(RxTxBuffer,loc,TAMPER_DATA_LENGTH);
				#if (defined(IS_DLMS_ENABLED_EEPL) && (IS_DLMS_ENABLED_EEPL == 1))
					TamperRecord.lastEventCode[EventCountIndex[i]]=loc;
				#endif
				TamperRecord.eventcounts[EventCountIndex[i]]++;

				mcu_flag|= COMM_RECEIVE_ENABLE; 
				if(TamperRecord.TamperStatusFlag & TamperType)   // if occurance
				{   
					TamperRecord.TamperCount[i]=TamperRecord.TamperCount[i]+1;
					TamperRecord.lastEventAddr[i]=loc;
					TamperRecord.lastOccRes[0]=loc;
				}
				else
					TamperRecord.lastOccRes[1]=loc;
			}
		}
		else
		{
			TamperTime_counter[i] = 0;
		}
		TamperType<<=1;
	}

	if(save_flag==1)
	{
		R_OBIS_Class07_EventUpdate();
		SaveEEPROM(PARA_WRITE_AFTER_STATE ,PARA_TYPE_TAMPER );
		
	}
			
}
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/

uint16_t getTotalTamperCount(void)
{
	uint8_t i;
	uint16_t tcount=0;
	for(i=0;i<MAX_TAMPER_COUNT;i++)
	{
		#if (defined(IS_DLMS_ENABLED_EEPL) && (IS_DLMS_ENABLED_EEPL == 1))
		if(i!=7)
		#endif
		tcount=tcount+TamperRecord.TamperCount[i];
		
	}
			
	return tcount;  
}


	
void getEventLog(uint8_t*ptr, uint8_t tampertype,uint8_t eventno)
{
	uint16_t addr; 
	uint16_t loggedEvents=getCurrentLogCount(tampertype);
	
	if(loggedEvents>=EventLen[tampertype])
		addr=TamperRecord.eventcounts[tampertype]%EventLen[tampertype];
	else
		addr=0;

	

	
	addr=addr+eventno;
		
	addr=addr%EventLen[tampertype];
	
	addr=startAdd[tampertype]+addr*TAMPER_DATA_LENGTH;
	
	
	read_eeprom(ptr,addr,TAMPER_DATA_LENGTH);
	
	
}

uint8_t getCurrentLogCount(uint8_t tamperid)
{
	uint16_t tamper_count[4];
	tamper_count[0]=TamperRecord.TamperCount[3]+TamperRecord.TamperCount[5];
	tamper_count[1]=TamperRecord.TamperCount[0]+TamperRecord.TamperCount[2]+TamperRecord.TamperCount[4];
	tamper_count[2]=TamperRecord.TamperCount[7];
	tamper_count[3]=TamperRecord.TamperCount[6]+TamperRecord.TamperCount[8];	
	
	if(TamperRecord.eventcounts[tamperid]<tamper_count[tamperid])
		return EventLen[tamperid];
	else
		return TamperRecord.eventcounts[tamperid];

}


uint8_t getEventMaxEntries(uint8_t tamperid)
{
	uint8_t maxEntry=0;
	switch(tamperid)
	{
		case 0:
			maxEntry=EventLen[0];
			break;
		case 1:
			maxEntry=PFAIL_EVENTS*2;
			break;
		case 2:
			maxEntry=SW_EVENTS;
			break;
		case 3:
			maxEntry=EventLen[1];
			break;
		case 4:
			maxEntry=EventLen[2];
			break;
		case 5:
			maxEntry=EventLen[3];
			break;			
	}
	return maxEntry;
}



void logSWTamper(uint8_t eventid)
{
	uint8_t logBuffer[6];
	SaveEEPROM(PARA_WRITE_BEFORE_STATE,PARA_TYPE_TAMPER );
	
	logBuffer[0]=InsSave.timeCounter;
	logBuffer[1]=InsSave.timeCounter>>8;
	logBuffer[2]=InsSave.timeCounter>>16;
	logBuffer[3]=InsSave.timeCounter>>24;
	logBuffer[4]=eventid;
	
	write_eeprom((uint8_t *)&logBuffer,SOFT_CHANGE_LOG_LOC+(TamperRecord.sw_log_count%SW_EVENTS)*5,5);
	
	TamperRecord.sw_log_count++;
	R_OBIS_Class07_EventUpdate();
	SaveEEPROM(PARA_WRITE_AFTER_STATE ,PARA_TYPE_TAMPER );  
	
}

void getPfailLog(uint8_t*ptr,uint8_t eventno)
{
	
	uint16_t addr; 
	
	if(InsSave.PFailCount>PFAIL_EVENTS)
		addr=InsSave.PFailCount%PFAIL_EVENTS;
	else
		addr=0;


	addr=addr+(eventno/2);

	addr=addr%PFAIL_EVENTS;

	addr=POWER_FAIL_LOC+addr*8; 
	
	
	read_eeprom(ptr,addr+(eventno%2)*4,4);
	
	*(ptr+4)=0x65+eventno%2;
	*(ptr+5)=0x00;;
		
}



void getSWLog(uint8_t*ptr,uint8_t eventno)
{
	
	uint16_t addr; 
  

	if(TamperRecord.sw_log_count>SW_EVENTS)
		addr=TamperRecord.sw_log_count%SW_EVENTS;
	else
		addr=0;

	addr=addr+eventno;

	addr=addr%SW_EVENTS;
	addr=SOFT_CHANGE_LOG_LOC+addr*5; 

	read_eeprom(ptr,addr,5);

	//*(ptr+4)=((*ptr+4))&0xFF);
	*(ptr+5)=0x00;;
	
}

void ClearCopen(void)
{

	#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 0))
		fillComBufferZero();
		
	
		write_eeprom(RxTxBuffer,TAMPER_NON_ROLLOVER_LOC,TAMPER_DATA_LENGTH);
		if(TamperRecord.lastOccRes[1]==TAMPER_NON_ROLLOVER_LOC)
			TamperRecord.lastOccRes[1]=0;
		if(TamperRecord.lastOccRes[0]==TAMPER_NON_ROLLOVER_LOC)
			TamperRecord.lastOccRes[0]=0;

		TamperRecord.TamperStatusFlag &= ~TAMPER_COVEROPEN;
		TamperRecord.TamperCount[7]=0x00;
		if(TamperRecord.eventcounts[EventCountIndex[7]]>0)
			TamperRecord.eventcounts[EventCountIndex[7]]--;
		
			
		R_OBIS_Class07_EventUpdate();
		
		SaveEEPROM(PARA_WRITE_AFTER_STATE ,PARA_TYPE_TAMPER );
		setParaBuff(DISPLAY_MODE_NORMAL); // restore orignal display sheet
	#endif
	
	#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 1))
		
		fillComBufferZero();
		
		write_eeprom(RxTxBuffer,TAMPER_CO_LOC,TAMPER_DATA_LENGTH);

		TamperRecord.TamperStatusFlag &= ~TAMPER_COVEROPEN;
		TamperRecord.TamperCount[7]=0x00;
		TamperRecord.eventcounts[EventCountIndex[7]]=0;
		
		SaveEEPROM(PARA_WRITE_AFTER_STATE ,PARA_TYPE_TAMPER );
		setParaBuff(DISPLAY_MODE_NORMAL); // restore orignal display sheet
	
	#endif
	
}

uint8_t getlastEventID(uint8_t eventtype)
{
	
	if(TamperRecord.lastOccRes[eventtype]>0)
		read_eeprom((uint8_t*)&eventtype,TamperRecord.lastOccRes[eventtype]+4,2);
	else
		eventtype=0;
		
	return eventtype;
}


uint16_t getEarthStatus(uint8_t threshold)
{
	uint16_t status=0;
	uint32_t GCurrent1=0;
	uint32_t GCurrent2=0;
	
	if(Ins.NeuCurrent  > Ins.PhCurrent)          // Earth Tamper Detection
	{
		GCurrent1=Ins.NeuCurrent;
		GCurrent2=Ins.PhCurrent; 
		
	}
	else
	{
		GCurrent1=Ins.PhCurrent; 
		GCurrent2=Ins.NeuCurrent;
	}
	
	if(Ins.EffectiveI >=EARTH_TAMPER_I_THRESHOLD)   // 50 ma 
		if(GCurrent1 >=EARTH_TAMPER_I_THRESHOLD)   // 50 ma 
			if((((GCurrent1 -GCurrent2 )*100)/GCurrent1) >= threshold)
				status= TAMPER_EARTH;
	
	return status;
	
}
#if (defined(IRDA_TYPE_METER_HP) && (IRDA_TYPE_METER_HP == 1))
uint16_t GetTamperForwardLoc(uint8_t eventno,uint8_t tamperid)
{   
  int16_t addr; 
  
  addr=(TamperRecord.eventcounts[EventCountIndex[tamperid]]%EventLen[EventCountIndex[tamperid]])-eventno % (EventLen[EventCountIndex[tamperid]]);
  if(addr<=0)
    addr=EventLen[EventCountIndex[tamperid]]+addr;
  
  addr=addr-1;
  addr=startAdd[EventCountIndex[tamperid]]+addr*TAMPER_DATA_LENGTH;
  return addr; 
}
#endif

