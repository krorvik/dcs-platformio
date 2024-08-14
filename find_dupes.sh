find . | grep main.cpp | grep -v .pio | grep -v template | xargs grep DCSBIOS_RS485_SLAVE
find . | grep main.cpp | grep -v .pio | grep -v template | xargs grep DCSBIOS_RS485_SLAVE | awk '{print $3}' | sort
