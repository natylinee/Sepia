;_______________________________________________________________________________________________________
;Temat projektu: sepia
;
;Algorytm polega na przekszta�ceniu obrazu na odcienie szaro�ci przemna�aj�c odpowiednie subpiksele przez okre�lone warto�ci. 
;Nat�pnie nale�y specyzowa� wsp�czynnik sepii z zakresu od 20 do 40 i i doda� go do zielonego subpiksela oraz jego wato�� 
;pomno�on� przez 2 do czerwonego subpiksela, niebieski bez zmian.
;
;Data oddania projektu: 31.01.2023 semestr 5 rok akademicki  2022/2023 Natalia Mendera
;_______________________________________________________________________________________________________

.data
	float_grey  dd 0.114f, 0.587f, 0.299f, 0.0f           ;RGB wsp�czynniki do skali szaro�ci
	float_max dd 255.0f, 255.0f, 255.0f, 255.0f			;maksymalne warto�ci RGB
	float_sepia dd 0.0f, 20.0f, 40.0f, 0.0f					;wsp�czynniki do sepii, wype�nienie sepii = 20
	var db 0, 4, 8, 12, 12 dup (-1)								;maska do przestawienia bit�w

.code
;_______________________________________________________________________________________________________
;rdi kolumna tablicy pikseli
;rdx szeroko�� tablicy
;r10 indeks startowy dla danego w�tku
;r11 indeks ko�cowy dla danego w�tku
;r12 wysoko�� tablicy
;
;xmm0 piksel
;xmm1 warto�ci si�y sepii
;xmm3 roz�o�one subpiksele w ka�dej ni�szej cz�ci 32bit�w rejestru
;xmm4 pomocniczy rejestr w kopiowaniu subpikseli
;xmm5 warto�ci do skali szaro�ci
;xmm6 warto�ci maksymalne subpikseli
;
	;Funkcje sumuj�ce warto�ci subpikseli i sk�aduj�ce je w 4 32 bitowych cz�ciach rejestru xmm3:
	;movshdup - (indeks nieparzysty)Move odd index single-precision floating-point values from xmm3/mem and duplicate each element into xmm4
	;addps - Add packed single-precision floating-point values from xmm4/m128 to xmm3 and store result in xmm3
	;movhlps - Move two packed single-precision floating-point values from high quadword of xmm3 to low quadword of xmm4
	;punpckldq - (przek�ada)Interleave low-order doublewords from xmm3 and xmm3/m128 into xmm3
	;
	;minps - Return the minimum single-precision floating-point values between xmm3 and xmm6/mem
;_______________________________________________________________________________________________________
Sepia proc
	push rbx
	mov r10,r8	;odczytanie indeksu startowego dla danego w�tka
	mov r11,r9	; odczytanie indeksu ko�cowego dla danego w�tka

	movaps xmm5, [float_grey]			;inicjalizacja warto�ci do skali szaro�ci
	movaps xmm6, [float_max]       ;inicjalizacja warto�ci maksymalnych
   	movaps xmm1, [float_sepia] 		;inicjalizacja warto�ci si�y sepii
		
nextColumn:
	mov r12,rcx					;wysoko�� tablicy
	mov rdi,[rdx + r8*8]		;odczytanie odpowiedniej kolumny tablicy

sep:		
	movd xmm0,dword ptr[rdi]		;odczytanie piksela
	;Musz� rozszerzy� 32 bity na 128bit�w, aby m�c pomno�y� ka�dy subpiksel * wsp�czynnik szaro�ci
	
	pmovzxbd xmm3, xmm0			;rozk�ada elementy - ka�dy piksel w ni�szej cz�ci 32bit�w
	cvtdq2ps xmm3, xmm3			;konwertuje 4 DWORD Integer w xmm3 na Single-Precision Floating Point
	mulps		xmm3, xmm5			;przemno�enie piksela przez warto�ci rgb -szaro��
	;Mam a|r|g|b, a na potrzeby algorytmu musz� mie� a+r+g+b w ka�dej 32bitowej cz�ci rejestru xmm

	movshdup    xmm4, xmm3		;xmm3 |a|r|g|b|									xmm4 |a|a|g|g|
    addps			xmm3, xmm4		;xmm3 |a+a|a+r|g+g|b+g|				xmm4 |a|a|g|g|
    movhlps		xmm4, xmm3		;xmm3 |a+a|a+r|g+g|b+g|				xmm4 |a|a|a+a|a+r|
    addps			xmm3, xmm4		;xmm3 |a+a+a|a+a+r|g+g+a+a|b+g+r+a|
	punpckldq	xmm3, xmm3		;xmm3 |g+g+a+a||g+g+a+a||b+g+r+a||b+g+r+a|
	punpckldq	xmm3, xmm3	    ;xmm3 |b+g+r+a||b+g+r+a||b+g+r+a||b+g+r+a|

	addps		xmm3, xmm1					;dodanie warto�ci sepii do sk��dowych piksela
	minps		xmm3, xmm6					;sprawdzanie czy wartosci nie przekroczy�y warto�ci 255
	cvtps2dq xmm3,xmm3						;Konwertuje float na int 
	pshufb xmm3, xmmword ptr var		;przemiesza bity w taki spos�b, aby ka�dy z subpikseli wr�ci� na swoje pierwotne miejsce 
    movd    dword ptr [rdi], xmm3			;zapisanie zmienionego piksela w tabeli

	add	rdi, 4				;przesuni�cie na kolejny piksel
	dec r12					; zmniejszamy wysoko�� tablicy
	jnz sep						;Jump if not zero (ZF = 0)
	add rdx,8					;przej�cie do kolejnej kolumny
	inc r10						;indeks startowy
	cmp r10,r11				;por�wnanie startowego z ko�cowym
	jl nextColumn			;je�li r11> r10 jump do nextColumn
	pop rbx
    ret
Sepia endp
END