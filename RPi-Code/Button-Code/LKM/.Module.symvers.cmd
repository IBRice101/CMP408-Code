cmd_/home/pi/CMP408-Code/RPi-Code/Button-Code/LKM/Module.symvers := sed 's/\.ko$$/\.o/' /home/pi/CMP408-Code/RPi-Code/Button-Code/LKM/modules.order | scripts/mod/modpost -m -a  -o /home/pi/CMP408-Code/RPi-Code/Button-Code/LKM/Module.symvers -e -i Module.symvers   -T -
