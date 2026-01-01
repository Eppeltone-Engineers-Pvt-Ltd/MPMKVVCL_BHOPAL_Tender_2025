//-------- include device specific files----------------
#include "..\\..\\rlDevice\\Include\\dI2c.h"
#include "..\\..\\rlDevice\\Include\\dIOCtrl.h"
#include "..\\..\\rlDevice\\Include\\dRtc.h"

//-----------------------------------------------------

//-------- include app specific files-------------------
#include "..\\Include\\AppLcd.h"
#include "..\\Include\\AppVariables.h"
#include "..\\Include\\AppEeprom.h"
#include "..\\Include\\AppMacros.h"
#include "..\\Include\\AppScheduleAction.h"
#include "..\\Include\\AppMisc.h"
#include "..\\Include\\AppBilling.h"
#include "..\\Include\\AppMD.h"
#include "..\\Include\\AppBlockDailyLS.h"
#include "..\\Include\\AppTampers.h"
#include "..\\Include\\AppIntervalkWh.h"
#include "..\\Include\\AppConfig.h"
#include "..\\Include\\AppTOD.h"
#include "..\\Include\\AppOneSecondTask.h"



//----------------dlms specific files---------------------------------
#include "..\\..\\rlDlms\\meter_app\\r_dlms_data_meter.h"				/* DLMS Data Definitions */
void updatePfailEvent(void);

