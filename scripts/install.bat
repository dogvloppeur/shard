@echo off
setlocal

:: Vérifie les droits administrateur
net session >nul 2>&1
if %errorLevel% NEQ 0 (
    echo Ce script doit être exécute en tant qu'administrateur.
    pause
    exit /b 1
)

:: Définit les chemins
set BIN=..\shard.exe
set INSTALL_DIR=C:\Program Files\Shard
set INSTALL_PATH=%INSTALL_DIR%\shard.exe

:: Vérifie si shard.exe existe
if not exist "%BIN%" (
    echo shard.exe introuvable. Compile-le ou place-le dans le dossier parent.
    pause
    exit /b 1
)

:: Crée le dossier d'installation s'il n'existe pas
if not exist "%INSTALL_DIR%" (
    mkdir "%INSTALL_DIR%"
)

:: Copie le fichier
copy /Y "%BIN%" "%INSTALL_PATH%"

:: Vérifie et ajoute au PATH système si nécessaire
for /f "tokens=*" %%A in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v Path 2^>nul') do (
    set REG_PATH=%%A
)

echo %REG_PATH% | find /I "%INSTALL_DIR%" >nul
if errorlevel 1 (
    echo Ajout de "%INSTALL_DIR%" au PATH...
    setx /M Path "%PATH%;%INSTALL_DIR%"
    echo Redémarre ton terminal pour que les modifications prennent effet.
)

echo Installation terminé
e.
pause
