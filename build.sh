cd esp
platformio run
cd ../sam
#platformio run --target upload
platformio run
uploadBOSSA .pioenvs/zeroUSB/firmware.bin
sleep 1
echo "** Asking for ESP flash mode... **"
sudo -u vico echo "set mode esp flash" >> /dev/ttyACM0
sleep 1
cd ..
esptool.py --port /dev/ttyACM0 write_flash -fm dio --flash_size=detect 0x00000 esp/.pioenvs/esp12e/firmware.bin
echo "** Remember you have to reset your kit after uploading firmwares!!! **"
