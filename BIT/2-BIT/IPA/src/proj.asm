; Autor: Daniel Klimaj; xklima22@stud.fit.vutbr.cz
; Subor: proj.asm
; Popis: Jednoduchy textovy editor v assembleri
; Datum: 2014-12-15

bits 32

;*******************************************************************************
%include "inc\win32.inc"
%include "inc\general.inc"

;*******************************************************************************
dllimport GetModuleHandle, kernel32.dll, GetModuleHandleA
dllimport GetCommandLine, kernel32.dll, GetCommandLineA
dllimport ExitProcess, kernel32.dll
dllimport ShowWindow, user32.dll
dllimport UpdateWindow, user32.dll
dllimport TranslateMessage, user32.dll
dllimport RegisterClassEx, user32.dll, RegisterClassExA
dllimport LoadIcon, user32.dll, LoadIconA
dllimport LoadCursor, user32.dll, LoadCursorA
dllimport CreateWindowEx, user32.dll, CreateWindowExA
dllimport GetMessage, user32.dll, GetMessageA
dllimport PeekMessage, user32.dll, PeekMessageA
dllimport DispatchMessage, user32.dll, DispatchMessageA
dllimport PostQuitMessage, user32.dll
dllimport MessageBox, user32.dll, MessageBoxA
dllimport DefWindowProc, user32.dll, DefWindowProcA
dllimport SendMessage, user32.dll, SendMessageA
dllimport AppendMenu, user32.dll, AppendMenuA
dllimport CreateMenu, user32.dll
dllimport SetMenu, user32.dll
dllimport GetClientRect, user32.dll
dllimport SetWindowPos, user32.dll
dllimport SetDlgItemText, user32.dll, SetDlgItemTextA
dllimport GetOpenFileName, comdlg32.dll, GetOpenFileNameA
dllimport GetSaveFileName, comdlg32.dll, GetSaveFileNameA
dllimport CreateFile, kernel32.dll, CreateFileA
dllimport GetFileSize, kernel32.dll
dllimport GlobalAlloc, kernel32.dll
dllimport GlobalFree, kernel32.dll
dllimport ReadFile, kernel32.dll
dllimport CloseHandle, kernel32.dll
dllimport GetWindowTextLength, user32.dll, GetWindowTextLengthA
dllimport GetWindowText, user32.dll, GetWindowTextA
dllimport WriteFile, kernel32.dll

;*******************************************************************************
[section .data class=DATA use32 align=16]

;Retazce
string emptyStr,     ""
string mwClassName,  "MainWindow"
string mwTitle,      "ASM Editor"
string ewClassName,  "Edit"
string menuNew,      "New"
string menuOpen,     "Open"
string menuSave,     "Save"
string menuQuit,     "Quit"
string menuFile,     "File"
string nameSave,     ""
string nameOpen,     ""

;OpenFileName
string filter, "Text Files (*.txt)",0,"*.txt",0,"All Files (*.*)",0,"*.*",0
string defExt, "txt"

;Hlavne okno
mwHInstance     dd 0
mwHandle        dd 0
mwWidth         dd 480
mwHeight        dd 320
mwMenubar       dd 0
mwMenu          dd 0

;Editor
ewHandle        dd 0
ewHInstance     dd 0

;Otvaranie a zapisovanie z/do suboru
fileOpen        dd 0
sizeOpen        dd 0
strOpen:        resb 1048576 ;10 MB max
readOpen        dd 0
fileSave        dd 0
sizeSave        dd 0
strSave:        resb 1048576 ;10 MB max
readSave        dd 0

;IDcka akcii a widgetov
IDC_MAIN_EDIT   dd 101
ID_FILE_QUIT    dd 40001
ID_FILE_OPEN    dd 40002
ID_FILE_SAVE    dd 40003
ID_FILE_NEW     dd 40004

mwMessage:      resb MSG_size
fileName:       resb 250

;*******************************************************************************
; TRIEDY
;*******************************************************************************
MainWinClass:
  istruc WNDCLASSEX
    at WNDCLASSEX.cbSize,                 dd  WNDCLASSEX_size
    at WNDCLASSEX.style,                  dd  CS_VREDRAW + CS_HREDRAW
    at WNDCLASSEX.lpfnWndProc,            dd  WndProc
    at WNDCLASSEX.cbClsExtra,             dd  0
    at WNDCLASSEX.cbWndExtra,             dd  0
    at WNDCLASSEX.hInstance,              dd  NULL
    at WNDCLASSEX.hIcon,                  dd  NULL
    at WNDCLASSEX.hCursor,                dd  NULL
    at WNDCLASSEX.hbrBackground,          dd  COLOR_3DFACE
    at WNDCLASSEX.lpszMenuName,           dd  NULL
    at WNDCLASSEX.lpszClassName,          dd  mwClassName
    at WNDCLASSEX.hIconSm,                dd  NULL
  iend

