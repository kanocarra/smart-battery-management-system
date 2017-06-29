# smart-battery-management-system
Using machine learning to estimate the state of charge of lithium ion batteries for electric vehicles

Code is for the STM32 F334 module.

## Steps to build Makefile

1. Generate the make file using the STM32 Cube generator for the STM32 F334 chip

2. Install gcc-arm-embedded library
```
brew install Caskroom/cask/gcc-arm-embedded
```

3. Edit the Makefile generated to ensure `BINPATH` points to the location of `gcc-arm-embedded`

4. Run
```make```

5. Install `st-link`
```
brew install stlink
```

6. Add a flash command to Makefile that programs STM32
```
flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash --reset write $< 0x8000000
```

7. Progam the STM32 with
```
make flash 
```
