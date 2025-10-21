@echo off
set MAX_W=155
set MAX_H=105
set /a counter=3

for %%f in (%*) do (
  magick convert "%%f" -resize %MAX_W%x%MAX_H% "b!counter!.png"
  echo Add to convimg.yaml:
  echo   - name: b!counter!
  echo     palette: global_palette
  echo     images:
  echo       - b!counter!.png
  echo.
  echo Then run convimg.
  echo Add to C code: #include "b!counter!.h"
  echo Add to sprites array: , b!counter!
  echo Update num_sprites accordingly.
  set /a counter+=1
)