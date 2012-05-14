@echo off
gmake

if errorlevel 3 goto error
goto end:

:error
echo.
echo Error: gmake not found
echo.
echo Install Strawberry Perl from:
echo   http://strawberryperl.com/
echo.

:end
pause