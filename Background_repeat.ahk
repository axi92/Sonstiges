w:=16 ;width of individual tile
h:=9 ;height of individual tile
Gui, +resize
Gui,add,picture,x0 y0 w%w% h%h% hwndhPic,U:\AHK\Scripts\bg.gif
SendMessage,0x173,0,0,,ahk_id %hPic% ;0x173 is STM_GETIMAGE
hBMCopy:=ErrorLevel
;Get a copy of the picture
hBM:=DllCall("CopyImage",uint,hBMCopy,uint,0,int,0,int,0,uint,0x2000)
;Get device context for picture
hDC:=DllCall("GetDC",uint,hPic,uint)
;Create compatible device context to put picture in
hCDC:=DllCall("CreateCompatibleDC",uint,hDC,uint)
;Put the picture in the CDC 
DllCall("SelectObject",uint,hCDC,uint,hBM)
;Create compatible device context to hold tiled image
hCDC2:=DllCall("CreateCompatibleDC",uint,hDC,uint)
;Create an empty bitmap to trap the tiled image in
hBM2:=DllCall("CreateCompatibleBitmap",uint,hDC,int,A_ScreenWidth,int,A_ScreenHeight,uint)
;Put the empty image in so can draw on it
DllCall("SelectObject",uint,hCDC2,uint,hBM2)

x:=-1*w
y:=0
Loop
{
  x+=w
	if(x >= A_ScreenWidth){
		x:=0
		y+=h
		if(y >= A_ScreenHeight)
			break
	}
	;Build the tiled image in the compatible heretofore empty bitmap
	DllCall("BitBlt",uint,hCDC2,int,x,int,y,int,w,int,h,uint,hCDC,int,0,int,0,uint,0xcc0020)
}
;Cleanup
DllCall("ReleaseDC",uint,hPic,uint,hDC)
DllCall("DeleteObject",uint,hCDC)
DllCall("DeleteObject",uint,hCDC2)
DllCall("DeleteObject",uint,hBM)
;Apply the tiled image to the original picture
SendMessage,0x172,0,hBM2,, ahk_id %hPic% ;0x172 is STM_SETIMAGE message
if(ErrorLevel && hBM2 <> ErrorLevel)
{
	DllCall("DeleteObject",uint,errorlevel)
}	
	
x1:=15
x2:=230
Gui, 1:Add,   text,   cwhite x%x1%  y25  +backgroundtrans, PW:
Gui, 1:Add,   Button, x100        w180 h25 , BE-Quickjoin
Gui, 1:Add,   text,   cwhite x%x1%  y45 +backgroundtrans , ^ = Motor
Gui, 1:Add,   text,   cwhite x%x1%  y65  +backgroundtrans, F2 = _
Gui, 1:Add,   text,   cwhite x%x1%  y85  +backgroundtrans, F3 = _
Gui, 1:Add,   text,   cwhite x%x1%  y105 +backgroundtrans, F5 = _
Gui, 1:Add,   text,   cwhite x%x1%  y125 +backgroundtrans, F10 = _
Gui, 1:Add,   text,   cwhite x%x1%  y145 +backgroundtrans, Numpad7 = _
Gui, 1:Add,   text,   cwhite x%x1%  y165 +backgroundtrans, Numpad8 = _
Gui, 1:Add,   text,   cwhite x%x1%  y185 +backgroundtrans, Numpad9 = _
Gui, 1:Add,   text,   cwhite x%x1%  y205 +backgroundtrans, Numpad+ = _

Gui, 1:Add,   text,   cwhite x%x2% y45 +backgroundtrans , XXX
Gui, 1:Add,   text,   cwhite x%x2% y65 +backgroundtrans , Ende = Countdown
Gui, 1:Add,   text,   cwhite x%x2% y85 +backgroundtrans , /tempomat
Gui, 1:Add,   text,   cwhite x%x2% y105 +backgroundtrans, XXX
Gui, 1:Add,   text,   cwhite x%x2% y125 +backgroundtrans, /ab = Anrufabsage, verweis auf SMS
Gui, 1:Add,   text,   cwhite x%x2% y145 +backgroundtrans, /lgc = /listgangcars
Gui, 1:Add,   text,   cwhite x%x2% y165 +backgroundtrans, XXX

	

Gui, Show, x100 y100 w400 h400, Test
WinSet, Transparent, 160, Test ;Transparent
return

#t::
;WinSet, Style, -0xC00000, Test ; Remove the active window's title bar (WS_CAPTION).

return


GuiClose:
ExitApp
