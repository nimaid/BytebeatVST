echo off

REM - batch file to build VS2010 project and zip the resulting binaries (or make installer)
REM - updating version numbers requires python and python path added to %PATH% env variable 
REM - zipping requires 7zip in %ProgramFiles%\7-Zip\7z.exe
REM - building installer requires innotsetup in "%ProgramFiles(x86)%\Inno Setup 5\iscc"
REM - AAX codesigning requires ashelper tool added to %PATH% env variable and aax.key/.crt in .\..\..\..\Certificates\

echo Making IPlugDistortion win distribution ...

echo ------------------------------------------------------------------
echo Updating version numbers ...

call python update_version.py

echo ------------------------------------------------------------------
echo Building ...

if exist "%ProgramFiles(x86)%" (goto 64-Bit) else (goto 32-Bit)

:32-Bit
echo 32-Bit O/S detected
call "%ProgramFiles%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
goto END

:64-Bit
echo 64-Bit Host O/S detected
call "%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"
goto END
:END

REM - set preprocessor macros like this, for instance to enable demo build:
REM - SET CMDLINE_DEFINES="DEMO_VERSION"

REM - Could build individual targets like this:
REM - msbuild IPlugDistortion-app.vcxproj /p:configuration=release /p:platform=win32

msbuild IPlugDistortion.sln /p:configuration=release /p:platform=win32 /nologo /noconsolelogger /fileLogger /v:quiet /flp:logfile=build-win.log;errorsonly 
msbuild IPlugDistortion.sln /p:configuration=release /p:platform=x64 /nologo /noconsolelogger /fileLogger /v:quiet /flp:logfile=build-win.log;errorsonly;append

echo ------------------------------------------------------------------
echo Code sign aax binary...
call ashelper -f .\build-win\aax\bin\IPlugDistortion.aaxplugin\Contents\Win32\IPlugDistortion.aaxplugin -l .\..\..\..\Certificates\aax.crt -k .\..\..\..\Certificates\aax.key -o .\build-win\aax\bin\IPlugDistortion.aaxplugin\Contents\Win32\IPlugDistortion.aaxplugin
REM - call ashelper -f .\build-win\aax\bin\IPlugDistortion.aaxplugin\Contents\x64\IPlugDistortion.aaxplugin -l .\..\..\..\Certificates\aax.crt -k .\..\..\..\Certificates\aax.key -o .\build-win\aax\bin\IPlugDistortion.aaxplugin\Contents\x64\IPlugDistortion.aaxplugin

REM - Make Installer (InnoSetup)

echo ------------------------------------------------------------------
echo Making Installer ...

if exist "%ProgramFiles(x86)%" (goto 64-Bit-is) else (goto 32-Bit-is)

:32-Bit-is
"%ProgramFiles%\Inno Setup 5\iscc" /cc ".\installer\IPlugDistortion.iss"
goto END-is

:64-Bit-is
"%ProgramFiles(x86)%\Inno Setup 5\iscc" /cc ".\installer\IPlugDistortion.iss"
goto END-is

:END-is

REM - ZIP
REM - "%ProgramFiles%\7-Zip\7z.exe" a .\installer\IPlugDistortion-win-32bit.zip .\build-win\app\win32\bin\IPlugDistortion.exe .\build-win\vst3\win32\bin\IPlugDistortion.vst3 .\build-win\vst2\win32\bin\IPlugDistortion.dll .\build-win\rtas\bin\IPlugDistortion.dpm .\build-win\rtas\bin\IPlugDistortion.dpm.rsr .\build-win\aax\bin\IPlugDistortion.aaxplugin* .\installer\license.rtf .\installer\readmewin.rtf
REM - "%ProgramFiles%\7-Zip\7z.exe" a .\installer\IPlugDistortion-win-64bit.zip .\build-win\app\x64\bin\IPlugDistortion.exe .\build-win\vst3\x64\bin\IPlugDistortion.vst3 .\build-win\vst2\x64\bin\IPlugDistortion.dll .\installer\license.rtf .\installer\readmewin.rtf

echo ------------------------------------------------------------------
echo Printing log file to console...

type build-win.log

pause