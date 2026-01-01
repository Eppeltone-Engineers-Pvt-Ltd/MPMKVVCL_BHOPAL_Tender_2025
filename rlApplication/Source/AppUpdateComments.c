//852
//1. NM LOCK COUNT INCREADED FROM 5 TO 180
//2. DEFAULT CALIBRATION CHANGED .
//3. NM LOCK WILL RESET WHEN PUSH BUTTON IS PRESSED.
//4. NM EXIT TIME DUE TO FALSE CURRENT CHANGED TO 3 MINUTES FROM 50 MINUTES.
//853
//5. REMOVED VER# FROM DISPLAY
//6. NOW IT CAN BE READ FROM COMMUNICATION. COMMAND "EEOVER0D0A";


//862
//1. earth is blinking. which is not correct.
//2. dc immunity range new set.

//866
//1. change in nm logic in normal mode.

//868
//1. Change in production display parameters.
//2. removed blinking in production mode.
//3. now command added to change offset values for current.
//4. show rev icon in production mode.
 
//875
//1. dynamic offset added when current flows through CT.
//2. change nm case if voltage is less than 120V
//0.03.14
//1. erase blocked in user mode.
//0.03.15
//1. first billing happening in NM if there is not LS data logged in normal mode.
//0.03.16
//1. power on duration is coorected in case meter RUN in NM.

//0.03.17
//1. billing occurs while erased in NM case.
//0.03.17
//inavlid

//0.03.019
//1. ND detection logic changed in case if it is applied REV.
//2. i2c reset clock increased for i2c hang problem.

//1.04.017
// 1. dc magnet immune logic changed.
//1.04.023
//1. meter will not run below 100V
//2. meter will comes only from power off mode if volatge is greater than 110V or if less than 110V and load is earthed.
//1.04.024
//1. meter takes time from battery mode to normal mode if supply is applied on portable source, if battery mode time is more than 5 mins.
//2. VRTC pin will get supply from MCU in AC supply condition.
//3. Now all seg will show when memtest command will be received in production mode.
//4. first stage powerfail data will be saved when voltage is less than 120V.
//4.65.000
//prekWh initial value changed.
//1. some times crc of data saved at power fail event mismatched. this is beacuse 12 bit timer counter modify pulse counter values after calculatiing crc.
//2. if there is no more than .1 kwh increament in one run, and kwh increased more than 3 kwh in such repeatition then kwh will become 0. now it is corrected.
//3. power fail data saved 1 level voltage chnaged to 120V. 

//900
//1. magnet logic changed.
//2. power assigned before tamper checking.

//904
//.1. NM accuracy flactuating.

