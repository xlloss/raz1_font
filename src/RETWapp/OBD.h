
#define			PID_EngineTemp						0x05
#define			PID_IntakeAirPressure			0x0B
#define			PID_EngineRPM							0x0C
#define			PID_VehicleSPEED					0x0D
#define			PID_IntakeTemp						0x0F
#define			PID_MAF										0x10
#define			PID_Throttle							0x11
#define			PID_AuxStatus							0x1E
#define			PID_RunTimeStart					0x1F

#define			PID_FuelTankLevel					0x2F


const unsigned char OBD2ModePID[256][3]={
0x00,4,				// PIDs supported [01 - 20]    Bit encoded [A7..D0] == [PID $01..PID $20] See below 
0x01,4,				// Monitor status since DTCs cleared. (Includes malfunction indicator lamp (MIL) status and number of DTCs.)    Bit encoded. See below 
0x02,2,				// Freeze DTC                       
0x03,2,				// Fuel system status    Bit encoded. See below                   
0x04,1,				// Calculated engine load 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} (or A 2.55      {\displaystyle {\tfrac {A}{2.55}}}  {\displaystyle {\tfrac {A}{2.55}}}) 
0x05,1,				// Engine coolant temperature -40 215 ¢XC A ? 40   {\displaystyle A-40}  {\displaystyle A-40}               
0x06,1,				// Short term fuel trim¡XBank 1 -100 (Reduce Fuel: Too Rich) 99.2 (Add Fuel: Too Lean)  %100 128   A ? 100   {\displaystyle {\frac {100}{128}}A-100} {\displaystyle {\frac {100}{128}}A-100}(or A 1.28    ? 100   {\displaystyle {\tfrac {A}{1.28}}-100}  {\displaystyle {\tfrac {A}{1.28}}-100} ) 
0x07,1,				// Long term fuel trim¡XBank 1 
0x08,1,				// Short term fuel trim¡XBank 2 
0x09,1,				// Long term fuel trim¡XBank 2 
0x0A,1,				// Fuel pressure (gauge pressure) 0 765 kPa 3 A   {\displaystyle 3A}  {\displaystyle 3A} 
0x0B,1,				// Intake manifold absolute pressure 0 255 kPa A   {\displaystyle A}  A 
0x0C,2,				// Engine RPM 0 16,383.75 rpm 256 A + B  4     {\displaystyle {\frac {256A+B}{4}}}  {\displaystyle {\frac {256A+B}{4}}} 
0x0D,1,				// Vehicle speed 0 255 km/h A   {\displaystyle A}  A 
0x0E,1,				// Timing advance -64 63.5 ¢X before TDC A 2   ? 64   {\displaystyle {\frac {A}{2}}-64}  {\displaystyle {\frac {A}{2}}-64} 
0x0F,1,				// Intake air temperature -40 215 ¢XC A ? 40   {\displaystyle A-40}  {\displaystyle A-40} 
0x10,2,				// MAF air flow rate 0 655.35 grams/sec 256 A + B  100     {\displaystyle {\frac {256A+B}{100}}}  {\displaystyle {\frac {256A+B}{100}}} 
0x11,1,				// Throttle position 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
0x12,1,				// Commanded secondary air status    Bit encoded. See below 
0x13,1,				// Oxygen sensors present (in 2 banks)    [A0..A3] == Bank 1, Sensors 1-4. [A4..A7] == Bank 2... 
0x14,2,				// Oxygen Sensor 1  A: Voltage  	B: Short term fuel trim 0 -100 1.275 99.2 volts %A 200     {\displaystyle {\frac {A}{200}}}  {\displaystyle {\frac {A}{200}}}100 128   B ? 100   {\displaystyle {\frac {100}{128}}B-100}  {\displaystyle {\frac {100}{128}}B-100}(if B==$FF, sensor is not used in trim calculation) 
0x15,2,				// Oxygen Sensor 2	A: Voltage		B: Short term fuel trim 
0x16,2,				// Oxygen Sensor 3	A: Voltage		B: Short term fuel trim 
0x17,2,				// Oxygen Sensor 4	A: Voltage		B: Short term fuel trim 
0x18,2,				// Oxygen Sensor 5	A: Voltage		B: Short term fuel trim 
0x19,2,				// Oxygen Sensor 6	A: Voltage		B: Short term fuel trim 
0x1A,2,				// Oxygen Sensor 7	A: Voltage		B: Short term fuel trim 
0x1B,2,				// Oxygen Sensor 8	A: Voltage		B: Short term fuel trim 
0x1C,1,				// OBD standards this vehicle conforms to    Bit encoded. See below 
0x1D,1,				// Oxygen sensors present (in 4 banks)    Similar to PID 13, but [A0..A7] == [B1S1, B1S2, B2S1, B2S2, B3S1, B3S2, B4S1, B4S2] 
0x1E,1,				// Auxiliary input status    A0 == Power Take Off (PTO) status (1 == active)	[A1..A7] not used 
0x1F,2,				// Run time since engine start 0 65,535 seconds 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 

0x20,4,				// PIDs supported [21 - 40]    Bit encoded [A7..D0] == [PID $21..PID $40] See below 
0x21,2,				// Distance traveled with malfunction indicator lamp (MIL) on 0 65,535 km 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
0x22,2,				// Fuel Rail Pressure (relative to manifold vacuum) 0 5177.265 kPa 0.079 ( 256 A + B )   {\displaystyle 0.079(256A+B)}  {\displaystyle 0.079(256A+B)} 
0x23,2,				// Fuel Rail Gauge Pressure (diesel, or gasoline direct injection) 0 655,350 kPa 10 ( 256 A + B )   {\displaystyle 10(256A+B)}  {\displaystyle 10(256A+B)} 
0x24,4,				// Oxygen Sensor 1	AB: Fuel¡VAir Equivalence Ratio	CD: Voltage 0		0 < 2	< 8 ratio V 2 65536   ( 256 A + B )   {\displaystyle {\frac {2}{65536}}(256A+B)}  {\displaystyle {\frac {2}{65536}}(256A+B)}8 65536   ( 256 C + D )   {\displaystyle {\frac {8}{65536}}(256C+D)}  {\displaystyle {\frac {8}{65536}}(256C+D)} 
0x25,4,				// Oxygen Sensor 2	AB: Fuel¡VAir Equivalence Ratio	CD: Voltage 
0x26,4,				// Oxygen Sensor 3	AB: Fuel¡VAir Equivalence Ratio	CD: Voltage 
0x27,4,				// Oxygen Sensor 4	AB: Fuel¡VAir Equivalence Ratio	CD: Voltage 
0x28,4,				// Oxygen Sensor 5	AB: Fuel¡VAir Equivalence Ratio	CD: Voltage 
0x29,4,				// Oxygen Sensor 6	AB: Fuel¡VAir Equivalence Ratio	CD: Voltage 
0x2A,4,				// Oxygen Sensor 7	AB: Fuel¡VAir Equivalence Ratio	CD: Voltage 
0x2B,4,				// Oxygen Sensor 8	AB: Fuel¡VAir Equivalence Ratio	CD: Voltage 
0x2C,1,				// Commanded EGR 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
0x2D,1,				// EGR Error -100 99.2  % 100 128    A ? 100   {\displaystyle {\tfrac {100}{128}}A-100}  {\displaystyle {\tfrac {100}{128}}A-100} 
0x2E,1,				// Commanded evaporative purge 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
0x2F,1,				// Fuel Tank Level Input 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
0x30,1,				// Warm-ups since codes cleared 0 255 count A   {\displaystyle A}  A 
0x31,2,				// Distance traveled since codes cleared 0 65,535 km 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
0x32,2,				// Evap. System Vapor Pressure -8,192 8191.75 Pa 256 A + B  4     {\displaystyle {\frac {256A+B}{4}}}  {\displaystyle {\frac {256A+B}{4}}} (AB is two's complement signed)[3]
0x33,1,				// Absolute Barometric Pressure 0 255 kPa A   {\displaystyle A}  A 
0x34,4,				// Oxygen Sensor 1	AB: Fuel¡VAir Equivalence Ratio 	CD: Current 0 -128 < 2 <128 ratio mA 2 65536   ( 256 A + B )   {\displaystyle {\frac {2}{65536}}(256A+B)}  {\displaystyle {\frac {2}{65536}}(256A+B)}256 C + D  256   ? 128   {\displaystyle {\frac {256C+D}{256}}-128}{\displaystyle {\frac {256C+D}{256}}-128}or C + D 256   ? 128   {\displaystyle C+{\frac {D}{256}}-128}  {\displaystyle C+{\frac {D}{256}}-128} 
0x35,4,				// Oxygen Sensor 2	AB: Fuel¡VAir Equivalence Ratio	CD: Current 
0x36,4,				// Oxygen Sensor 3 	AB: Fuel¡VAir Equivalence Ratio	CD: Current 
0x37,4,				// Oxygen Sensor 4	AB: Fuel¡VAir Equivalence Ratio	CD: Current 
0x38,4,				// Oxygen Sensor 5	AB: Fuel¡VAir Equivalence Ratio	CD: Current 
0x39,4,				// Oxygen Sensor 6	AB: Fuel¡VAir Equivalence Ratio	CD: Current 
0x3A,4,				// Oxygen Sensor 7	AB: Fuel¡VAir Equivalence Ratio	CD: Current 
0x3B,4,				// Oxygen Sensor 8	AB: Fuel¡VAir Equivalence Ratio	CD: Current 
0x3C,2,				// Catalyst Temperature: Bank 1, Sensor 1 -40 6,513.5 ¢XC 256 A + B  10   ? 40   {\displaystyle {\frac {256A+B}{10}}-40}  {\displaystyle {\frac {256A+B}{10}}-40} 
0x3D,2,				// Catalyst Temperature: Bank 2, Sensor 1 
0x3E,2,				// Catalyst Temperature: Bank 1, Sensor 2 
0x3F,2,				// Catalyst Temperature: Bank 2, Sensor 2 

0x40,4,				// PIDs supported [41 - 60]    Bit encoded [A7..D0] == [PID $41..PID $60] See below 
0x41,4,				// Monitor status this drive cycle    Bit encoded. See below 
0x42,2,				// Control module voltage 0 65.535 V 256 A + B  1000     {\displaystyle {\frac {256A+B}{1000}}}  {\displaystyle {\frac {256A+B}{1000}}} 
0x43,2,				// Absolute load value 0 25,700  % 100 255    ( 256 A + B )   {\displaystyle {\tfrac {100}{255}}(256A+B)}  {\displaystyle {\tfrac {100}{255}}(256A+B)} 
0x44,2,				// Fuel¡VAir commanded equivalence ratio 0 < 2 ratio 2 65536    ( 256 A + B )   {\displaystyle {\tfrac {2}{65536}}(256A+B)}  {\displaystyle {\tfrac {2}{65536}}(256A+B)} 
0x45,1,				// Relative throttle position 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
0x46,1,				// Ambient air temperature -40 215 ¢XC A ? 40   {\displaystyle A-40}  {\displaystyle A-40} 
0x47,1,				// Absolute throttle position B 0 100  % 100 255   A   {\displaystyle {\frac {100}{255}}A}  {\displaystyle {\frac {100}{255}}A} 
0x48,1,				// Absolute throttle position C 
0x49,1,				// Accelerator pedal position D 
0x4A,1,				// Accelerator pedal position E 
0x4B,1,				// Accelerator pedal position F 
0x4C,1,				// Commanded throttle actuator 
0x4D,2,				// Time run with MIL on 0 65,535 minutes 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
0x4E,2,				// Time since trouble codes cleared 
0x4F,4,				// Maximum value for Fuel¡VAir equivalence ratio, oxygen sensor voltage, oxygen sensor current, and intake manifold absolute pressure 0, 0, 0, 0 255, 255, 255, 2550 ratio, V, mA, kPa A, B, C, D*10 
0x50,4,				// Maximum value for air flow rate from mass air flow sensor 0 2550 g/s A*10, B, C, and D are reserved for future use 
0x51,1,				// Fuel Type    From fuel type table see below 
0x52,1,				// Ethanol fuel % 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
0x53,2,				// Absolute Evap system Vapor Pressure 0 327.675 kPa 256 A + B  200     {\displaystyle {\frac {256A+B}{200}}}  {\displaystyle {\frac {256A+B}{200}}} 
0x54,2,				// Evap system vapor pressure -32,767 32,768 Pa ((A*256)+B)-32767 
0x55,2,				// Short term secondary oxygen sensor trim, A: bank 1, B: bank 3 -100 99.2  % 100 128   A ? 100   {\displaystyle {\frac {100}{128}}A-100}  {\displaystyle {\frac {100}{128}}A-100} 100 128   B ? 100   {\displaystyle {\frac {100}{128}}B-100}  {\displaystyle {\frac {100}{128}}B-100}
0x56,2,				// Long term secondary oxygen sensor trim, A: bank 1, B: bank 3 
0x57,2,				// Short term secondary oxygen sensor trim, A: bank 2, B: bank 4 
0x58,2,				// Long term secondary oxygen sensor trim, A: bank 2, B: bank 4 
0x59,2,				// Fuel rail absolute pressure 0 655,350 kPa 10 ( 256 A + B )   {\displaystyle 10(256A+B)}  {\displaystyle 10(256A+B)} 
0x5A,1,				// Relative accelerator pedal position 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
0x5B,1,				// Hybrid battery pack remaining life 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
0x5C,1,				// Engine oil temperature -40 210 ¢XC A ? 40   {\displaystyle A-40}  {\displaystyle A-40} 
0x5D,2,				// Fuel injection timing -210.00 301.992 ¢X 256 A + B  128   ? 210   {\displaystyle {\frac {256A+B}{128}}-210}  {\displaystyle {\frac {256A+B}{128}}-210} 
0x5E,2,				// Engine fuel rate 0 3276.75 L/h 256 A + B  20     {\displaystyle {\frac {256A+B}{20}}}  {\displaystyle {\frac {256A+B}{20}}} 
0x5F,1,				// Emission requirements to which vehicle is designed    Bit Encoded 

0x60,4,				// PIDs supported [61 - 80]    Bit encoded [A7..D0] == [PID $61..PID $80] See below 
0x61,1,				// Driver's demand engine - percent torque -125 125  % A-125 
0x62,1,				// Actual engine - percent torque -125 125  % A-125 
0x63,2,				// Engine reference torque 0 65,535 Nm 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
0x64,5,				// Engine percent torque data -125 125  % A-125 Idle	B-125 Engine point 1		C-125 Engine point 2		D-125 Engine point 3	E-125 Engine point 4 
0x65,2,				// Auxiliary input / output supported    Bit Encoded 
0x66,5,				// Mass air flow sensor     
0x67,3,				// Engine coolant temperature     
0x68,7,				// Intake air temperature sensor     
0x69,7,				// Commanded EGR and EGR Error     
0x6A,5,				// Commanded Diesel intake air flow control and relative intake air flow position     
0x6B,5,				// Exhaust gas recirculation temperature     
0x6C,5,				// Commanded throttle actuator control and relative throttle position     
0x6D,6,				// Fuel pressure control system     
0x6E,5,				// Injection pressure control system     
0x6F,3,				// Turbocharger compressor inlet pressure     
0x70,9,				// Boost pressure control     
0x71,5,				// Variable Geometry turbo (VGT) control     
0x72,5,				// Wastegate control     
0x73,5,				// Exhaust pressure     
0x74,5,				// Turbocharger RPM     
0x75,7,				// Turbocharger temperature     
0x76,7,				// Turbocharger temperature     
0x77,5,				// Charge air cooler temperature (CACT)     
0x78,9,				// Exhaust Gas temperature (EGT) Bank 1    Special PID. See below 
0x79,9,				// Exhaust Gas temperature (EGT) Bank 2    Special PID. See below 
0x7A,7,				// Diesel particulate filter (DPF)     
0x7B,7,				// Diesel particulate filter (DPF)     
0x7C,9,				// Diesel Particulate filter (DPF) temperature     
0x7D,1,				// NOx NTE control area status     
0x7E,1,				// PM NTE control area status     
0x7F,13, 			// Engine run time     

0x80,4, 			// PIDs supported [81 - A0]    Bit encoded [A7..D0] == [PID $81..PID $A0] See below 
0x81,21, 			// Engine run time for Auxiliary Emissions Control Device(AECD)     
0x82,21, 			// Engine run time for Auxiliary Emissions Control Device(AECD)     
0x83,5, 			// NOx sensor     
0x84,0,				// Manifold surface temperature     
0x85,0,				// NOx reagent system     
0x86,0,				// Particulate matter (PM) sensor     
0x87,0,				// Intake manifold absolute pressure     
0xA0,4, 			//	PIDs supported [A1 - C0]    Bit encoded [A7..D0] == [PID $A1..PID $C0] See below 
0xC0,4, 			//	PIDs supported [C1 - E0]    Bit encoded [A7..D0] == [PID $C1..PID $E0] See below 
0xC3,0, 			//	?  ?  ?  ?  ? Returns numerous data, including Drive Condition ID and Engine Speed* 
0xC4,0, 			//	?  ?  ?  ?  ? B5 is Engine Idle Request
0xB6,0				//	is Engine Stop Request*          
};
    
  
#if 0      
00  PIDs supported [01 - 20]    Bit encoded [A7..D0] == [PID $01..PID $20] See below 
01 4 Monitor status since DTCs cleared. (Includes malfunction indicator lamp (MIL) status and number of DTCs.)    Bit encoded. See below 
02 2 Freeze DTC                       
03 2 Fuel system status    Bit encoded. See below                   
04 1 Calculated engine load 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} (or A 2.55      {\displaystyle {\tfrac {A}{2.55}}}  {\displaystyle {\tfrac {A}{2.55}}}) 
05 1 Engine coolant temperature -40 215 ¢XC A ? 40   {\displaystyle A-40}  {\displaystyle A-40}               
06 1 Short term fuel trim¡XBank 1 -100 (Reduce Fuel: Too Rich) 99.2 (Add Fuel: Too Lean)  %                   

