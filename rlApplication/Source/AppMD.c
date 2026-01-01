//-------- include device specific files----------------
#include "..\\..\\rlDevice\\Include\\dI2c.h"
//-----------------------------------------------------

//-------- include app specific files-------------------
#include "..\\Include\\AppMD.h"
#include "..\\Include\\AppVariables.h"
#include "..\\Include\\AppEeprom.h"
#include "..\\Include\\AppLcd.h"
#include "..\\Include\\AppMisc.h"
#include "..\\Include\\AppTOD.h"
#include "..\\Include\\AppConfig.h"
#include "..\\Include\\AppOneSecondTask.h"


/****************** Maximum Demand ********************************************/
void Compute_MD(uint8_t reset_option)
{   
		

		uint16_t ZenergykWh=InsSave.ZkWhCounter;
		uint16_t ZenergykVAh=InsSave.ZkVAhCounter;
		uint32_t logtimeCounter;
		uint8_t sd_period;
		#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))		
		uint16_t ZenergyExpkWh=InsSave.ExpZkWhCounter;
		uint16_t ZenergyExpkVAh=InsSave.ExpZkVAhCounter;
		#endif		
		if(new_md_period==4)
		{
			sd_period=12;
		}
		else if(new_md_period==2)
		{
			sd_period=6;
		}
		logtimeCounter=prevTimeCounter-(prevTimeCounter%(3600/sd_period));
		//uint8_t preSeason=0;
		
		logtimeCounter=logtimeCounter+(3600/sd_period);
			
		
		ZenergykWh=ZenergykWh/METER_CONSTANT;
		ZenergykVAh=ZenergykVAh/METER_CONSTANT;
		#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))		
		ZenergyExpkWh=ZenergyExpkWh/METER_CONSTANT;
		ZenergyExpkVAh=ZenergyExpkVAh/METER_CONSTANT;
		
		#endif		
		
		
		if(ZenergykWh>ZenergykVAh)
		{
			ZenergykVAh=ZenergykWh;
			InsSave.ZkVAhCounter=InsSave.ZkWhCounter;
		}
		#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))		
		if(ZenergyExpkWh>ZenergyExpkVAh)
		{
			ZenergyExpkVAh=ZenergyExpkWh;
			InsSave.ExpZkVAhCounter=InsSave.ExpZkWhCounter;
		}		
		#endif		
		
		InsSave.CumkWh+=ZenergykWh;
		
		#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))		
		InsSave.ExpCumkWh+=ZenergyExpkWh;
		#endif		
		
		#if (defined(GUJRAT_METER_0) && (GUJRAT_METER_0 == 1))
			#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))
			InsSave.TODEnergy.TOD[currentZoneID].kWh+=ZenergyExpkWh;
			#endif
		#else
			InsSave.TODEnergy.TOD[currentZoneID].kWh+=ZenergykWh;
		#endif
		#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))
		if(((logtimeCounter%86400)>=72000)||(((logtimeCounter%86400)<=18000)))
			InsSave.NonSolarCumkWh+=ZenergyExpkWh;
		#endif
		
		
		if(reset_option==BILL_TYPE_AUTO)
		{			
			InsSave.mdkVAhPulseCounter=checkkWh2kVAh(InsSave.mdkWhPulseCounter,InsSave.mdkVAhPulseCounter);
			if((InsSave.mdkWhPulseCounter*new_md_period) >InsSave.BillMD.Current.kW)
			{
				if(InsSave.BillMD.Current.kW)
				{
					if(InsSave.BillMD.CumkWMD>=InsSave.BillMD.Current.kW)
						InsSave.BillMD.CumkWMD-=InsSave.BillMD.Current.kW;
				}
				
				InsSave.BillMD.Current.kW=InsSave.mdkWhPulseCounter*new_md_period;
				InsSave.BillMD.Current.kW_Date=logtimeCounter;
				InsSave.BillMD.CumkWMD+=InsSave.BillMD.Current.kW;
			}
			#if (defined(GUJRAT_METER_0) && (GUJRAT_METER_0 == 1))
			{
				#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))				
				if((ZenergyExpkWh*md_period) >InsSave.TODEnergy.TOD[currentZoneID].TODMD.kW)
				{
					InsSave.TODEnergy.TOD[currentZoneID].TODMD.kW=ZenergyExpkWh*md_period;
					InsSave.TODEnergy.TOD[currentZoneID].TODMD.kW_Date=logtimeCounter;
				}			
				#endif
			}
			#else
			{
				if((InsSave.mdkWhPulseCounter*new_md_period) >InsSave.TODEnergy.TOD[currentZoneID].TODMD.kW)
				{
					InsSave.TODEnergy.TOD[currentZoneID].TODMD.kW=InsSave.mdkWhPulseCounter*new_md_period;
					InsSave.TODEnergy.TOD[currentZoneID].TODMD.kW_Date=logtimeCounter;
				}				
			}
			#endif
		}
		
		
		InsSave.CumkVAh+=ZenergykVAh;
		#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))
		InsSave.ExpCumkVAh+=ZenergyExpkVAh;
		#endif
		#if (defined(GUJRAT_METER_0) && (GUJRAT_METER_0 == 1))
			#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))
			InsSave.TODEnergy.TOD[currentZoneID].kVAh+=ZenergyExpkVAh;
			#endif
		#else
			InsSave.TODEnergy.TOD[currentZoneID].kVAh+=ZenergykVAh;
		#endif
		
		if(reset_option==BILL_TYPE_AUTO)
		{
			if((InsSave.mdkVAhPulseCounter*new_md_period)>InsSave.BillMD.Current.kVA)
			{
				if(InsSave.BillMD.Current.kVA)
				{
					if(InsSave.BillMD.CumkVAMD>=InsSave.BillMD.Current.kVA)
						InsSave.BillMD.CumkVAMD-=InsSave.BillMD.Current.kVA;
				}				
				InsSave.BillMD.Current.kVA=InsSave.mdkVAhPulseCounter*new_md_period;
				InsSave.BillMD.Current.kVA_Date=logtimeCounter;
				InsSave.BillMD.CumkVAMD+=InsSave.BillMD.Current.kVA;
			}
			#if (defined(GUJRAT_METER_0) && (GUJRAT_METER_0 == 1))
			{
				#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))
				if((ZenergyExpkVAh*md_period)>InsSave.TODEnergy.TOD[currentZoneID].TODMD.kVA)
				{
				
					InsSave.TODEnergy.TOD[currentZoneID].TODMD.kVA=ZenergyExpkVAh*md_period;
					InsSave.TODEnergy.TOD[currentZoneID].TODMD.kVA_Date=logtimeCounter;
					
				}
				#endif
			}
			#else
			{
				if((InsSave.mdkVAhPulseCounter*new_md_period)>InsSave.TODEnergy.TOD[currentZoneID].TODMD.kVA)
				{
				
					InsSave.TODEnergy.TOD[currentZoneID].TODMD.kVA=InsSave.mdkVAhPulseCounter*new_md_period;
					InsSave.TODEnergy.TOD[currentZoneID].TODMD.kVA_Date=logtimeCounter;
					
				}				
			}
			#endif
		}
		
		if(reset_option==BILL_TYPE_AUTO)
		{
			InsSave.mdkWhPulseCounter=0;
			InsSave.mdkVAhPulseCounter=0;
		}
		
		InsSave.ZkWhCounter=InsSave.ZkWhCounter-ZenergykWh*METER_CONSTANT;
		InsSave.ZkVAhCounter=InsSave.ZkVAhCounter-ZenergykVAh*METER_CONSTANT;
		#if (defined(IS_EXPORT_METER) && (IS_EXPORT_METER == 1))
		InsSave.ExpZkWhCounter=InsSave.ExpZkWhCounter-ZenergyExpkWh*METER_CONSTANT;
		InsSave.ExpZkVAhCounter=InsSave.ExpZkVAhCounter-ZenergyExpkVAh*METER_CONSTANT;
		#endif
		
		InsSave.MonthPowerOnDuration+=InsSave.PowerOn30;
		InsSave.CumPowerOnDuration+=InsSave.PowerOn30;
		#if (defined(OLD_AVG_U_AVG_C_LOGIC) && (OLD_AVG_U_AVG_C_LOGIC == 1))		
		
		if(InsSave.PowerOn30!=0)
			InsSave.lsAvgV=InsSave.lsAvgV/InsSave.PowerOn30;
		if(InsSave.PowerOn30!=0)
			InsSave.lsAvgC=InsSave.lsAvgC/InsSave.PowerOn30;
		#endif			
			
		InsSave.PowerOn30=0;
		currentZoneID=getCurrentZoneNo(InsSave.timeCounter);
		
	//--------------------------------------------------
	
	
}

