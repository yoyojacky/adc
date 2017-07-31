# ADC module manual
## First of all, you need to open your I2C function on your OS.
* sudo raspi-config 
## And then Install i2c-tools package.
* sudo apt-get update 
* sudo apt-get upgrade 
* sudo apt-get -y install i2c-tools
## Then git clone this code and configure your /boot/config.txt file.
* sudo vim.tiny /boot/config.txt 
 dtparam=i2c_arm=on
## Reboot and try again.
