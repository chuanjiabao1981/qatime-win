@set OUTDIR=%1
@set TARGETNAME=%2
@set CONFIG=%3
@set PLATFORM=%4

@rem *******************************
@rem: copy to lib dir
@if .%1 == . goto INVALID_PARAM
@if .%2 == . goto INVALID_PARAM
@if .%3 == . goto INVALID_PARAM
@if .%4 == . goto INVALID_PARAM



@rem *******************************
@rem: Debug模式下生成的文件名带_d
@set AppendixForSA=

@if .%CONFIG% == .Release goto IsRelease
@set DstDir=.\Bin\Debug
@goto endif_CheckDebug

:IsRelease
@set DstDir=.\Bin\Release
:endif_CheckDebug


@set LibSrcDir=.\depend\lib\win32
@set DllSrcDir=.\depend\dlllink


@echo copy to lib dir
@echo %DstDir%

COPY %LibSrcDir%\*.dll %DstDir% /Y
COPY %DllSrcDir%\* %DstDir% /Y
mkdir %DstDir%\platforms
COPY %DllSrcDir%\platforms %DstDir%\platforms /Y

mkdir %DstDir%\images
mkdir %DstDir%\styles
COPY .\images       %DstDir%\images /Y
COPY .\styles       %DstDir%\styles /Y


@goto END

:INVALID_PARAM
echo invalid parameter to copy.bat
@pause 

:END

