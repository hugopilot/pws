# Written by Hugo Woesthuis

echo "4" > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio4/direction
echo "1" > /sys/class/gpio/gpio4/value
echo "0" > /sys/class/gpio/gpio4/value
echo "4" > /sys/class/gpio/unexport
