@echo off

call ..\scripts\CloneRepositories.cmd https://github.com/ImageMagick shallow

set INNOSETUP=C:\InnoSetup
if exist "C:\InnoSetup" goto strawberry

mkdir %INNOSETUP%
cd %INNOSETUP%
echo Downloading innosetup-5.5.9-unicode.exe
powershell -ExecutionPolicy Unrestricted (New-Object Net.WebClient).DownloadFile(\"https://github.com/ImageMagick/ImageMagick-Windows/releases/download/20160630/innosetup-5.5.9-unicode.exe\", \"innosetup-5.5.9-unicode.exe\")

cd ..

:strawberry

if "%1" == "x64" goto x64

set STRAWBERRYX86=C:\Strawberry-x86
if exist %STRAWBERRYX86% goto install

mkdir %STRAWBERRYX86%
cd %STRAWBERRYX86%
echo Downloading strawberry-perl-5.20.1.1-32bit.msi
powershell -ExecutionPolicy Unrestricted (New-Object Net.WebClient).DownloadFile(\"https://github.com/ImageMagick/ImageMagick-Windows/releases/download/20160630/strawberry-perl-5.20.1.1-32bit.msi\", \"strawberry-perl-5.20.1.1-32bit.msi\")


goto install

:x64

set STRAWBERRY64=C:\Strawberry-x64
if exist %STRAWBERRY64% goto install

mkdir %STRAWBERRY64%
cd %STRAWBERRY64%
echo Downloading strawberry-perl-5.20.1.1-64bit.msi
powershell -ExecutionPolicy Unrestricted (New-Object Net.WebClient).DownloadFile(\"https://github.com/ImageMagick/ImageMagick-Windows/releases/download/20160630/strawberry-perl-5.20.1.1-64bit.msi\", \"strawberry-perl-5.20.1.1-64bit.msi\")

:install
echo %~dp0
cd %~dp0
powershell -ExecutionPolicy Unrestricted .\Install-InnoSetup.ps1
if "%1" == "x86" powershell -ExecutionPolicy Unrestricted .\Install-StrawberryPerl-x86.ps1
if "%1" == "x64" powershell -ExecutionPolicy Unrestricted .\Install-StrawberryPerl-x64.ps1
cd ..