//606
//1. R symbol will blink for cover open
//2. cover open will show continously
//3. cover open tamper will appear in push button with date and time.
//4. tod timing for seasons interchanged. (Reported by Pankaj )
//607,508
//1. no load scan time reduced to 60 secs from 3600 secs.
//609,510
//1. lcd on in battery mode after power fail.
//611,512
//1. avg pf of last month added in power fail mode
//617,516
//1. Avg V,Avg Current logic changed.
//2. Pf will be unity while there is no load
//3. Apperent power will be zero if it is below 1.6VA
//4. active power is greater than Apperent power check place after all calculation.
//5. tod zone time should be macth integration period , if not match then not set.
//621,520
//1. cover tamprer count is removed from total tamper count.
/*623,522
1. 2 seconds power added into NM case in battery mode while first time current detected to adjust NM case dial test.
2. Earth, Rev and ND will not restore below 50mA.
30/09/2022
3. production mode rtc batetry status modiled, earlier it show main battery status.
4. bill day obis code modified, now need to set through single action schedule.
4.1 bill date=="FF/FF/FF" then md reset
4.2 bill date=="xx/FF/FF" then bill day set.
4.3 otherwise single action scheduled.
5. Check Selft Diagonstic feature
13/10/2022
1. form factor threshold changed from 2 to 1.78, if voltage is less than 120V.
2. In battry mode voltage threshold chnaged from 50V to 35 V. it can effect NM case , need to verify it.
12/11/2022
1. commented E2PR acces in dlms.
2. also changed typedef for tag_attr_access_method_t in r_dlms_obis_ic.h file.
3. lcd mapping chnaged for Type A lcd.
15/11/2022
1. TOD kWh & kVAh register value not incrementing as per main kWh. Current TOD enrergy added for export energy.
15/11/2022
1. Wattage power will be in 1 decimal place , it was 2 decimal places
26/12/2022
1. no of season as per reuirement not 2 seasons always.
2. TOD activation transaction will be immediate.
3. md integration can be read immediate
4. blinking on display if nd case U<200V
5. Tamper persistance time.
27/12/2022
1. magnet time reduced from 1 min to 30 sec.
2. Push Display parameters are not ok when no tamper occurred.
3. Addition of currents in NM not effective for energy calculation.
4. NM data not logging in case of NM if events are more than 32  intervals.
28/12/2022-606
1. NM persistance time was 60 seconds in battery mode, now it is 300 seconds.
29/12/2022-MP 607
1. load survery data log in nm mode at interval of 32 was wrong, it was writing only first 32 interval data.
30/12/2022-MP 608
1. default calibration values changed.
04/01/2023-MP 611
1. time of rtc is now in HH:mm:ss mode.

18/01/2023-MP 612
1. tod default zones will be 3.
2. remove kVA md from display
19/01/2023-MP 614
1. added kVA MD into auto scroll mode.
24/01/2023-MP 615
1. calibration location chnage from 0x00 to 0x08
2. memtest enabled during memory clear.
3. in memtest function first write 4 buyes to 0x00 location then proceed forrther steps.
24/01/2023-MP 616
1. calibration location again 0x00
27/01/2023-MP INDORE 701
1. harmonics file will be disable if harmonics not enabled.
2. daily ls for indore will be 45 days.
3. kW MD, kVA MD will not availabe in auto mode.
//17/02/2023 MP_INDORE 703
1. timercounter increament in power fail mode commented. this will come from getrtcCounter function when user pressed button and load available and getrtccounter commented while not load availabe 
2. GetTamperForwardLoc is commented for dlms meters
3. "RR" command for rtc read is commented
4. memory clear status 0x01020304 is removed .
5. receving and transmitting code is commented due to code size issue.
//03/04/2023 MP INDORE 709
1. calibration second loaction added
2. push button first parameter not scrolling when push button pressed in nm case.
3. uinty power factor added while magned applied.
//12/04/2023 MP INDORE 710
1. Timer0 can stop due to loW bat so it can create problem in Timer on-off test. display can struct and energy will not increment.
2.LS scalar for kWh, kVAh and kW is changed fromm 10 to 1.
//28/04/2023 MP INDORE 711,712
1. self disgonistic feature bits status changed 0-OK,1-NOT OK
2. avoid to go into sleep mode due to fast on -off test.
14/07/2023 INDORE-716
1. single schedule stamping adjust if it is divisible of 5 min.
2. leakage max current will be 50mA for tamper checking.
3. extra logical device name removed from class 17.
4. obis code of ph current corrected in event profile.
09/08/2023-INDORE 717
1. rtc re-init will be done when new rtc is set.
26/08/2023-INDORE 718
1. apparent power threshold chnaged from 1.6VA to 2.2VA
2. power factor will be uinty in case of magnet recovery with no load condition.
3. scalar and uinit of export kWh and export kVAh correct in instant profile.
4. script table class added.
5. now we can configure and read back week days ids.
6. code optimized to read class 3 parameters.

26/08/2023-INDORE 719
1. reference volatge added when magnet occurs.
2. obis code of tod md kva of tod zone 8 corrected.
3. scalar of cummulative tamper count scalar corrected.
23/09/2023-INDORE 720
1. effective current added into event profile.
30/09/2023	INDORE 720,721
1. billing profile tamper count scalar corrected.
2. actual parameters will show during magnet recovery.
07/11/2023	INDORE 722
1. tamper leakage changed.
07/11/2023	INDORE 723
1. leakage current with blocker.
*/
