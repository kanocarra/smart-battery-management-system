# smart-battery-management-system
Using machine learning to estimate the state of charge of lithium ion batteries for electric vehicles

Code is for the STM32 F334 module.

Programming has only been tested in Mac OSX. It will mostly likely work on Linux with no problems (yay for Unix based OS) but sorry Windows people, you're on your own.

## Steps to build Makefile

1. Generate the make file using the STM32 Cube generator for the STM32 F334 chip

2. Install gcc-arm-embedded library
```
brew install Caskroom/cask/gcc-arm-embedded
```

3. Edit the Makefile generated to ensure `BINPATH` points to the location of `gcc-arm-embedded`

4. Compile the files with
```
make
```

5. Install `st-link`
```
brew install stlink
```

6. Add a flash command to Makefile that programs STM32
```
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash --reset write $< 0x8000000
```

7. Program the STM32 with
```
make flash 
```
If all goes well then everything is set up correctly!


## To program STM32f334

1. First clean up any old build files with
```
make clean
```
2. Then run the build 
```
make
```

3. Then you can program the chip using
```
make flash
```

Then all should work, yay!
