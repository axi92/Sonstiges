#SingleInstance, Force ; Es darf nur eine Instanz von dem Programm vorhanden sein, wird eine neue gestartet, schlie࠴ sich die alte. (Reload)
#IfWinActive, axi92 Anwalt Rechner
x_quickjoin:=250
y_quickjoin:=5
x1:=30
x2:=230
version:=0.1
GUI, 1:font, s10 bold
Gui, 1:Add,   text,   cblack x%x1%  y25  +backgroundtranss, Sekunden:
Gui, 1:Add,   edit,          x110    y20  w100 vsekunden gUpdate, %sekunden%
Gui, 1:Add,   text,   cblack x%x1%  y45  +backgroundtrans, Preis:
Gui, 1:Add,   text,          x110  y50  w100 vpreis, %preis%
Gui, 1:Show, W250,axi92 Anwalt Rechner
return


Update:
Gui 1:Submit, NoHide
preis:= FormatNumberWithThousandSeparator(Ceil(sekunden/60/6*100000),",",".")
;MsgBox, %preis% : %sekunden%
preis:= preis "$"
GuiControl,,preis,%preis%
return

FormatNumberWithThousandSeparator(Number1,DS,TS) { 
    If Number1 contains %DS% 
        StringSplit, NumberA, Number1, %DS%
    Else 
        NumberA1 = %Number1% 
    If NumberA1 is not integer
        return
    If NumberA2 is not digit
        return
    Loop, % Floor((StrLen(NumberA1) - 1)/3) { 
       StringRight,Digit,NumberA1,% 3 + (A_Index -1 ) * 4 
       StringReplace, NumberA1, NumberA1, %Digit%, %TS%%Digit% 
      }
    If NumberA2 is space 
        Return NumberA1 
    Return NumberA1 . DS . NumberA2 
  }