100 128   A ? 100   {\displaystyle {\frac {100}{128}}A-100}  
{\displaystyle {\frac {100}{128}}A-100}(or A 1.28    ? 100   {\displaystyle {\tfrac {A}{1.28}}-100}  {\displaystyle {\tfrac {A}{1.28}}-100} ) 
07 1 Long term fuel trim¡XBank 1 
08 1 Short term fuel trim¡XBank 2 
09 1 Long term fuel trim¡XBank 2 
0A 1 Fuel pressure (gauge pressure) 0 765 kPa 3 A   {\displaystyle 3A}  {\displaystyle 3A} 
0B 1 Intake manifold absolute pressure 0 255 kPa A   {\displaystyle A}  A 
0C 2 Engine RPM 0 16,383.75 rpm 256 A + B  4     {\displaystyle {\frac {256A+B}{4}}}  {\displaystyle {\frac {256A+B}{4}}} 
0D 1 Vehicle speed 0 255 km/h A   {\displaystyle A}  A 
0E 1 Timing advance -64 63.5 ¢X before TDC A 2   ? 64   {\displaystyle {\frac {A}{2}}-64}  {\displaystyle {\frac {A}{2}}-64} 
0F 1 Intake air temperature -40 215 ¢XC A ? 40   {\displaystyle A-40}  {\displaystyle A-40} 
10 2 MAF air flow rate 0 655.35 grams/sec 256 A + B  100     {\displaystyle {\frac {256A+B}{100}}}  {\displaystyle {\frac {256A+B}{100}}} 
11 1 Throttle position 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
12 1 Commanded secondary air status    Bit encoded. See below 
13 1 Oxygen sensors present (in 2 banks)    [A0..A3] == Bank 1, Sensors 1-4. [A4..A7] == Bank 2... 
14 2 Oxygen Sensor 1
 A: Voltage
 B: Short term fuel trim 0
 -100 1.275
 99.2 volts 
