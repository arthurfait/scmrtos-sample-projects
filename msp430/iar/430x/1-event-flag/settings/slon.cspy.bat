@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM 


"D:\cad\iar\ew430.530\common\bin\cspybat" "D:\cad\iar\ew430.530\430\bin\430proc.dll" "D:\cad\iar\ew430.530\430\bin\430fet.dll"  %1 --plugin "D:\cad\iar\ew430.530\430\bin\430bat.dll" --backend -B "--hardware_multiplier" "16" "--hwmult_type" "2" "-p" "D:\cad\iar\ew430.530\430\config\MSP430F2617.ddf" "--core=430X" "--data_model=small" "--iv_base" "0xFFC0" "-d" "fet" "--verify_all" "--erase_retain_target" "--derivative" "MSP430F2618" "--protocol" "4wire" "--eem" "EMEX_HIGH" "--port" "Automatic" "--connection" "ti_usb" "--settlingtime=0" "--msp430_dll" "msp430.dll" "--vccDefault" "3.3" 