MainWinRect:
  istruc RECT
    at RECT.left,                         dd 0
    at RECT.top,                          dd 0
    at RECT.right,                        dd 0
    at RECT.bottom,                       dd 0
  iend

ofnOpen:
  istruc OPENFILENAME
    at OPENFILENAME.lStructSize,          dd OPENFILENAME_size
    at OPENFILENAME.hWndOwner,            dd 0
    at OPENFILENAME.hInstance,            dd 0
    at OPENFILENAME.lpstrFilter,          dd filter
    at OPENFILENAME.lpstrCustomFilter,    dd 0
    at OPENFILENAME.nMaxCustFilter,       dd 0
    at OPENFILENAME.nFilterIndex,         dd 0
    at OPENFILENAME.lpstrFile,            dd nameOpen
    at OPENFILENAME.nMaxFile,             dd 250
    at OPENFILENAME.lpstrFileTitle,       dd 0
    at OPENFILENAME.nMaxFileTitle,        dd 0
    at OPENFILENAME.lpstrInitialDir,      dd 0
    at OPENFILENAME.lpstrTitle,           dd 0
    at OPENFILENAME.Flags,                dd OFN_EXPLORER + \
      OFN_FILEMUSTEXIST + OFN_HIDEREADONLY
    at OPENFILENAME.nFileOffset,          dw 0
    at OPENFILENAME.nFileExtension,       dw 0
    at OPENFILENAME.lpstrDefExt,          dd defExt
    at OPENFILENAME.lCustData,            dd 0
    at OPENFILENAME.lpfnHook,             dd 0
    at OPENFILENAME.lpTemplateName,       dd 0
  iend

ofnSave:
  istruc OPENFILENAME
    at OPENFILENAME.lStructSize,          dd OPENFILENAME_size
    at OPENFILENAME.hWndOwner,            dd 0
    at OPENFILENAME.hInstance,            dd 0
    at OPENFILENAME.lpstrFilter,          dd filter
    at OPENFILENAME.lpstrCustomFilter,    dd 0
    at OPENFILENAME.nMaxCustFilter,       dd 0
    at OPENFILENAME.nFilterIndex,         dd 0
    at OPENFILENAME.lpstrFile,            dd nameSave
    at OPENFILENAME.nMaxFile,             dd 250
    at OPENFILENAME.lpstrFileTitle,       dd 0
    at OPENFILENAME.nMaxFileTitle,        dd 0
    at OPENFILENAME.lpstrInitialDir,      dd 0
    at OPENFILENAME.lpstrTitle,           dd 0
    at OPENFILENAME.Flags,                dd OFN_EXPLORER + \
      OFN_PATHMUSTEXIST + OFN_HIDEREADONLY + OFN_OVERWRITEPROMPT
    at OPENFILENAME.nFileOffset,          dw 0
    at OPENFILENAME.nFileExtension,       dw 0
    at OPENFILENAME.lpstrDefExt,          dd defExt
    at OPENFILENAME.lCustData,            dd 0
    at OPENFILENAME.lpfnHook,             dd 0
    at OPENFILENAME.lpTemplateName,       dd 0
  iend

;*******************************************************************************
[section .code use32 class=CODE]

..start:

  invoke GetModuleHandle, NULL
  mov [mwHInstance], eax
  mov [MainWinClass + WNDCLASSEX.hInstance], eax

  invoke RegisterClassEx, MainWinClass
  test eax, eax
  jz near .end

  invoke CreateWindowEx, 0, mwClassName, mwTitle, \
    WS_OVERLAPPEDWINDOW + WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, \
    [mwWidth], [mwHeight], NULL, NULL, [mwHInstance], NULL
  test eax, eax
  jz near .end

  mov [mwHandle], eax
  invoke ShowWindow, eax, SW_SHOWDEFAULT
  invoke UpdateWindow, [mwHandle]

;*******************************************************************************

.getMessage:
  invoke GetMessage, mwMessage, NULL, 0, 0
  test eax, eax
  jz near .end

  test eax, -1
  jz near .end

  invoke TranslateMessage, mwMessage
  invoke DispatchMessage, mwMessage

  jmp .getMessage

.end:
  invoke ExitProcess, [mwMessage + MSG.wParam]

;*******************************************************************************

;LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
function WndProc, hwnd, msg, wParam, lParam
begin
  mov eax, dword [msg]

  cmp eax, WM_CREATE
  je near .create

  cmp eax, WM_SIZE
  je near .resize

  cmp eax, WM_COMMAND
  je near .command

  cmp eax, WM_DESTROY
  je near .destroy

  cmp eax, WM_CLOSE
  je near .destroy

  ;return DefWindowProcW(hwnd, msg, wParam, lParam);
  invoke DefWindowProc, [hwnd], [msg], [wParam], [lParam]
  return eax

