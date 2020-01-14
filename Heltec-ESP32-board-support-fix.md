# Heltec ESP32 Board Support Fix
**Temporary MUST DO**
We are currently waiting for this fix to get released in the board support packages, but for now you must follow the instructions below in order to successfully build.
#### Modify the platform.txt file  
Open platform.txt with a text editor:

linux: /home/{user}/.arduino15/packages/Heltec-esp32/hardware/esp32/0.0.4/platform.txt
windows: C:/users/{user}/AppData/Local/Arduino15/packages/Heltec-esp32/hardware/esp32/0.0.4/platform.txt
mac os: /Users/{user}/Library/Arduino15/packages/Heltec-esp32/hardware/esp32/0.0.4/platform.txt

Find the following block:
```
compiler.c.extra_flags=
compiler.c.elf.extra_flags=
compiler.S.extra_flags=
compiler.cpp.extra_flags=
compiler.ar.extra_flags=
compiler.objcopy.eep.extra_flags=
compiler.elf2hex.extra_flags=
```
add this to the end of it:
```
compiler.libraries.ldflags=
```
Next, find this line:

```
## Combine gc-sections, archives, and objects
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} {compiler.c.elf.extra_flags} -Wl,--start-group {object_files} "{archive_file_path}" {compiler.c.elf.libs} -Wl,--end-group -Wl,-EL -o "{build.path}/{build.project_name}.elf"
```

and add `{compiler.libraries.ldflags}` in the exact position below:
```
## Combine gc-sections, archives, and objects
recipe.c.combine.pattern="{compiler.path}{compiler.c.elf.cmd}" {compiler.c.elf.flags} {compiler.c.elf.extra_flags} -Wl,--start-group {object_files} {compiler.libraries.ldflags} "{archive_file_path}" {compiler.c.elf.libs} -Wl,--end-group -Wl,-EL -o "{build.path}/{build.project_name}.elf"
```

Save the file and you are done!