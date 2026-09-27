@echo off
set /p name="insert check name "
set /p num="insert check number "
cd .github\workflows

(
echo name: %num%
echo.
echo on:
echo   push:
echo   workflow_dispatch:
echo.
echo jobs:
echo   check:
echo     uses: ant-lab-ru/es-test/.github/workflows/%num%.yml@master
echo     secrets:
echo       ES_ACCOUNT_SECRET: ${{ secrets.ES_ACCOUNT_SECRET }}
) > %num%.yml