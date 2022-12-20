.data
tab db 3 ; dzielnik ka�dego z piksela dla uzyskania szarosci
.code
Sepia proc
push rbx
mov r10,r8;odczytanie indeksu startowego dla danego w�tka
mov r11,r9; odczytanie indeksu ko�cowego dla danego w�tka
L1: 
mov r12,rcx ;wysokosc tablicy
mov rdi,[rdx + r8*8]; odczytanie odpowiedniej kolumny tablicy
L2:
movd xmm0,dword ptr[rdi] ;odczytanie piksela
pmovzxbd xmm0, dword ptr[rdi] ;rozdzielenie sk�adowych piksela
;****Szarosc*****;
shufps xmm0, xmm0, 4Eh
movupd xmm2,xmm0 ;wydzielenie koloru czerwonego
shufps xmm0, xmm0, 93h
pmovzxbd xmm1,xmm0; jeszcze jedno rozdzielenie �eby w xmm, by� tylko jeden kolor
addpd xmm2,xmm1; dodanie do koloru czerwnoego zielony
shufps xmm0,xmm0, 93h
pmovzxbd xmm1,xmm0; jeszcze jedno rozdzielenie �eby w xmm, by� tylko jeden kolor
addpd xmm2,xmm1; dodanie koloru niebieskiego
movd rax,xmm2
div tab ; podzielenie sumy sk�adowych przez 3
mov bl, al; przenoszenie, poniewa� w przypadaku dzielenia z reszt� w al znajduje si� cze��
;ca�kowita tego dzielenia
xor rax,rax
mov al,bl; cz�� ca�kowita po podzieleniu
; w tym miejscu mamy wartos� szarego piksela (w rejestrze rax)
;odrazu zmiana szarego piksela na sepie
;****Sepia*****;
add r13,rax; dodanie do reda odpowiednio przemno�onego wsp�czynnika
cmp r13, 255
jle dalszar; je�li warto�� piksela b�dzie mniejsza od 255 przejd� dalej
mov r13, 255; w przeciwnym razie przypisanie 255
dalszar:
;tu por�wnanie
cmp r13,59;59=(2*30)-1
ja przepr
mov r13,255
przepr:
add r15,rax;dodanie do greena wsp�czynnika
cmp r15, 255
jle dalszag
mov r15, 255
dalszag:
;tu por�wnanie
cmp r15,29;29=30-1
ja dalsza
mov r15,255
;zapisywanie do xmm0 zmieninonego kolorku
dalsza:
movd xmm0, rax; niebieski zostaje bez zmian
;przesuni�cia bitowe �eby zapisa� sk�adowe piksela w odpoweidnie miejcse w rejestrze xmm0
mov rbx,r15
shl r15,8
movd xmm1, r15
addsd xmm0,xmm1
mov rbx,r13
shl rbx,16
movd xmm1, rbx
addss xmm0,xmm1
movd dword ptr[rdi], xmm0;zapisanie piksela
add rdi,4; przej�cie do nast�pnego piksela
; ustawienie rejestr�w do warto�ci pocz�tkowych
mov r13,60; rejestr ten przechowuje przemno�ony wsp�czynnik dla koloru czerwonego
mov r15,30; rejestr ten przechowuje przemno�ony wsp�czynnik dla koloru zielonego
dec r12
jnz L2
add rdx,8; przej�cie do kolejnej kolumny
inc r10
cmp r10,r11
jl L1
pop rbx
ret
Sepia endp
end