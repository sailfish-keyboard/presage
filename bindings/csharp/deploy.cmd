:: Copyright (C) 2015  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
:: ___________________
:: The Presage project
:: ~~~~~~~~~~~~~~~~~~~


@echo off
setlocal

if "%1%" == "" (
    echo Usage: %0% presage_installation_root
    exit /b 1
) else (
    set target_bin_dir=%1\bin
)

if exist "%target_bin_dir%" (
    echo Deploying files to %target_bin_dir%\
) else (
    echo Error: presage installation directory does not contain a \bin directory
    exit /b 1
)

if "%Platform%" == "X64" (
    set build_mode=Release
    set build_arch=x64
) else (
    set build_mode=Release
    set build_arch=x86
)

:: list of files to copy across to target bin dir
set files=presage.net\bin\%build_arch%\%build_mode%\presage.net.dll presage_wcf_service\bin\%build_arch%\%build_mode%\presage_wcf_service.dll presage_csharp_demo\bin\%build_arch%\%build_mode%\presage_csharp_demo.exe presage_wcf_service_console_host\bin\%build_arch%\%build_mode%\presage_wcf_service_console_host.exe presage_wcf_service_system_tray\bin\%build_arch%\%build_mode%\presage_wcf_service_system_tray.exe

for %%i in (%files%); do (
    echo   - %%i
    if exist %%i (
        copy /Y /V %%i %target_bin_dir% || (echo Error: unable to copy %%i && exit /b 1)
    ) else (
        echo Error: %%i does not exist
	exit /b 1
    )
)

endlocal
