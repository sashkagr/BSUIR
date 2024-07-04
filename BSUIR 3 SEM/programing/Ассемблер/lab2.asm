.model small
.stack 100h   
.data                     
    new_line db 0Ah, 0Dh, "$" 
    input_message db "input: $"
    output_message db "output: $"
    string_max_size equ 200 ; const
    input_string db string_max_size, ?, "$", string_max_size dup(0)
    
.code
start proc     
    mov ax, @data
    mov ds,ax  
    mov es,ax
    lea dx, input_message
    call print
    
    lea dx, input_string
    lea si, input_string + 1
    call get_user_input
        
    lea si, input_string + 2 ; Load the address of input_string
    call get_words_count   
    
       call find_begin_and_end
    
    lea dx, output_message
    call print
    lea dx, input_string + 2
    call print
    
    call exit
start endp
   
find_begin_and_end proc 
  xor ax,ax
  xor dx,dx
  xor bx,bx
  ;xor cx,cx
  
  
  lea si, input_string+2 
  add si,[bx] 
  lea di, input_string +2
  add di,[bx] 
  mov ax,si
  mov ah,al
  mov dx,di
  mov al,dl 
    
  M:  
   inc bl
   cmp di, ' '
   jmp N    
  
  N:  
   dec di 
   mov ax,di
   mov ah,al
   inc di
   inc di 
   mov si,di
   inc bl
   mov dh,si 
   
   F:   
   inc bl
   cmp di, ' '
   jmp V
     
   V:
   dec di 
   mov dl,di 
   inc bl 
  
  
call swap
  
 find_begin_and_end endp 
    
          

get_words_count proc
    push ax
    push bx
    xor ax,ax
    xor bx,bx
    xor cx, cx      ; Initialize word count to zero
    mov bl, ' '     ; Set the delimiter to space (' ')

count_loop:
    lodsb           ; Load a character from [si] into AL
    cmp al, bl       ; Compare AL with the delimiter
    je skip_space   ; If AL is a space, skip it
    cmp al, '$'     ; Check if we've reached the end of the string
    je done_count   ; If yes, we're done counting
    jmp count_loop ; Repeat the loop

skip_space:
    lodsb           ; Load the next character (could be space)
    cmp al, '$'     ; Check if we've reached the end of the string
    je done_count   ; If yes, we're done counting
    cmp al, bl      ; Check if the character is a space
    je skip_space   ; If it's a space, skip it
    inc cx          ; Increment word count
    jmp count_loop ; Repeat the loop

done_count:
    mov ah, 0
    inc cx       ; Clear AH to ensure proper termination
    pop bx
    pop ax
    ret
  
get_words_count endp 
   
   
   
   
   
   
   
                
; ** you have to move offset of your string into dx before using this **    
print proc
    push ax
    mov ah, 9
    int 21h  
    pop ax 
    ret
print endp 

; ** you have to move offset of your buffer into dx before using this **
; ** yout have to move offset of your stirng's length to si before using this **
get_user_input proc
    push ax
    push cx

    ; get user input into buff
    mov ah, 0Ah
    int 21h 
    ; insert space in the end of the string
    xor cx, cx         ; clear cx for use
    inc [si]           ;si++
    mov bx, si
    mov cl, [si]       ; move length value to cl 
    ;inc cx             ; increment cx to reach index in the end of the stirng for $
    add si, cx         ; now si points to the offset of the end of the string  |   200    |  6  |   |  |   |   |   |'$'|
   ; mov al, ' '        ; move $ to al so now we can move it to the memory of si
   ; mov [si], al       ; insert al in the position of the end of the string
    ; insert $ in the end of the string
   ; inc cx 
    mov al,'$'
    inc si
    mov [si], al 

    ; go to the new line in console
    mov ah, 9
    push dx
    xor dx, dx
    lea dx, new_line
    int 21h
    pop dx 

    pop cx
    pop ax
    ret
get_user_input endp 


reverse_string proc
    push ax
    push cx
    push dx
      
    xor dx,dx
    ; si points to the starting index
    add si, bx
    ; di points to the ending index
    add di, cx
    ; Loop to reverse the substring
reverse_loop:
    cmp si, di  ; Compare source and destination indices
    jae done    ; If si >= di, we're done
    
    lodsb       ; Load a character from [si] into AL    
    mov dl, [di] ; Load a character from [di] into DL   
    stosb 
    dec si      ; Store aL in [di]
    mov [si], dl ; Store dL in [si]
                                                        
    inc si      ; Move source index forward
    dec di  
    dec di    ; Move destination index backward
    jmp reverse_loop ; Repeat the loop
          
                  
done:
    pop dx
    pop cx
    pop ax
    ret
reverse_string endp   
                
swap proc        
    xor bx,bx
    xor cx,cx
    mov bl,ah
    mov cl,dl
   lea si, input_string + 2 ;                            
   lea di, input_string + 2 ;
   call   reverse_string    
  
    mov bl,ah
    mov ch,dl              ; 
    sub ch,dh
    add ch,ah
    mov cl,ch
    xor ch,ch
   lea si, input_string + 2 ;                            
   lea di, input_string + 2 ;
   call   reverse_string    
    
    mov bh,al
    sub bh,ah 
    mov bl,dl
    sub bl,bh              
    mov cl,dl
    xor bh,bh
   lea si, input_string + 2 ;                            
   lea di, input_string + 2 ;
   call   reverse_string  
             
             
    mov bl,ah
    mov ch,dl              ; 
    sub ch,dh
    add bl,ch
    xor ch,ch
    inc bl
    mov cl,dl
    mov bh,al
    sub bh,ah              
    sub cl,bh
    xor bh,bh  
    dec cl
   lea si, input_string + 2 ;                            
   lea di, input_string + 2 ;
   call   reverse_string                 
   ret           
swap endp          
            
             
exit proc
    mov ah, 4ch
    int 21h
exit endp     

end start