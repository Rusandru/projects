 .model small
 .stack 100h
 .data
y dw 80 ;coordonatele capului sarpelui
x dw 160
ylast dw 112 ;coordonatele cozii sarpelui
xlast dw 160
ytest dw 112 ;coordonatele celulei folosite pentru compararea de culori
xtest dw 160
i db 8 ;dimensiunile celului
j db 8

 .code
mov ax,@data
mov ds,ax
mov ah,00h
mov al,13h ;comutare in modul grafic 13h (320 x 200)
 int 10h



 call snakecellhead ;afisare sarpe initial
mov ax,4
mov dx,y
push dx

 initialrepeatbubble:
mov bh,i
;
 initialbubble:
inc y
dec bh
 jnz initialbubble
;
mov dx,y
push ax
 call snakecellbody
pop ax
dec ax
 jnz initialrepeatbubble

pop dx
mov [y],dx



 keys:
mov ah,00h
 int 16h
cmp ah,01h ;verificare apasare tasta esc
 je exitcall
;
cmp ah,48h ;verificare apasare tasta up arrow
 je moveupcall
;
cmp ah,50h ;verificare apasare tasta down arrow
 je movedowncall
;
cmp ah,4dh ;verificare apasare tasta right arrow
 je moverightcall
;
cmp ah,4bh ;verificare apasare tasta left arrow
 je moveleftcall
;
 jmp keys

 exitcall:
 call exit
 jmp keys

 moveupcall:
 call moveup
 jmp keys

 movedowncall:
 call movedown
 jmp keys

 moverightcall:
 call moveright
 jmp keys

 moveleftcall:
 call moveleft
 jmp keys



 moveup:
 call snakecellbody ;muta coordonatele capului in sus
mov bh,i
;
 bubbleup:
dec y
dec bh
 jnz bubbleup
;
 call snakecellhead
 call testlasty
 ret



 movedown:
 call snakecellbody ;muta coordonatele capului in jos
mov bh,i
;
 bubbledown:
inc y
dec bh
 jnz bubbledown
;
 call snakecellhead
 call testlasty
 ret



 moveright:
 call snakecellbody ;muta coordonatele capului la dreapta
mov bh,i
;
 bubbleright:
inc x
dec bh
 jnz bubbleright
;
 call snakecellhead
 call testlastx
 ret



 moveleft:
 call snakecellbody ;muta coordonatele capului la stanga
mov bh,i
;
 bubbleleft:
dec x
dec bh
 jnz bubbleleft
;
 call snakecellhead
 call testlastx
 ret



 exit:
mov ah,0h
mov al,3h ;comutare in modul text
 int 10h
mov ah,4ch
 int 21h
 ret



 snakecellhead: ;colorarea capului cu verde inchis
mov dx,y
mov cx,x
push cx
mov al,02h ;culoarea celulei
mov ah,0ch
mov bh,i
;
 bubbleheadi:
mov bl,j
;
 bubbleheadj:
 int 10h
inc cx
dec bl
 jnz bubbleheadj
;
pop cx
push cx
inc dx
dec bh
 jnz bubbleheadi
;
pop cx
 ret



 snakecellbody: ;colorarea corpului cu verde deschis
mov dx,y
mov cx,x
push cx
mov al,0ah ;culoarea celulei
mov ah,0ch
mov bh,i
;
 bubblebodyi:
mov bl,j
;
 bubblebodyj:
 int 10h
inc cx
dec bl
 jnz bubblebodyj
;
pop cx
push cx
inc dx
dec bh
 jnz bubblebodyi
;
pop cx
 ret



 testlasty:
 call colorreadinglast ;testarea celulei de sus
push ax
mov bh,i
;
 testbubbleupy:
dec ytest
dec bh
 jnz testbubbleupy
;
 call colorreadingtest
mov bl,al
pop ax
cmp al,bl
 je testendy

 call testreset
 call colorreadinglast ;testarea celulei de jos
push ax
mov bh,i
;
 testbubbledowny:
inc ytest
dec bh
 jnz testbubbledowny
;
 call colorreadingtest
mov bl,al
pop ax
cmp al,bl
 je testendy

 call testreset
 call colorreadinglast ;testarea celulei din dreapta
push ax
mov bh,i
;
 testbubblerighty:
inc xtest
dec bh
 jnz testbubblerighty
;
 call colorreadingtest
mov bl,al
pop ax
cmp al,bl
 je testendy

 call testreset
 call colorreadinglast ;testarea celulei din stanga
push ax
mov bh,i 
;
 testbubblelefty:
dec xtest
dec bh
 jnz testbubblelefty
;
 call colorreadingtest
mov bl,al
pop ax
cmp al,bl
 je testendy

 testendy:
 call snakecelllast
mov dx,ytest
mov cx,xtest
mov [ylast],dx
mov [xlast],cx
 ret



 testlastx:
 call colorreadinglast ;testarea celulei din dreapta
push ax
mov bh,i
;
 testbubblerightx:
inc xtest
dec bh
 jnz testbubblerightx
;
 call colorreadingtest
mov bl,al
pop ax
cmp al,bl
 je testendx

 call testreset
 call colorreadinglast ;testarea celulei din stanga
push ax
mov bh,i 
;
 testbubbleleftx:
dec xtest
dec bh
 jnz testbubbleleftx
;
 call colorreadingtest
mov bl,al
pop ax
cmp al,bl
 je testendx

 call testreset
 call colorreadinglast ;testarea celulei de sus
push ax
mov bh,i
;
 testbubbleupx:
dec ytest
dec bh
 jnz testbubbleupx
;
 call colorreadingtest
mov bl,al
pop ax
cmp al,bl
 je testendx

 call testreset
 call colorreadinglast ;testarea celulei de jos
push ax
mov bh,i
;
 testbubbledownx:
inc ytest
dec bh
 jnz testbubbledownx
;
 call colorreadingtest
mov bl,al
pop ax
cmp al,bl
 je testendx

 testendx:
 call snakecelllast
mov dx,ytest
mov cx,xtest
mov [ylast],dx
mov [xlast],cx
 ret



 testreset:
mov dx,ylast
mov cx,xlast
mov [ytest],dx
mov [xtest],cx
 ret

 colorreadinglast:
mov dx,ylast
mov cx,xlast
mov ah,0dh
 int 10h
 ret

 colorreadingtest:
mov dx,ytest
mov cx,xtest
mov ah,0dh
 int 10h
 ret



 snakecelllast: ;colorarea ultimei celule cu negru
mov dx,ylast
mov cx,xlast
push cx
mov al,00h ;culoarea celulei
mov ah,0ch
mov bh,i
;
 bubblelasti:
mov bl,j
;
 bubblelastj:
 int 10h
inc cx
dec bl
 jnz bubblelastj
;
pop cx
push cx
inc dx
dec bh
 jnz bubblelasti
;
pop cx
 ret



end