%
 

A 200     {\displaystyle {\frac {A}{200}}}  
{\displaystyle {\frac {A}{200}}}

100 128   B ? 100   {\displaystyle {\frac {100}{128}}B-100}  
{\displaystyle {\frac {100}{128}}B-100}(if B==$FF, sensor is not used in trim calculation) 
15 2 Oxygen Sensor 2
 A: Voltage
 B: Short term fuel trim 
16 2 Oxygen Sensor 3
 A: Voltage
 B: Short term fuel trim 
17 2 Oxygen Sensor 4
 A: Voltage
 B: Short term fuel trim 
18 2 Oxygen Sensor 5
 A: Voltage
 B: Short term fuel trim 
19 2 Oxygen Sensor 6
 A: Voltage
 B: Short term fuel trim 
1A 2 Oxygen Sensor 7
 A: Voltage
 B: Short term fuel trim 
1B 2 Oxygen Sensor 8
 A: Voltage
 B: Short term fuel trim 
1C 1 OBD standards this vehicle conforms to    Bit encoded. See below 
1D 1 Oxygen sensors present (in 4 banks)    Similar to PID 13, but [A0..A7] == [B1S1, B1S2, B2S1, B2S2, B3S1, B3S2, B4S1, B4S2] 
1E 1 Auxiliary input status    A0 == Power Take Off (PTO) status (1 == active)
 [A1..A7] not used 