;Vytvorenie okna a ostatnych widgetov
.create:
  invoke GetModuleHandle, NULL
  mov [ewHInstance], eax

  ;Vytvorenie editoru
  invoke CreateWindowEx, WS_EX_CLIENTEDGE, ewClassName, emptyStr, \
    WS_CHILD + WS_VISIBLE + WS_VSCROLL + WS_HSCROLL + ES_MULTILINE + \
    ES_AUTOVSCROLL + ES_AUTOHSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, 100, 100, \
    [hwnd], IDC_MAIN_EDIT, [mwHInstance], NULL

  test eax, eax
  jz near .destroy
  mov [ewHandle], eax

  ;Pridanie menu
  invoke CreateMenu
  test eax, eax
  jz near .destroy
  mov [mwMenubar], eax

  invoke CreateMenu
  test eax, eax
  jz near .destroy
  mov [mwMenu], eax

  invoke AppendMenu, [mwMenu], MF_STRING, ID_FILE_NEW,  menuNew
  invoke AppendMenu, [mwMenu], MF_STRING, ID_FILE_OPEN, menuOpen
  invoke AppendMenu, [mwMenu], MF_STRING, ID_FILE_SAVE, menuSave
  invoke AppendMenu, [mwMenu], MF_STRING, ID_FILE_QUIT, menuQuit

  invoke AppendMenu, [mwMenubar], MF_POPUP, [mwMenu], menuFile

  invoke SetMenu, [hwnd], [mwMenubar]

  jmp near .finish

;Zmena rozmerov okna
.resize:
  invoke GetClientRect, [hwnd], MainWinRect
  invoke SetWindowPos, [ewHandle], NULL, 0, 0, [MainWinRect + RECT.right], \
    [MainWinRect + RECT.bottom], SWP_NOZORDER
  
  jmp .finish

; Akcie
.command:
  mov eax, dword [wParam]

  cmp eax, ID_FILE_QUIT
  je near .cmd_quit

  cmp eax, ID_FILE_NEW
  je near .cmd_new

  cmp eax, ID_FILE_OPEN
  je near .cmd_open

  cmp eax, ID_FILE_SAVE
  je near .cmd_save

  jmp near .finish

;*******************************************************************************
; QUIT
;*******************************************************************************
.cmd_quit:
  invoke SendMessage, [hwnd], WM_CLOSE, 0, 0
  jmp near .finish

;*******************************************************************************
; NEW
;*******************************************************************************
.cmd_new:
  invoke SetDlgItemText, [hwnd], IDC_MAIN_EDIT, emptyStr
  jmp near .finish

;*******************************************************************************
; OPEN
;*******************************************************************************
.cmd_open:
  mov eax, [hwnd]
  mov [ofnOpen + OPENFILENAME.hWndOwner], eax

  invoke GetOpenFileName, ofnOpen
  mov ebx, [ofnOpen + OPENFILENAME.lpstrFile]
  invoke CreateFile, ebx, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL

  cmp eax, INVALID_HANDLE_VALUE
  je near .destroy
  mov [fileOpen], eax

  invoke GetFileSize, [fileOpen], NULL
  cmp eax, 0xFFFFFFFF
  je near .destroy
  mov [sizeOpen], eax
  add eax, dword 1

  invoke GlobalAlloc, GPTR, eax
  cmp eax, NULL
  je near .destroy
  mov [strOpen], eax

  invoke ReadFile, [fileOpen], [strOpen], [sizeOpen], readOpen, NULL
  invoke SetDlgItemText, [hwnd], IDC_MAIN_EDIT, [strOpen]

  invoke GlobalFree, [strOpen]
  invoke CloseHandle, [fileOpen]

  jmp near .finish

;*******************************************************************************
; SAVE
;*******************************************************************************
.cmd_save:
  mov eax, [hwnd]
  mov [ofnSave + OPENFILENAME.hWndOwner], eax

  invoke GetSaveFileName, ofnSave
  mov ebx, [ofnSave + OPENFILENAME.lpstrFile]

  invoke CreateFile, ebx, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, \
    FILE_ATTRIBUTE_NORMAL, NULL

  cmp eax, INVALID_HANDLE_VALUE
  je near .destroy
  mov [fileSave], eax

  invoke GetWindowTextLength, [ewHandle]
  cmp eax, 0
  je near .destroy
  mov [sizeSave], eax
  add eax, dword 1
  mov ebx, eax

  invoke GlobalAlloc, GPTR, ebx
  cmp eax, NULL
  je near .destroy
  mov [strSave], eax

  invoke GetWindowText, [ewHandle], [strSave], ebx
  invoke WriteFile, [fileSave], [strSave], [sizeSave], readSave, NULL

  invoke GlobalFree, [strSave]
  invoke CloseHandle, [fileSave]

  jmp near .finish

;*******************************************************************************
.destroy:
  invoke PostQuitMessage, 0

;*******************************************************************************
.finish:
  return 0

end

; proj.asm
