# inculog
Raspberry Pi Software to connect to an incubator device through RS232 and to an external sensor PCB via CAN.
All data measured by the sensors in the incubator and the external PCB are collected and transmitted
to an influxDB database.

# Compiling the Software
## Building for PC
To build the software for your PC just use the CMake-File and your favorite build environment:
```commandline
mkdir build
cd build
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -G "Ninja"
ninja
```


## Crosscompile for the Raspi on PC
If you want to compile the project directly on the Raspi just skip this step and use the method from above!

To use cross compilation install all needed dependencies on the Raspberry Pi:
- Base-devel / build-essentials
- fmt
- clang
- ninja

Copy the whole root folder of the raspberry pi onto `~/sysroot`.

Change the sysroot path in the cmake call to your local sysroot path (needs to be absolute!) and then run:
```commandline
mkdir build_pi
cd build_pi
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake -DCMAKE_SYSROOT=/home/<USERNAME>/sysroot -G "Ninja"
```

# Raspberry Pi Setup
## Installing Linux and kernel update
On the *Raspberry Pi Zero 2 W* is running aarch64 Linux with the raspi kernel.
Just install aarch64 as described in the Archlinuxarm-Wiki. Note: Use the 64-Bit kernel!
https://archlinuxarm.org/platforms/armv8/broadcom/raspberry-pi-zero-2

After that you can boot your pi and update the kernel to the raspi kernel with pacman.
```commandline
pacman -Syyu
```

## Enable UART and CAN
Update the following files `/boot/config.txt` to
```text
# See /boot/overlays/README for all available options
dtoverlay=vc4-kms-v3d
dtoverlay=disable_bt
initramfs initramfs-linux.img followkernel

[pi4]
# Run as fast as firmware / board allows
arm_boost=1

[all]
enable_uart=1
dtparam=spi=on
dtoverlay=mcp251xfd,spi0-0,oscillator=40000000,interrupt=25
```
and the file `/boot/cmdline.txt` to
```text
root=/dev/mmcblk0p2 rw rootwait console=tty1 selinux=0 plymouth.enable=0 smsc95xx.turbo_mode=N dwc_otg.lpm_enable=0
```
*Reboot!*