1F 2 Run time since engine start 0 65,535 seconds 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
20 4 PIDs supported [21 - 40]    Bit encoded [A7..D0] == [PID $21..PID $40] See below 
21 2 Distance traveled with malfunction indicator lamp (MIL) on 0 65,535 km 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
22 2 Fuel Rail Pressure (relative to manifold vacuum) 0 5177.265 kPa 0.079 ( 256 A + B )   {\displaystyle 0.079(256A+B)}  {\displaystyle 0.079(256A+B)} 
23 2 Fuel Rail Gauge Pressure (diesel, or gasoline direct injection) 0 655,350 kPa 10 ( 256 A + B )   {\displaystyle 10(256A+B)}  {\displaystyle 10(256A+B)} 
24 4 Oxygen Sensor 1
 AB: Fuel¡VAir Equivalence Ratio
 CD: Voltage 0
 0 < 2
 < 8 ratio
 V 

2 65536   ( 256 A + B )   {\displaystyle {\frac {2}{65536}}(256A+B)}  
{\displaystyle {\frac {2}{65536}}(256A+B)}

8 65536   ( 256 C + D )   {\displaystyle {\frac {8}{65536}}(256C+D)}  
{\displaystyle {\frac {8}{65536}}(256C+D)} 
25 4 Oxygen Sensor 2
 AB: Fuel¡VAir Equivalence Ratio
 CD: Voltage 
