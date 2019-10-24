# ST B-L072Z-LRWAN1 Board Support Fix
**Temporary MUST DO**  
We are currently waiting for this fix to get released in the board support packages, but for now you must follow the instructions below in order to successfully build.  
#### Modify the platform.txt file  
Open platform.txt with a text editor:  

linux: /home/{user}/.arduino15/packages/STM32/hardware/stm32/1.7.0/platform.txt  
windows: C:/users/{user}/AppData/Local/Arduino15/packages/STM32/hardware/stm32/1.7.0/platform.txt    
mac os: /Users/{user}/Library/Arduino15/packages/STM32/hardware/stm32/1.7.0/platform.txt

Find the following block:  
```
compiler.S.cmd=arm-none-eabi-gcc  
compiler.c.cmd=arm-none-eabi-gcc  
compiler.cpp.cmd=arm-none-eabi-g++  
compiler.ar.cmd=arm-none-eabi-gcc-ar  
compiler.c.elf.cmd=arm-none-eabi-gcc  
compiler.objcopy.cmd=arm-none-eabi-objcopy  
compiler.elf2hex.cmd=arm-none-eabi-objcopy  
```
add this to the end of it:
```
compiler.libraries.ldflags=
```
Next, find this line:  

```
## Combine gc-sections, archives, and objects
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} "-T{build.variant.path}/{build.ldscript}" "-Wl,-Map,{build.path}/{build.project_name}.map" {compiler.c.elf.extra_flags} {compiler.ldflags} {compiler.arm.cmsis.ldflags} -o "{build.path}/{build.project_name}.elf" "-L{build.path}" -Wl,--start-group {object_files} -Wl,--whole-archive "{archive_file_path}" -Wl,--no-whole-archive -lc -Wl,--end-group -lm -lgcc -lstdc++
```

and add `{compiler.libraries.ldflags}` in the exact position below:
```
## Combine gc-sections, archives, and objects
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} "-T{build.variant.path}/{build.ldscript}" "-Wl,-Map,{build.path}/{build.project_name}.map" {compiler.c.elf.extra_flags} {compiler.ldflags} {compiler.arm.cmsis.ldflags} -o "{build.path}/{build.project_name}.elf" "-L{build.path}" -Wl,--start-group {object_files} {compiler.libraries.ldflags} -Wl,--whole-archive "{archive_file_path}" -Wl,--no-whole-archive -lc -Wl,--end-group -lm -lgcc -lstdc++
```

Save the file and you are done!