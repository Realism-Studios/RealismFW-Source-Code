@ECHO OFF
echo Are you sure you want to do this? [Y/N]
choice /c YN
if %errorlevel%==1 goto yes
if %errorlevel%==2 exit

:yes
echo Yes
color 0f
py -3 makeGame.py RealismProject.yaml
color 04
powershell -c "[console]::beep(3000, 1000)"
echo DON'T EXIT OUT OF THE COMPILER YET!
timeout /t 5
color 0f
echo We will now delete you're NewerDXs.
timeout /t 5
@ECHO ON
cd C:\Users\Private Snowball\AppData\Roaming\Dolphin Emulator\Load\Riivolution\NewerDX
pause
