;*****************************************************************
;* This stationery serves as the framework for a                 *
;* user application (single file, absolute assembly application) *
;* For a more comprehensive program that                         *
;* demonstrates the more advanced functionality of this          *
;* processor, please see the demonstration applications          *
;* located in the examples subdirectory of the                   *
;* Freescale CodeWarrior for the HC12 Program directory          *
;*****************************************************************

; export symbols
            XDEF Entry, _Startup            ; export 'Entry' symbol
            ABSENTRY Entry        ; for absolute assembly: mark this as application entry point
                                                              
; Include derivative-specific definitions 
		INCLUDE 'derivative.inc' 

ROMStart    EQU  $4000  ; absolute address to place my code/constant data
data        EQU  $1000
backup      EQU  $2000  ; yedek alinan yer
resultadd   EQU  $3000  ; sonucun bulundugu adres

            ORG data
 ; Insert here your data definition.
inp1     dc.w $28 ;     40  decimal  -   28 Hex
inp2     dc.w $18 ;     24  decimal  -   18 Hex

            ORG resultadd
result   ds.w 1   ;     120 decimal  -   78 Hex

            ORG backup
storageX  ds.w 1        ; 1. sayinin yedeginin alinacagi yer
storageY  ds.w 1        ; 2. sayinin yedeginin alinacagi yer

; code section
            ORG   ROMStart     

Entry:
_Startup:
            MOVW inp1, storageX   ; birinci sayi nin yedegi
            MOVW inp2, storageY   ; ikinci sayi nin yedegi                 
while:
            LDD   storageX        ; D ye birinci sayi yuklenir
            SUBD  storageY        ; 1. sayidan 2. sayi cikarilir
            BEQ   return          ; sayilar birbirine esit ise return e gidilir
                         
            BLT   negative        ; 1. sayi 2. sayidan kucuk ise label'a atlar.              
             
            LDD   storageY        ; D ye ikinci sayi yuklenir          
            ADDD  inp2            ; 1. sayi 2. sayidan buyuk ise 2. sayinin uzerine 2. sayinin ilk degeri eklenir. 
            STD   storageY        ; 2. sayinin degistirilmis degeri 2. sayinin saklandigi yere yazilir
            
            BRA   while           ; while a geri don
negative:

            LDD   storageX        ; D ye birinci sayi yuklenir          
            ADDD  inp1            ; 1. sayi 2. sayidan kucuk ise 1. sayinin uzerine 1. sayinin ilk degeri eklenir.  
            STD   storageX        ; 1. sayinin degistirilmis degeri 1. sayinin saklandigi yere yazilir.
            
            BRA   while           ; while a geri don
return:
            LDD   storageX        ; 1. sayinin son hali D ye eklenir
            STD   result          ; D deki sonuc result ($3000) adresine yuklenir.
            
            LDAA  #$FF            ; A ya FF yuklenir
            STAA  DDRB            ; DDRB output yapilir.         
            STAB  PORTB           ; PORTB ye sonuc eklenir.
            
            SWI
;**************************************************************
;*                 Interrupt Vectors                          *
;**************************************************************
            ORG   $FFFE
            DC.W  Entry           ; Reset Vector