uint8_t isIntevalCrossed(uint32_t interval)
{
	uint8_t status=0;
	if((InsSave.timeCounter/interval)!=(prevTimeCounter /interval))
		status= 1;
		
	return status;
}
void OneSecondTask(void)
{
	uint8_t task=0;
	uint8_t task_SD=0;
	uint8_t clear_nm_clock=0;
	uint32_t nowkWh=InsSave.CumkWh+InsSave.ZkWhCounter/METER_CONSTANT;
	uint16_t timer5m;
	if(new_md_period==2)
	{
	timer5m=600;
	}
	else if(new_md_period==4)
	{
	timer5m=300;
	}
	InsSave.timeCounter++;
		
	isRAMOK();	
	if(mcu_flag&POWER_STABLE_FLAG)
	{
		InsSave.lsAvgV=InsSave.lsAvgV+Ins.Voltage;
		#if (defined(IS_EFF_I_AVG) && (IS_EFF_I_AVG == 1))
			InsSave.lsAvgPh_EffC=InsSave.lsAvgPh_EffC+Ins.EffectiveI;
		#endif
		#if (defined(IS_PHASE_I_AVG) && (IS_PHASE_I_AVG == 1))
			InsSave.lsAvgPh_EffC=InsSave.lsAvgPh_EffC+Ins.PhCurrent;
		#endif
		
		#if (defined(IS_NU_I_AVG) && (IS_NU_I_AVG == 1))
			InsSave.lsAvgNuC=InsSave.lsAvgNuC+Ins.NeuCurrent;
		#endif		
		InsSave.PowerOn30++;
	}
		
	if((((nowkWh-prekWh) >=NORMAL_KWH_THRESHOLD_LL)&&((nowkWh-prekWh) <=NORMAL_KWH_THRESHOLD_UL))||(((nowkWh-prekWhinterval) >=NORMAL_KWH_THRESHOLD_LL)&&((nowkWh-prekWhinterval) <=NORMAL_KWH_THRESHOLD_UL)))
	{
		savekWh(nowkWh);
		prekWh=nowkWh;
		prekWhinterval=nowkWh;
		
	}
	getMD_LSPulseCounter_SD();	
//	getMD_LSPulseCounter();
	if(((mcu_flag&POWER_STABLE_FLAG)==0)&&isPowerfailLog(USER_CODE))
		task|=RECORD_POWER_FAIL_DATA;
	
	if(isIntevalCrossed(300))
		getRtcCounter(RTC_NO_CHECK_MODE);
		
	//if(isIntevalCrossed(3600/md_period))
	//	task|=RECORD_MD_DATA;

	if((InsSave.timeCounter/timer5m)!=(prevTimeCounter /timer5m))  // Convert time
		task_SD|=RECORD_SD_MD_DATA;
		

	if(isIntevalCrossed(3600/ls_period))    
		task|=RECORD_LS_DATA ;
  
	if(isIntevalCrossed(86400UL))
	{
		if(getNextBillingDate()&&(InsSave.DailyDayCount>0||InsSave.IntCount>0))
		{
			task|=RECORD_BILL_DATA;
		}
		
		write_eeprom((uint8_t *)&clear_nm_clock,BAT_NM_LOCK_COUNT_LOC,1);
	}
	
	if(mcu_flag & SINGLE_ACTION_CHECK_FLAG )
	{
		task|=doBillSingleAction();
	}
		
	if(mcu_flag & PASSIVE_CAL_CHECK_FLAG )
	{
		task|=doCalanderAction();
	}		
		
	if(task)
	{
		mcu_flag &= ~COMM_RECEIVE_ENABLE;
		SaveEEPROM(PARA_WRITE_BEFORE_STATE ,PARA_TYPE_INSTANT); // save at before location when MD, LS or month over
	}
	
	if(task&RECORD_MD_DATA)
		Compute_MD(BILL_TYPE_AUTO);
	else if(task&RECORD_MAN_MD_DATA)
		Compute_MD(BILL_TYPE_MAN);
	else if	(task_SD&RECORD_SD_MD_DATA)
		Compute_MD(BILL_TYPE_AUTO);

	

	if(new_md_period!=md_period)
	{
		
		if((((InsSave.timeCounter%3600)%1800)/(3600/new_md_period))==(((InsSave.timeCounter%3600)%1800)/(3600/md_period)))
		{
			md_period=updateMDInterval(MD_PERIOD_CHANGE_FLAG,md_period);
		}
	}		
	
	if(task&RECORD_LS_DATA)
	{
		doLs();
		if((task&RECORD_BILL_DATA)==0)
		{
			if(isIntevalCrossed(86400UL))
			{
				if(getNextBillingDate()&&(InsSave.DailyDayCount>0||InsSave.IntCount>0))
				{
					task|=RECORD_BILL_DATA;
				}
			}		
		}
	}

	
	if(task&RECORD_BILL_DATA)
		CheckBillingOver(BILL_TYPE_AUTO,getNextBillingDate());
	
	if(task&RECORD_DO_BILL_DATA)
	{
		Compute_MD(BILL_TYPE_MAN);
		if(isIntevalCrossed(300))
			CheckBillingOver(BILL_TYPE_MAN,InsSave.timeCounter-InsSave.timeCounter%60);
		else
			CheckBillingOver(BILL_TYPE_MAN,InsSave.timeCounter);
	}

	if(task&RECORD_POWER_FAIL_DATA)
	{
		powerfailLogging();
		updatePfailEvent();
	}
	
	prevTimeCounter=InsSave.timeCounter;
	
	if(task)
	{
		R_OBIS_Class07_BlockloadUpdate();
		SaveEEPROM(PARA_WRITE_AFTER_STATE ,PARA_TYPE_PFAIL_INSTANT); // save at current location
		SaveEEPROM(PARA_WRITE_AFTER_STATE  ,PARA_TYPE_INSTANT); // save at after location
		
		mcu_flag |= COMM_RECEIVE_ENABLE;
		#if (defined(IS_DISPLAY_CONFIGURABLE) && (IS_DISPLAY_CONFIGURABLE == 1))
			validatelcdRAMBuff();
		#endif
	}
	
	
	Tamperlog();		
	
	
	//-------------------------------------
	CheckParaTimeOut();
	Display(USER_CODE);
	//-------------------------------------
	
	
	if((mcu_flag&POWER_STABLE_FLAG)==0)
	{
		if(powerStableCounter<5)
		{
			powerStableCounter++;
			if(powerStableCounter==5)
				SaveEEPROM(PARA_WRITE_BEFORE_STATE ,PARA_TYPE_INSTANT); // save at before location when MD, LS or month over
		}
		else
		{
			power_fail_state|=SAVE_LEVEL1_KWH_STATE_OPEN;			
			mcu_flag|=POWER_STABLE_FLAG;
			powerStableCounter=0;
			SWITCH_ON_RTC_VDD;
			SaveEEPROM(PARA_WRITE_AFTER_STATE  ,PARA_TYPE_INSTANT); // save at after location
			SaveEEPROM(PARA_WRITE_AFTER_STATE ,PARA_TYPE_PFAIL_INSTANT); // save at current location
			
		}
	}
	else 
	{
		if((power_fail_state&SAVE_LEVEL1_KWH_STATE_OPEN)==0)
		{
			if(powerStableCounter<10)
				powerStableCounter++;
			else
			{
				power_fail_state|=SAVE_LEVEL2_KWH_STATE_OPEN;
				powerStableCounter=0;
			}
		}
		
	}
		
	isRAMOK();	
	getSetPfailData(ENERGY_W_RAM);
}	


