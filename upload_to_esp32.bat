@echo off
echo ========================================
echo AI GREENHOUSE ESP32 UPLOAD SCRIPT
echo ========================================
echo.

cd /d "C:\Users\drash\Desktop\AI_Project\GreenHouseManagement\hardware_esp32\firmware"

echo Checking Arduino CLI installation...
arduino-cli version
if %errorlevel% neq 0 (
    echo ERROR: Arduino CLI not found!
    echo Please install Arduino CLI from: https://arduino.github.io/arduino-cli/
    pause
    exit /b 1
)

echo.
echo Installing ESP32 board support...
arduino-cli config init
arduino-cli core update-index
arduino-cli core install esp32:esp32

echo.
echo Compiling relay_control_mist_module.ino...
arduino-cli compile --fqbn esp32:esp32:esp32 relay_control_mist_module.ino

if %errorlevel% neq 0 (
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)

echo.
echo Compilation successful!
echo.
echo IMPORTANT: Make sure your ESP32 is connected and COM port is correct
echo.
set /p COMPORT="Enter your ESP32 COM port (e.g., COM3): "

echo.
echo Uploading to ESP32 on port %COMPORT%...
arduino-cli upload -p %COMPORT% --fqbn esp32:esp32:esp32 relay_control_mist_module.ino

if %errorlevel% neq 0 (
    echo ERROR: Upload failed! Check COM port and connections.
    pause
    exit /b 1
)

echo.
echo ========================================
echo SUCCESS! AI Greenhouse uploaded to ESP32
echo ========================================
echo.
echo Next steps:
echo 1. Open Arduino Serial Monitor (115200 baud)
echo 2. You should see: "=== SMART GREENHOUSE WITH RELAY CONTROL ==="
echo 3. Test relay clicking and mist activation
echo 4. Monitor AI decisions in real-time
echo.
echo Your greenhouse is now AI-controlled! 🌱🤖
echo.
pause