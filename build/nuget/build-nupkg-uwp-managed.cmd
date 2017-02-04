@ECHO OFF
REM
REM Version is read from the VERSION file.
REM
REM Say VERSION contains "0.0.3" then:
REM
REM build-nupkg                     <-- generates package with version 0.0.3
REM

SETLOCAL

PUSHD "%~dp0"

WHERE /Q nuget >NUL
IF %ERRORLEVEL% NEQ 0 (     
    ECHO ERROR: nuget not found.
    ECHO.
    ECHO Run "%~pd0download-nuget.cmd" to download the latest version, or update PATH as appropriate. 
    GOTO END
)


SET PACKAGENAME=UWP.SDKforGoogleAnalytics.Managed
SET /p VERSION=<VERSION
SET BIN=bin
SET OUTDIR=..\..\src\Managed\bin
 

IF NOT "%1" == "" (
    SET VERSION=%VERSION%-%1
)

SET NUGET_ARGS=^
    -nopackageanalysis ^
    -basepath ..\..\src\Managed ^
    -outputdirectory %OUTDIR% ^
    -version %VERSION% 
     

nuget pack %PACKAGENAME%.nuspec %NUGET_ARGS%
    IF %ERRORLEVEL% NEQ 0 GOTO END

 
:END

POPD
EXIT /B %ERRORLEVEL%