uint32_t Sliding_MD_kWh[3]  = {0};
uint32_t Sliding_MD_kVAh[3] = {0};
uint32_t Energy_kWh  = 0;
uint32_t Energy_kVAh = 0;

void getMD_LSPulseCounter_SD(void)
{
    uint16_t energykWh;
    uint16_t energykVAh;
    uint8_t  i;

	uint16_t timer5m;
	if(new_md_period==2)
	{
	timer5m=600;
	}
	else if(new_md_period==4)
	{
	timer5m=300;
	}
    /* Check 5-minute boundary */
    if ((InsSave.timeCounter / timer5m) !=(prevTimeCounter / timer5m))
    {
        /* Energy of last 5 minutes */
        energykWh  = InsSave.kWhCounter10m  / METER_CONSTANT;
        energykVAh = InsSave.kVAhCounter10m / METER_CONSTANT;

        /* Shift sliding window */
        for (i = 0; i < 3 - 1; i++)
        {
            Sliding_MD_kWh[i]  = Sliding_MD_kWh[i + 1];
            Sliding_MD_kVAh[i] = Sliding_MD_kVAh[i + 1];
        }
        Sliding_MD_kWh[3 - 1]  = energykWh;
        Sliding_MD_kVAh[3 - 1] = energykVAh;

        /* Calculate 15-minute energy */
        Energy_kWh  = 0;
        Energy_kVAh = 0;

        for (i = 0; i < 3; i++)
        {
            Energy_kWh  += Sliding_MD_kWh[i];
            Energy_kVAh += Sliding_MD_kVAh[i];
        }
		write_eeprom((uint8_t *)&Sliding_MD_kWh,SLIDING_MD_KW,sizeof(Sliding_MD_kWh));
		write_eeprom((uint8_t *)&Sliding_MD_kVAh,SLIDING_MD_KVA,sizeof(Sliding_MD_kVAh));
        /* Update MD and LS */
        InsSave.mdkWhPulseCounter  = Energy_kWh;
        InsSave.mdkVAhPulseCounter = Energy_kVAh;

        InsSave.lskWhPulseCounter  = Energy_kWh;
        InsSave.lskVAhPulseCounter = Energy_kVAh;

        /* Reset ONLY last 5-minute pulse count */
        InsSave.kWhCounter10m  = 0;
        InsSave.kVAhCounter10m = 0;
    }
}

uint16_t checkkWh2kVAh(uint16_t blkkwh,uint16_t blkkVAh)
{
	if(blkkwh>blkkVAh)
		blkkVAh=blkkwh;
	return blkkVAh;
}
