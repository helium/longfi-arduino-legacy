# Catena 4610 Board Support Fix
**Temporary MUST DO**  
We are currently waiting for this fix to get released in the board support packages, but for now you must follow the instructions below in order to successfully build.  
#### Modify the platform.txt file  
Open platform.txt with a text editor:  

linux: /home/{user}/.arduino15/packages/mcci/hardware/stm32/2.5.0/platform.txt  
windows: C:/users/{user}/AppData/Local/Arduino15/packages/mcci/hardware/stm32/2.5.0/platform.txt    
mac os: /Users/{user}/Library/Arduino15/packages/mcci/hardware/stm32/2.5.0/platform.txt

Find the following block:  

```
compiler.S.cmd=arm-none-eabi-gcc
compiler.c.cmd=arm-none-eabi-gcc
compiler.cpp.cmd=arm-none-eabi-g++
compiler.ar.cmd=arm-none-eabi-gcc-ar
compiler.c.elf.cmd=arm-none-eabi-gcc
compiler.objcopy.cmd=arm-none-eabi-objcopy
compiler.elf2bin.flags=-O binary
compiler.elf2hex.flags=-O ihex
compiler.elf2hex.bin.flags=-O binary
compiler.elf2hex.hex.flags=-O ihex
compiler.elf2hex.cmd=arm-none-eabi-objcopy
compiler.suffix.cmd=dfu-suffix
compiler.suffix.cmd.windows=dfu-suffix.exe
```
add this to the end of it:
```
compiler.libraries.ldflags=
```
Next, find this line:  

```
## Combine gc-sections, archives, and objects
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} "-T{build.variant.path}/{build.ldscript}" "-Wl,-Map,{build.path}/{build.project_name}.map" {compiler.ldflags} {compiler.arm.cmsis.ldflags} -o "{build.path}/{build.project_name}.elf" "-L{build.path}" -Wl,--start-group {object_files} -Wl,--whole-archive "{archive_file_path}" -Wl,--no-whole-archive {compiler.c.elf.extra_flags} -lc -Wl,--end-group -lm -lgcc -lstdc++ --specs=nano.specs
```

and add `{compiler.libraries.ldflags}` in the exact position below:
```
## Combine gc-sections, archives, and objects
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} "-T{build.variant.path}/{build.ldscript}" "-Wl,-Map,{build.path}/{build.project_name}.map" {compiler.ldflags} {compiler.arm.cmsis.ldflags} -o "{build.path}/{build.project_name}.elf" "-L{build.path}" -Wl,--start-group {object_files} {compiler.libraries.ldflags} -Wl,--whole-archive "{archive_file_path}" -Wl,--no-whole-archive {compiler.c.elf.extra_flags} -lc -Wl,--end-group -lm -lgcc -lstdc++ --specs=nano.specs
```

Save the file and you are done!