26 4 Oxygen Sensor 3
 AB: Fuel¡VAir Equivalence Ratio
 CD: Voltage 
27 4 Oxygen Sensor 4
 AB: Fuel¡VAir Equivalence Ratio
 CD: Voltage 
28 4 Oxygen Sensor 5
 AB: Fuel¡VAir Equivalence Ratio
 CD: Voltage 
29 4 Oxygen Sensor 6
 AB: Fuel¡VAir Equivalence Ratio
 CD: Voltage 
2A 4 Oxygen Sensor 7
 AB: Fuel¡VAir Equivalence Ratio
 CD: Voltage 
2B 4 Oxygen Sensor 8
 AB: Fuel¡VAir Equivalence Ratio
 CD: Voltage 
2C 1 Commanded EGR 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
2D 1 EGR Error -100 99.2  % 100 128    A ? 100   {\displaystyle {\tfrac {100}{128}}A-100}  {\displaystyle {\tfrac {100}{128}}A-100} 
2E 1 Commanded evaporative purge 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
2F 1 Fuel Tank Level Input 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
30 1 Warm-ups since codes cleared 0 255 count A   {\displaystyle A}  A 
31 2 Distance traveled since codes cleared 0 65,535 km 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
32 2 Evap. System Vapor Pressure -8,192 8191.75 Pa 256 A + B  4     {\displaystyle {\frac {256A+B}{4}}}  {\displaystyle {\frac {256A+B}{4}}} 
(AB is two's complement signed)[3]
 
33 1 Absolute Barometric Pressure 0 255 kPa A   {\displaystyle A}  A 
34 4 Oxygen Sensor 1
 AB: Fuel¡VAir Equivalence Ratio
 CD: Current 0
 -128 < 2
 <128 ratio
 mA 

2 65536   ( 256 A + B )   {\displaystyle {\frac {2}{65536}}(256A+B)}  
{\displaystyle {\frac {2}{65536}}(256A+B)}

256 C + D  256   ? 128   {\displaystyle {\frac {256C+D}{256}}-128}  
{\displaystyle {\frac {256C+D}{256}}-128}or C + D 256   ? 128   {\displaystyle C+{\frac {D}{256}}-128}  {\displaystyle C+{\frac {D}{256}}-128} 
35 4 Oxygen Sensor 2
 AB: Fuel¡VAir Equivalence Ratio
 CD: Current 
36 4 Oxygen Sensor 3
 AB: Fuel¡VAir Equivalence Ratio
 CD: Current 
37 4 Oxygen Sensor 4
 AB: Fuel¡VAir Equivalence Ratio
 CD: Current 
38 4 Oxygen Sensor 5
 AB: Fuel¡VAir Equivalence Ratio
 CD: Current 
39 4 Oxygen Sensor 6
 AB: Fuel¡VAir Equivalence Ratio
 CD: Current 
3A 4 Oxygen Sensor 7
 AB: Fuel¡VAir Equivalence Ratio
 CD: Current 
3B 4 Oxygen Sensor 8
 AB: Fuel¡VAir Equivalence Ratio
 CD: Current 
3C 2 Catalyst Temperature: Bank 1, Sensor 1 -40 6,513.5 ¢XC 256 A + B  10   ? 40   {\displaystyle {\frac {256A+B}{10}}-40}  {\displaystyle {\frac {256A+B}{10}}-40} 
3D 2 Catalyst Temperature: Bank 2, Sensor 1 
3E 2 Catalyst Temperature: Bank 1, Sensor 2 
3F 2 Catalyst Temperature: Bank 2, Sensor 2 
40 4 PIDs supported [41 - 60]    Bit encoded [A7..D0] == [PID $41..PID $60] See below 
41 4 Monitor status this drive cycle    Bit encoded. See below 
42 2 Control module voltage 0 65.535 V 256 A + B  1000     {\displaystyle {\frac {256A+B}{1000}}}  {\displaystyle {\frac {256A+B}{1000}}} 
43 2 Absolute load value 0 25,700  % 100 255    ( 256 A + B )   {\displaystyle {\tfrac {100}{255}}(256A+B)}  {\displaystyle {\tfrac {100}{255}}(256A+B)} 
44 2 Fuel¡VAir commanded equivalence ratio 0 < 2 ratio 2 65536    ( 256 A + B )   {\displaystyle {\tfrac {2}{65536}}(256A+B)}  {\displaystyle {\tfrac {2}{65536}}(256A+B)} 
45 1 Relative throttle position 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
46 1 Ambient air temperature -40 215 ¢XC A ? 40   {\displaystyle A-40}  {\displaystyle A-40} 
47 1 Absolute throttle position B 0 100  % 100 255   A   {\displaystyle {\frac {100}{255}}A}  {\displaystyle {\frac {100}{255}}A} 
48 1 Absolute throttle position C 
49 1 Accelerator pedal position D 
4A 1 Accelerator pedal position E 
4B 1 Accelerator pedal position F 
4C 1 Commanded throttle actuator 
4D 2 Time run with MIL on 0 65,535 minutes 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
4E 2 Time since trouble codes cleared 
4F 4 Maximum value for Fuel¡VAir equivalence ratio, oxygen sensor voltage, oxygen sensor current, and intake manifold absolute pressure 0, 0, 0, 0 255, 255, 255, 2550 ratio, V, mA, kPa A, B, C, D*10 
50 4 Maximum value for air flow rate from mass air flow sensor 0 2550 g/s A*10, B, C, and D are reserved for future use 
51 1 Fuel Type    From fuel type table see below 
52 1 Ethanol fuel % 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
53 2 Absolute Evap system Vapor Pressure 0 327.675 kPa 256 A + B  200     {\displaystyle {\frac {256A+B}{200}}}  {\displaystyle {\frac {256A+B}{200}}} 
54 2 Evap system vapor pressure -32,767 32,768 Pa ((A*256)+B)-32767 
55 2 Short term secondary oxygen sensor trim, A: bank 1, B: bank 3 -100 99.2  % 100 128   A ? 100   {\displaystyle {\frac {100}{128}}A-100}  {\displaystyle {\frac {100}{128}}A-100} 
100 128   B ? 100   {\displaystyle {\frac {100}{128}}B-100}  {\displaystyle {\frac {100}{128}}B-100}
 
56 2 Long term secondary oxygen sensor trim, A: bank 1, B: bank 3 
57 2 Short term secondary oxygen sensor trim, A: bank 2, B: bank 4 
58 2 Long term secondary oxygen sensor trim, A: bank 2, B: bank 4 
59 2 Fuel rail absolute pressure 0 655,350 kPa 10 ( 256 A + B )   {\displaystyle 10(256A+B)}  {\displaystyle 10(256A+B)} 
5A 1 Relative accelerator pedal position 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
5B 1 Hybrid battery pack remaining life 0 100  % 100 255    A   {\displaystyle {\tfrac {100}{255}}A}  {\displaystyle {\tfrac {100}{255}}A} 
5C 1 Engine oil temperature -40 210 ¢XC A ? 40   {\displaystyle A-40}  {\displaystyle A-40} 
5D 2 Fuel injection timing -210.00 301.992 ¢X 256 A + B  128   ? 210   {\displaystyle {\frac {256A+B}{128}}-210}  {\displaystyle {\frac {256A+B}{128}}-210} 
5E 2 Engine fuel rate 0 3276.75 L/h 256 A + B  20     {\displaystyle {\frac {256A+B}{20}}}  {\displaystyle {\frac {256A+B}{20}}} 
5F 1 Emission requirements to which vehicle is designed    Bit Encoded 
60 4 PIDs supported [61 - 80]    Bit encoded [A7..D0] == [PID $61..PID $80] See below 
61 1 Driver's demand engine - percent torque -125 125  % A-125 
62 1 Actual engine - percent torque -125 125  % A-125 
63 2 Engine reference torque 0 65,535 Nm 256 A + B   {\displaystyle 256A+B}  {\displaystyle 256A+B} 
64 5 Engine percent torque data -125 125  % A-125 Idle
 B-125 Engine point 1
 C-125 Engine point 2
 D-125 Engine point 3
 E-125 Engine point 4 
65 2 Auxiliary input / output supported    Bit Encoded 
66 5 Mass air flow sensor     
67 3 Engine coolant temperature     
68 7 Intake air temperature sensor     
69 7 Commanded EGR and EGR Error     
6A 5 Commanded Diesel intake air flow control and relative intake air flow position     
6B 5 Exhaust gas recirculation temperature     
6C 5 Commanded throttle actuator control and relative throttle position     
6D 6 Fuel pressure control system     
6E 5 Injection pressure control system     
6F 3 Turbocharger compressor inlet pressure     
70 9 Boost pressure control     
71 5 Variable Geometry turbo (VGT) control     
72 5 Wastegate control     
73 5 Exhaust pressure     
74 5 Turbocharger RPM     
75 7 Turbocharger temperature     
76 7 Turbocharger temperature     
77 5 Charge air cooler temperature (CACT)     
78 9 Exhaust Gas temperature (EGT) Bank 1    Special PID. See below 
79 9 Exhaust Gas temperature (EGT) Bank 2    Special PID. See below 
7A 7 Diesel particulate filter (DPF)     
7B 7 Diesel particulate filter (DPF)     
7C 9 Diesel Particulate filter (DPF) temperature     
7D 1 NOx NTE control area status     
7E 1 PM NTE control area status     
7F 13 Engine run time     
80 4 PIDs supported [81 - A0]    Bit encoded [A7..D0] == [PID $81..PID $A0] See below 
81 21 Engine run time for Auxiliary Emissions Control Device(AECD)     
82 21 Engine run time for Auxiliary Emissions Control Device(AECD)     
83 5 NOx sensor     
84  Manifold surface temperature     
85  NOx reagent system     
86  Particulate matter (PM) sensor     
87  Intake manifold absolute pressure     
A0 4 PIDs supported [A1 - C0]    Bit encoded [A7..D0] == [PID $A1..PID $C0] See below 
C0 4 PIDs supported [C1 - E0]    Bit encoded [A7..D0] == [PID $C1..PID $E0] See below 
C3  ?  ?  ?  ?  ? Returns numerous data, including Drive Condition ID and Engine Speed* 
C4  ?  ?  ?  ?  ? B5 is Engine Idle Request
 B6 is Engine Stop Request* 
#endif
