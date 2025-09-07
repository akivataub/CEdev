# TI-84 Plus CE C Programming Cheat Sheet

## Setup Notes
- **Include Headers**: Use `#include <tice.h>`, `#include <graphx.h>`, etc., for relevant libraries.
- **Compile**: Use a `makefile` with `make` to generate `.8xp` files.
- **Test**: Run in CEmu due to OS 5.8.3 restrictions ([CEmu](https://github.com/CE-Programming/CEmu/releases)).
- **Memory**: Avoid `malloc`/`new`; use static arrays due to 4KiB stack, ~60KiB heap limits.

## GraphX Library (graphx.h)
For drawing graphics, shapes, and sprites on the 320x240 LCD.

| Function | Description | Example |
|----------|-------------|---------|
| `gfx_Begin()` | Initializes GraphX context. | `gfx_Begin();` |
| `gfx_End()` | Ends GraphX, restores LCD. | `gfx_End();` |
| `gfx_SetDrawBuffer()` | Sets drawing to non-visible buffer. | `gfx_SetDrawBuffer();` |
| `gfx_SetDrawScreen()` | Sets drawing to visible screen. | `gfx_SetDrawScreen();` |
| `gfx_BlitScreen()` | Copies screen to buffer. | `gfx_BlitScreen();` |
| `gfx_BlitBuffer()` | Copies buffer to screen. | `gfx_BlitBuffer();` |
| `gfx_FillScreen(color)` | Fills screen with color (0-255). | `gfx_FillScreen(255); // White` |
| `gfx_ZeroScreen()` | Fills screen with black (faster). | `gfx_ZeroScreen();` |
| `gfx_SetPixel(x, y)` | Sets pixel to current color. | `gfx_SetPixel(10, 10);` |
| `gfx_GetPixel(x, y)` | Gets pixel color index. | `uint8_t color = gfx_GetPixel(10, 10);` |
| `gfx_Line(x1, y1, x2, y2)` | Draws clipped line. | `gfx_Line(10, 10, 50, 50);` |
| `gfx_Line_NoClip(x1, y1, x2, y2)` | Draws unclipped line. | `gfx_Line_NoClip(10, 10, 50, 50);` |
| `gfx_HorizLine(x, y, length)` | Draws horizontal line. | `gfx_HorizLine(10, 10, 20);` |
| `gfx_HorizLine_NoClip(x, y, length)` | Draws unclipped horizontal line. | `gfx_HorizLine_NoClip(10, 10, 20);` |
| `gfx_VertLine(x, y, length)` | Draws vertical line. | `gfx_VertLine(10, 10, 20);` |
| `gfx_VertLine_NoClip(x, y, length)` | Draws unclipped vertical line. | `gfx_VertLine_NoClip(10, 10, 20);` |
| `gfx_Rectangle(x, y, w, h)` | Draws rectangle outline. | `gfx_Rectangle(50, 50, 20, 20);` |
| `gfx_Rectangle_NoClip(x, y, w, h)` | Draws unclipped rectangle outline. | `gfx_Rectangle_NoClip(50, 50, 20, 20);` |
| `gfx_FillRectangle(x, y, w, h)` | Draws filled rectangle. | `gfx_FillRectangle(50, 50, 20, 20);` |
| `gfx_FillRectangle_NoClip(x, y, w, h)` | Draws unclipped filled rectangle. | `gfx_FillRectangle_NoClip(50, 50, 20, 20);` |
| `gfx_Circle(x, y, r)` | Draws circle outline. | `gfx_Circle(100, 100, 10);` |
| `gfx_Circle_NoClip(x, y, r)` | Draws unclipped circle outline. | `gfx_Circle_NoClip(100, 100, 10);` |
| `gfx_FillCircle(x, y, r)` | Draws filled circle. | `gfx_FillCircle(100, 100, 10);` |
| `gfx_FillCircle_NoClip(x, y, r)` | Draws unclipped filled circle. | `gfx_FillCircle_NoClip(100, 100, 10);` |
| `gfx_Ellipse(x, y, a, b)` | Draws ellipse outline (max radius 128). | `gfx_Ellipse(100, 100, 10, 5);` |
| `gfx_Ellipse_NoClip(x, y, a, b)` | Draws unclipped ellipse outline. | `gfx_Ellipse_NoClip(100, 100, 10, 5);` |
| `gfx_FillEllipse(x, y, a, b)` | Draws filled ellipse. | `gfx_FillEllipse(100, 100, 10, 5);` |
| `gfx_FillEllipse_NoClip(x, y, a, b)` | Draws unclipped filled ellipse. | `gfx_FillEllipse_NoClip(100, 100, 10, 5);` |
| `gfx_Polygon(points, num_points)` | Draws clipped polygon outline. | `int points[] = {10,10,50,50,10,50}; gfx_Polygon(points, 3);` |
| `gfx_Polygon_NoClip(points, num_points)` | Draws unclipped polygon outline. | `gfx_Polygon_NoClip(points, 3);` |
| `gfx_SetColor(color)` | Sets drawing color (0-255). | `gfx_SetColor(224); // Red` |
| `gfx_SetTransparentColor(color)` | Sets transparent color for sprites. | `gfx_SetTransparentColor(0);` |
| `gfx_SetTextFGColor(color)` | Sets text foreground color. | `gfx_SetTextFGColor(0); // Black` |
| `gfx_SetTextBGColor(color)` | Sets text background color. | `gfx_SetTextBGColor(255); // White` |
| `gfx_PrintString(str)` | Prints string at cursor. | `gfx_PrintString("Hello");` |
| `gfx_PrintStringXY(str, x, y)` | Prints string at (x, y). | `gfx_PrintStringXY("Hello", 10, 10);` |
| `gfx_SetTextXY(x, y)` | Sets text cursor position. | `gfx_SetTextXY(10, 10);` |
| `gfx_SwapDraw()` | Swaps buffer to screen. | `gfx_SwapDraw();` |
| `gfx_AllocSprite(w, h, malloc_routine)` | Allocates sprite memory. | `gfx_sprite_t *sprite = gfx_AllocSprite(16, 16, malloc);` |
| `gfx_MallocSprite(w, h)` | Allocates sprite using malloc. | `gfx_sprite_t *sprite = gfx_MallocSprite(16, 16);` |
| `gfx_UninitedSprite(name, w, h)` | Statically allocates uninitialized sprite. | `gfx_UninitedSprite(mySprite, 16, 16);` |
| `gfx_TempSprite(name, w, h)` | Statically allocates sprite. | `gfx_TempSprite(mySprite, 16, 16);` |
| `gfx_TransparentSprite(sprite, x, y)` | Draws transparent sprite. | `gfx_TransparentSprite(sprite, 50, 50);` |
| `gfx_ScaledSprite(sprite, x, y, scale)` | Draws scaled sprite. | `gfx_ScaledSprite(sprite, 50, 50, 2);` |
| `gfx_ScaledTransparentSprite(sprite, x, y, scale)` | Draws transparent scaled sprite. | `gfx_ScaledTransparentSprite(sprite, 50, 50, 2);` |
| `gfx_SetPalette(index, color)` | Sets palette color at index. | `gfx_SetPalette(16, 0xFFFF);` |
| `gfx_RGBTo1555(r, g, b)` | Converts RGB to 1555 color. | `uint16_t color = gfx_RGBTo1555(255, 0, 0);` |
| `gfx_SetDefaultPalette()` | Sets default xlibc palette. | `gfx_SetDefaultPalette();` |
| `gfx_CheckRectangleHotspot(x1, y1, w1, h1, x2, y2, w2, h2)` | Tests rectangle collision. | `bool hit = gfx_CheckRectangleHotspot(10, 10, 20, 20, 30, 30, 20, 20);` |
| `gfx_GetZX7SpriteWidth(sprite)` | Gets width of ZX7-compressed sprite. | `uint8_t w = gfx_GetZX7SpriteWidth(sprite);` |
| `gfx_GetZX7SpriteHeight(sprite)` | Gets height of ZX7-compressed sprite. | `uint8_t h = gfx_GetZX7SpriteHeight(sprite);` |
| `gfx_GetZX7SpriteSize(sprite)` | Gets size of ZX7-compressed sprite. | `size_t size = gfx_GetZX7SpriteSize(sprite);` |
| `gfx_Tilemap(map, x, y)` | Draws clipped tilemap. | `gfx_Tilemap(&tilemap, 0, 0);` |
| `gfx_Tilemap_NoClip(map, x, y)` | Draws unclipped tilemap. | `gfx_Tilemap_NoClip(&tilemap, 0, 0);` |
| `gfx_TransparentTilemap(map, x, y)` | Draws transparent tilemap. | `gfx_TransparentTilemap(&tilemap, 0, 0);` |
| `gfx_TransparentTilemap_NoClip(map, x, y)` | Draws unclipped transparent tilemap. | `gfx_TransparentTilemap_NoClip(&tilemap, 0, 0);` |
| `gfx_SetTile(x, y, index)` | Sets tile sprite index by pixel offset. | `gfx_SetTile(10, 10, 1);` |
| `gfx_GetTile(x, y)` | Gets tile sprite index by pixel offset. | `uint8_t index = gfx_GetTile(10, 10);` |
| `gfx_SetTileMapped(x, y, index)` | Sets tile sprite index by array coordinates. | `gfx_SetTileMapped(1, 1, 1);` |
| `gfx_GetTileMapped(x, y)` | Gets tile sprite index by array coordinates. | `uint8_t index = gfx_GetTileMapped(1, 1);` |
| `gfx_TilePtr(x, y)` | Gets sprite pointer by pixel offset. | `gfx_sprite_t *ptr = gfx_TilePtr(10, 10);` |
| `gfx_TilePtrMapped(x, y)` | Gets sprite pointer by array coordinates. | `gfx_sprite_t *ptr = gfx_TilePtrMapped(1, 1);` |
| `gfx_ConvertMallocRLETSprite(sprite)` | Converts sprite to RLE transparency. | `gfx_rletsprite_t *rle = gfx_ConvertMallocRLETSprite(sprite);` |
| `gfx_RotateSprite(sprite_in, sprite_out)` | Rotates sprite (in != out). | `gfx_RotateSprite(sprite_in, sprite_out);` |
| `gfx_FlipSpriteX(sprite_in, sprite_out)` | Flips sprite horizontally. | `gfx_FlipSpriteX(sprite_in, sprite_out);` |
| `gfx_FlipSpriteY(sprite_in, sprite_out)` | Flips sprite vertically. | `gfx_FlipSpriteY(sprite_in, sprite_out);` |
| `gfx_RotateSpriteC(sprite_in, sprite_out)` | Rotates sprite clockwise 90°. | `gfx_RotateSpriteC(sprite_in, sprite_out);` |
| `gfx_RotateSpriteCC(sprite_in, sprite_out)` | Rotates sprite counterclockwise 90°. | `gfx_RotateSpriteCC(sprite_in, sprite_out);` |
| `gfx_ShiftDown(n)` | Shifts screen contents down by n pixels. | `gfx_ShiftDown(10);` |
| `gfx_ShiftUp(n)` | Shifts screen contents up by n pixels. | `gfx_ShiftUp(10);` |
| `gfx_ShiftLeft(n)` | Shifts screen contents left by n pixels. | `gfx_ShiftLeft(10);` |
| `gfx_ShiftRight(n)` | Shifts screen contents right by n pixels. | `gfx_ShiftRight(10);` |
| `gfx_SetClipRegion(x_min, y_min, x_max, y_max)` | Sets clipping region for drawing. | `gfx_SetClipRegion(10, 10, 300, 200);` |
| `gfx_SetTextScale(w_scale, h_scale)` | Sets text scaling factors. | `gfx_SetTextScale(2, 2);` |
| `gfx_SetFontHeight(height)` | Sets font height for text. | `gfx_SetFontHeight(8);` |
| `gfx_SetFontData(font_data)` | Sets custom font data. | `gfx_SetFontData(myFont);` |
| `gfx_SetMonoSpaceFont(spacing)` | Sets monospace font spacing. | `gfx_SetMonoSpaceFont(8);` |

## KeypadC Library (keypadc.h)
For reading keypad input, supporting multi-key presses.

| Function/Constant | Description | Example |
|-------------------|-------------|---------|
| `kb_Scan()` | Updates keypad data registers. | `kb_Scan();` |
| `kb_AnyKey()` | True if any key is pressed. | `if (kb_AnyKey()) {...}` |
| `kb_IsDown(key)` | Checks if key is pressed (kb_lkey_t). | `if (kb_IsDown(kb_Enter)) {...}` |
| `kb_Data[group]` | Access key states (array). | `if (kb_Data[6] & kb_Enter) {...}` |
| `kb_SetMode(mode)` | Sets scanning mode (e.g., MODE_3_CONTINUOUS). | `kb_SetMode(MODE_3_CONTINUOUS);` |
| `kb_GetMode()` | Gets current scanning mode. | `kb_scan_mode_t mode = kb_GetMode();` |
| `kb_EnableInt()` | Enables keypad interrupts. | `kb_EnableInt();` |
| `kb_IntAcknowledge()` | Acknowledges keypad interrupts. | `kb_IntAcknowledge();` |
| `kb_IntStatus()` | Gets interrupt status. | `kb_int_signal_t status = kb_IntStatus();` |
| `kb_Config()` | Configures keypad controller. | `kb_Config();` |
| `kb_EnableOnLatch()` | Enables ON key latching. | `kb_EnableOnLatch();` |
| `kb_DisableOnLatch()` | Disables ON key latching. | `kb_DisableOnLatch();` |
| `kb_ClearOnLatch()` | Resets ON key latch. | `kb_ClearOnLatch();` |
| `kb_ScanGroup(row)` | Scans specific keyboard row. | `uint8_t row = kb_ScanGroup(1);` |
| `kb_Reset()` | Resets keypad before OS return. | `kb_Reset();` |
| `kb_On` | Key constant for [ON] key. | `if (kb_IsDown(kb_On)) {...}` |
| `kb_KeyGroup1` | Key group 1 (e.g., arrow keys). | `if (kb_Data[1] & kb_Up) {...}` |
| `kb_KeyGroup2` | Key group 2 (e.g., number keys). | `if (kb_Data[2] & kb_2) {...}` |
| `kb_KeyGroup3` | Key group 3 (e.g., math operations). | `if (kb_Data[3] & kb_Add) {...}` |
| `kb_KeyGroup4` | Key group 4 (e.g., function keys). | `if (kb_Data[4] & kb_Yequ) {...}` |
| `kb_KeyGroup5` | Key group 5 (e.g., alpha, 2nd). | `if (kb_Data[5] & kb_Alpha) {...}` |
| `kb_KeyGroup6` | Key group 6 (e.g., enter, clear). | `if (kb_Data[6] & kb_Enter) {...}` |
| `kb_KeyGroup7` | Key group 7 (e.g., power, mode). | `if (kb_Data[7] & kb_Power) {...}` |
| `kb_SetIntHandler(handler)` | Sets custom keypad interrupt handler. | `kb_SetIntHandler(myHandler);` |
| `kb_DisableIntHandler()` | Disables keypad interrupt handler. | `kb_DisableIntHandler();` |

## FileIoC Library (fileioc.h)
For saving/loading data using AppVars.

| Function | Description | Example |
|----------|-------------|---------|
| `ti_Open(name, mode)` | Opens AppVar ("r", "w", "a"). | `ti_var_t file = ti_Open("SAVE", "w");` |
| `ti_OpenVar(name, mode, type)` | Opens variable with type. | `ti_var_t file = ti_OpenVar("LIST", "w", TI_LIST_TYPE);` |
| `ti_Close(handle)` | Closes AppVar/variable. | `ti_Close(file);` |
| `ti_Write(data, size, count, handle)` | Writes data to AppVar. | `ti_Write(&score, sizeof(int), 1, file);` |
| `ti_Read(data, size, count, handle)` | Reads data from AppVar. | `ti_Read(&score, sizeof(int), 1, file);` |
| `ti_PutC(ch, handle)` | Writes character to AppVar. | `ti_PutC('A', file);` |
| `ti_GetC(handle)` | Reads character from AppVar. | `char ch = ti_GetC(file);` |
| `ti_Seek(offset, origin, handle)` | Seeks to offset in AppVar. | `ti_Seek(0, SEEK_SET, file);` |
| `ti_Rewind(handle)` | Seeks to start of AppVar data. | `ti_Rewind(file);` |
| `ti_Tell(handle)` | Gets current offset. | `size_t offset = ti_Tell(file);` |
| `ti_GetSize(handle)` | Gets AppVar size in bytes. | `size_t size = ti_GetSize(file);` |
| `ti_Resize(size, handle)` | Resizes AppVar. | `ti_Resize(100, file);` |
| `ti_IsArchived(handle)` | Checks if AppVar is archived. | `bool archived = ti_IsArchived(file);` |
| `ti_SetArchiveStatus(archive, handle)` | Archives/unarchives AppVar. | `ti_SetArchiveStatus(true, file);` |
| `ti_Delete(name)` | Deletes AppVar. | `ti_Delete("SAVE");` |
| `ti_DeleteVar(name, type)` | Deletes variable. | `ti_DeleteVar("LIST", TI_LIST_TYPE);` |
| `ti_Detect(vat_ptr, detect_string)` | Locates AppVars in RAM/archive. | `void *ptr = NULL; ti_Detect(&ptr, "SAVE");` |
| `ti_DetectVar(vat_ptr, detect_string, var_type)` | Locates variables by type. | `ti_DetectVar(&ptr, "LIST", TI_LIST_TYPE);` |
| `ti_DetectAny(vat_ptr, detect_string, var_type)` | Locates any variables. | `ti_DetectAny(&ptr, NULL, NULL);` |
| `ti_GetTokenString(read_ptr, token_len, str_len)` | Gets TI token string. | `char *str; ti_GetTokenString(&ptr, &len, &slen);` |
| `ti_GetDataPtr(handle)` | Gets data pointer at offset. | `void *data = ti_GetDataPtr(file);` |
| `ti_GetVATPtr(handle)` | Gets VAT entry pointer. | `void *vat = ti_GetVATPtr(file);` |
| `ti_GetName(name, handle)` | Gets AppVar name. | `char name[9]; ti_GetName(name, file);` |
| `ti_Rename(old_name, new_name)` | Renames AppVar. | `ti_Rename("SAVE", "BACKUP");` |
| `ti_RenameVar(old_name, new_name, type)` | Renames variable. | `ti_RenameVar("LIST", "NEWLIST", TI_LIST_TYPE);` |
| `ti_SetVar(type, name, data)` | Stores data to OS variable. | `ti_SetVar(TI_REAL_TYPE, "NUM", &real);` |
| `ti_StoVar(type_to, to, type_from, from)` | Stores variable to another. | `ti_StoVar(TI_LIST_TYPE, "L1", TI_LIST_TYPE, "L2");` |
| `ti_RclVar(type, name, data)` | Recalls OS variable. | `ti_RclVar(TI_REAL_TYPE, "NUM", &real);` |
| `ti_ArchiveHasRoom(num_bytes)` | Checks archive space. | `bool room = ti_ArchiveHasRoom(100);` |
| `ti_ArchiveHasRoomVar(handle)` | Checks archive space for AppVar. | `bool room = ti_ArchiveHasRoomVar(file);` |
| `ti_SetGCBehavior(before, after)` | Sets garbage collect routines. | `ti_SetGCBehavior(before_fn, after_fn);` |
| `ti_MallocReal()` | Allocates real_t variable. | `real_t *var = ti_MallocReal();` |
| `ti_MallocCplx()` | Allocates cplx_t variable. | `cplx_t *var = ti_MallocCplx();` |
| `ti_MallocString(len)` | Allocates string_t variable. | `string_t *str = ti_MallocString(10);` |
| `ti_MallocList(dim)` | Allocates list_t variable. | `list_t *list = ti_MallocList(5);` |
| `ti_MallocMatrix(rows, cols)` | Allocates matrix_t variable. | `matrix_t *mat = ti_MallocMatrix(2, 3);` |
| `ti_MallocCplxList(dim)` | Allocates cplx_list_t variable. | `cplx_list_t *list = ti_MallocCplxList(5);` |
| `ti_MallocEqu(len)` | Allocates equ_t variable. | `equ_t *eq = ti_MallocEqu(10);` |
| `ti_GetFreeMem()` | Gets free RAM in bytes. | `size_t free = ti_GetFreeMem();` |
| `ti_GetFreeArchive()` | Gets free archive memory in bytes. | `size_t free = ti_GetFreeArchive();` |
| `ti_SetVarData(type, name, data, size)` | Stores data to OS variable with size. | `ti_SetVarData(TI_STRING_TYPE, "STR", str, strlen(str));` |
| `ti_GetVarData(type, name, data, size)` | Recalls data from OS variable. | `ti_GetVarData(TI_STRING_TYPE, "STR", str, &size);` |

## FontLibC Library (fontlibc.h)
For rendering custom fonts and text layout.

| Function | Description | Example |
|----------|-------------|---------|
| `fontlib_SetWindowFullScreen()` | Sets text window to full screen. | `fontlib_SetWindowFullScreen();` |
| `fontlib_SetWindow(x_min, y_min, w, h)` | Sets text window bounds. | `fontlib_SetWindow(10, 10, 100, 50);` |
| `fontlib_GetWindowXMin()` | Gets window start column. | `uint16_t x = fontlib_GetWindowXMin();` |
| `fontlib_GetWindowYMin()` | Gets window start row. | `uint16_t y = fontlib_GetWindowYMin();` |
| `fontlib_GetWindowWidth()` | Gets window width. | `uint16_t w = fontlib_GetWindowWidth();` |
| `fontlib_GetWindowHeight()` | Gets window height. | `uint16_t h = fontlib_GetWindowHeight();` |
| `fontlib_SetCursorPosition(x, y)` | Sets cursor position. | `fontlib_SetCursorPosition(10, 10);` |
| `fontlib_GetCursorX()` | Gets cursor column. | `uint16_t x = fontlib_GetCursorX();` |
| `fontlib_GetCursorY()` | Gets cursor row. | `uint16_t y = fontlib_GetCursorY();` |
| `fontlib_ShiftCursorPosition(x, y)` | Adds offset to cursor. | `fontlib_ShiftCursorPosition(5, 5);` |
| `fontlib_HomeUp()` | Moves cursor to window top-left. | `fontlib_HomeUp();` |
| `fontlib_Home()` | Moves cursor to line start. | `fontlib_Home();` |
| `fontlib_SetFont(font_data, flags)` | Sets current font. | `fontlib_SetFont(myFont, 0);` |
| `fontlib_SetForegroundColor(color)` | Sets text color. | `fontlib_SetForegroundColor(0);` |
| `fontlib_SetBackgroundColor(color)` | Sets background color. | `fontlib_SetBackgroundColor(255);` |
| `fontlib_SetColors(forecolor, backcolor)` | Sets both colors. | `fontlib_SetColors(0, 255);` |
| `fontlib_GetForegroundColor()` | Gets foreground color. | `uint8_t color = fontlib_GetForegroundColor();` |
| `fontlib_GetBackgroundColor()` | Gets background color. | `uint8_t color = fontlib_GetBackgroundColor();` |
| `fontlib_SetTransparency(trans)` | Sets transparent background. | `fontlib_SetTransparency(true);` |
| `fontlib_GetTransparency()` | Gets transparency state. | `bool trans = fontlib_GetTransparency();` |
| `fontlib_SetLineSpacing(above, below)` | Sets line spacing. | `fontlib_SetLineSpacing(2, 2);` |
| `fontlib_GetSpaceAbove()` | Gets padding above. | `uint8_t space = fontlib_GetSpaceAbove();` |
| `fontlib_GetSpaceBelow()` | Gets padding below. | `uint8_t space = fontlib_GetSpaceBelow();` |
| `fontlib_SetItalicSpacingAdjustment(adj)` | Sets italic spacing adjustment. | `fontlib_SetItalicSpacingAdjustment(1);` |
| `fontlib_GetItalicSpacingAdjustment()` | Gets italic spacing adjustment. | `uint8_t adj = fontlib_GetItalicSpacingAdjustment();` |
| `fontlib_GetCurrentFontHeight()` | Gets font height with spacing. | `uint8_t h = fontlib_GetCurrentFontHeight();` |
| `fontlib_ValidateCodePoint(code_point)` | Tests if font has code point. | `bool valid = fontlib_ValidateCodePoint('A');` |
| `fontlib_GetTotalGlyphs()` | Gets number of glyphs. | `uint16_t count = fontlib_GetTotalGlyphs();` |
| `fontlib_GetFirstGlyph()` | Gets first glyph code point. | `uint24_t cp = fontlib_GetFirstGlyph();` |
| `fontlib_SetNewlineCode(code_point)` | Sets newline code point. | `fontlib_SetNewlineCode('\n');` |
| `fontlib_GetNewlineCode()` | Gets newline code point. | `uint24_t cp = fontlib_GetNewlineCode();` |
| `fontlib_SetAlternateStopCode(code_point)` | Sets alternate stop code. | `fontlib_SetAlternateStopCode(' ');` |
| `fontlib_GetAlternateStopCode()` | Gets alternate stop code. | `uint24_t cp = fontlib_GetAlternateStopCode();` |
| `fontlib_SetFirstPrintableCodePoint(cp)` | Sets first printable code point. | `fontlib_SetFirstPrintableCodePoint(32);` |
| `fontlib_GetFirstPrintableCodePoint()` | Gets first printable code point. | `uint24_t cp = fontlib_GetFirstPrintableCodePoint();` |
| `fontlib_SetDrawIntCodePoints(minus, zero)` | Sets code points for DrawInt. | `fontlib_SetDrawIntCodePoints('-', '0');` |
| `fontlib_GetDrawIntMinus()` | Gets minus code point for DrawInt. | `uint24_t cp = fontlib_GetDrawIntMinus();` |
| `fontlib_GetDrawIntZero()` | Gets zero code point for DrawInt. | `uint24_t cp = fontlib_GetDrawIntZero();` |
| `fontlib_GetGlyphWidth(codepoint)` | Gets glyph width. | `uint8_t w = fontlib_GetGlyphWidth('A');` |
| `fontlib_GetStringWidth(str)` | Gets string width. | `unsigned int w = fontlib_GetStringWidth("Score");` |
| `fontlib_GetStringWidthL(str, max_chars)` | Gets string width up to max chars. | `unsigned int w = fontlib_GetStringWidthL("Score", 3);` |
| `fontlib_GetLastCharacterRead()` | Gets last character processed. | `char *last = fontlib_GetLastCharacterRead();` |
| `fontlib_GetCharactersRemaining()` | Checks if max_chars reached. | `unsigned int rem = fontlib_GetCharactersRemaining();` |
| `fontlib_DrawGlyph(glyph)` | Draws single glyph. | `uint16_t x = fontlib_DrawGlyph('A');` |
| `fontlib_DrawString(str)` | Draws string at cursor. | `uint16_t x = fontlib_DrawString("Score");` |
| `fontlib_DrawStringL(str, max_chars)` | Draws string up to max characters. | `fontlib_DrawStringL("Score", 3);` |
| `fontlib_DrawInt(value)` | Draws integer at cursor. | `fontlib_DrawInt(123);` |
| `fontlib_DrawUInt(value)` | Draws unsigned integer at cursor. | `fontlib_DrawUInt(123);` |
| `fontlib_SetNewlineOptions(options)` | Sets newline behavior flags. | `fontlib_SetNewlineOptions(FONTLIB_AUTO_WRAP);` |
| `fontlib_GetNewlineOptions()` | Gets newline behavior flags. | `uint8_t opts = fontlib_GetNewlineOptions();` |
| `fontlib_SetAlignment(align)` | Sets text alignment (e.g., left, center). | `fontlib_SetAlignment(FONTLIB_ALIGN_CENTER);` |

## System Utilities (tice.h, sys/util.h)
For system operations and utilities.

| Function | Description | Example |
|----------|-------------|---------|
| `os_GetCSC()` | Scans for keypress, returns code. | `while (!os_GetCSC());` |
| `randInt(min, max)` | Generates random integer. | `int n = randInt(1, 100);` |
| `srandom(seed)` | Seeds random number generator. | `srandom(1);` |
| `delay(milliseconds)` | Delays execution. | `delay(1000); // 1 second` |
| `boot_GetBatteryStatus()` | Gets battery level (0–4). | `uint8_t level = boot_GetBatteryStatus();` |
| `boot_SetBatteryStatus(level)` | Sets battery status (for testing). | `boot_SetBatteryStatus(4);` |
| `rtc_GetTime()` | Gets RTC time as seconds since epoch. | `uint32_t time = rtc_GetTime();` |
| `rtc_SetTime(seconds)` | Sets RTC time in seconds since epoch. | `rtc_SetTime(1633036800);` |
| `os_GetSystemInfo(info)` | Gets system info (e.g., OS version). | `os_system_info_t info; os_GetSystemInfo(&info);` |

## Key Constants
### Number Keys
- `sk_0`
- `sk_1`
- `sk_2`
- `sk_3`
- `sk_4`
- `sk_5`
- `sk_6`
- `sk_7`
- `sk_8`
- `sk_9`

### Math & Operation Keys
- `sk_Add` // [+]
- `sk_Sub` // [−]
- `sk_Mul` // [×]
- `sk_Div` // [÷]
- `sk_Enter` // [ENTER]
- `sk_DecPnt` // [ . ]
- `sk_Neg` // [(-)]
- `sk_Recip` // [1/x]
- `sk_Exp` // [^]
- `sk_Store` // [STO→]
- `sk_Comma`
- `sk_LParen`
- `sk_RParen`

### Arrow Keys
- `sk_Left`
- `sk_Right`
- `sk_Up`
- `sk_Down`

### Alpha & 2nd
- `sk_Alpha`
- `sk_2nd`

### Other Function Keys
- `sk_Mode`
- `sk_Del`
- `sk_Clear`
- `sk_Stat`
- `sk_Math`
- `sk_Apps`
- `sk_Prgm`
- `sk_Vars`
- `sk_Yequ` // [Y=]
- `sk_Window`
- `sk_Zoom`
- `sk_Trace`
- `sk_Graph`
- `sk_Table`
- `sk_Power` // [ON]
- `sk_Sin`
- `sk_Cos`
- `sk_Tan`
- `sk_Log`
- `sk_Ln`
- `sk_XTThetaN` // [X,T,θ,n]
- `sk_Square` // [x²]
- `sk_Ans` // [ANS]
- `sk_Colon` // [:]
- `sk_Question` // [?]
- `sk_Quote` // ["]