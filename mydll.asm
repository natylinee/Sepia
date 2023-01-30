;_______________________________________________________________________________________________________
;Temat projektu: sepia
;
;Algorytm polega na przekszta³ceniu obrazu na odcienie szaroœci przemna¿aj¹c odpowiednie subpiksele przez okreœlone wartoœci. 
;Natêpnie nale¿y specyzowaæ wspó³czynnik sepii z zakresu od 20 do 40 i i dodaæ go do zielonego subpiksela oraz jego watoœæ 
;pomno¿on¹ przez 2 do czerwonego subpiksela, niebieski bez zmian.
;
;Data oddania projektu: 31.01.2023 semestr 5 rok akademicki  2022/2023 Natalia Mendera
;_______________________________________________________________________________________________________

.data
	float_grey  dd 0.114f, 0.587f, 0.299f, 0.0f           ;RGB wspó³czynniki do skali szaroœci
	float_max dd 255.0f, 255.0f, 255.0f, 255.0f			;maksymalne wartoœci RGB
	float_sepia dd 0.0f, 20.0f, 40.0f, 0.0f					;wspó³czynniki do sepii, wype³nienie sepii = 20
	var db 0, 4, 8, 12, 12 dup (-1)								;maska do przestawienia bitów

.code
;_______________________________________________________________________________________________________
;rdi kolumna tablicy pikseli
;rdx szerokoœæ tablicy
;r10 indeks startowy dla danego w¹tku
;r11 indeks koñcowy dla danego w¹tku
;r12 wysokoœæ tablicy
;
;xmm0 piksel
;xmm1 wartoœci si³y sepii
;xmm3 roz³o¿one subpiksele w ka¿dej ni¿szej czêœci 32bitów rejestru
;xmm4 pomocniczy rejestr w kopiowaniu subpikseli
;xmm5 wartoœci do skali szaroœci
;xmm6 wartoœci maksymalne subpikseli
;
	;Funkcje sumuj¹ce wartoœci subpikseli i sk³aduj¹ce je w 4 32 bitowych czêœciach rejestru xmm3:
	;movshdup - (indeks nieparzysty)Move odd index single-precision floating-point values from xmm3/mem and duplicate each element into xmm4
	;addps - Add packed single-precision floating-point values from xmm4/m128 to xmm3 and store result in xmm3
	;movhlps - Move two packed single-precision floating-point values from high quadword of xmm3 to low quadword of xmm4
	;punpckldq - (przek³ada)Interleave low-order doublewords from xmm3 and xmm3/m128 into xmm3
	;
	;minps - Return the minimum single-precision floating-point values between xmm3 and xmm6/mem
;_______________________________________________________________________________________________________
Sepia proc
	push rbx
	mov r10,r8	;odczytanie indeksu startowego dla danego w¹tka
	mov r11,r9	; odczytanie indeksu koñcowego dla danego w¹tka

	movaps xmm5, [float_grey]			;inicjalizacja wartoœci do skali szaroœci
	movaps xmm6, [float_max]       ;inicjalizacja wartoœci maksymalnych
   	movaps xmm1, [float_sepia] 		;inicjalizacja wartoœci si³y sepii
		
nextColumn:
	mov r12,rcx					;wysokoœæ tablicy
	mov rdi,[rdx + r8*8]		;odczytanie odpowiedniej kolumny tablicy

sep:		
	movd xmm0,dword ptr[rdi]		;odczytanie piksela
	;Muszê rozszerzyæ 32 bity na 128bitów, aby móc pomno¿yæ ka¿dy subpiksel * wspó³czynnik szaroœci
	
	pmovzxbd xmm3, xmm0			;rozk³ada elementy - ka¿dy piksel w ni¿szej czêœci 32bitów
	cvtdq2ps xmm3, xmm3			;konwertuje 4 DWORD Integer w xmm3 na Single-Precision Floating Point
	mulps		xmm3, xmm5			;przemno¿enie piksela przez wartoœci rgb -szaroœæ
	;Mam a|r|g|b, a na potrzeby algorytmu muszê mieæ a+r+g+b w ka¿dej 32bitowej czêœci rejestru xmm

	movshdup    xmm4, xmm3		;xmm3 |a|r|g|b|									xmm4 |a|a|g|g|
    addps			xmm3, xmm4		;xmm3 |a+a|a+r|g+g|b+g|				xmm4 |a|a|g|g|
    movhlps		xmm4, xmm3		;xmm3 |a+a|a+r|g+g|b+g|				xmm4 |a|a|a+a|a+r|
    addps			xmm3, xmm4		;xmm3 |a+a+a|a+a+r|g+g+a+a|b+g+r+a|
	punpckldq	xmm3, xmm3		;xmm3 |g+g+a+a||g+g+a+a||b+g+r+a||b+g+r+a|
	punpckldq	xmm3, xmm3	    ;xmm3 |b+g+r+a||b+g+r+a||b+g+r+a||b+g+r+a|

	addps		xmm3, xmm1					;dodanie wartoœci sepii do sk³¹dowych piksela
	minps		xmm3, xmm6					;sprawdzanie czy wartosci nie przekroczy³y wartoœci 255
	cvtps2dq xmm3,xmm3						;Konwertuje float na int 
	pshufb xmm3, xmmword ptr var		;przemiesza bity w taki sposób, aby ka¿dy z subpikseli wróci³ na swoje pierwotne miejsce 
    movd    dword ptr [rdi], xmm3			;zapisanie zmienionego piksela w tabeli

	add	rdi, 4				;przesuniêcie na kolejny piksel
	dec r12					; zmniejszamy wysokoœæ tablicy
	jnz sep						;Jump if not zero (ZF = 0)
	add rdx,8					;przejœcie do kolejnej kolumny
	inc r10						;indeks startowy
	cmp r10,r11				;porównanie startowego z koñcowym
	jl nextColumn			;jeœli r11> r10 jump do nextColumn
	pop rbx
    ret
Sepia endp
END