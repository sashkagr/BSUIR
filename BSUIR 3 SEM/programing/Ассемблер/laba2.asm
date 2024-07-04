.model small
.stack 100h   
.data                     
    new_line db 0Ah, 0Dh, "$" 
    input_message db "input: $"
    output_message db "output: $"
    delimeters db " ,.!;:?"
    delimeters_length equ 7
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
    
    call sort_words
    
    lea dx, output_message
    call print
    lea dx, input_string + 2
    call print
    
    call exit
start endp

sort_words proc
    push ax
    push bx
    push dx
    push cx
    xor ax, ax
    xor bx, bx
    xor cx, cx
    xor dx, dx
    
    call get_words_count
    
    mov cl, 1
    outer_loop:
        mov ch, cl
        cmp ch, [input_string]
        je end_inner_loop
        inc ch
        inner_loop:
            push cx
            
            mov dh, ch
            call get_words_count
            mov dh, ah
            mov dl, al
            
            push dx
            mov dh, cl
            call get_words_count
            pop dx
            
            mov cl, al
            sub cl, ah
            mov bl, dl
            sub bl, dh
            inc cl
            inc bl
            cmp cl, bl
            jg skip_swap
            call swap
            skip_swap:
            pop cx
        inc ch
        cmp ch, [input_string]
        jng inner_loop
        end_inner_loop:
    inc cl
    cmp cl, [input_string]
    jng outer_loop
    
    
    pop cx
    pop dx
    pop bx
    pop ax
    ret    
sort_words endp
 
get_words_count proc
    push cx
    push bx
    xor cx, cx 
    xor dl, dl
    xor bx, bx
    
    mov cl, [input_string+1]
    lea si, input_string+2
    get_words_count_loop:
        push cx
        lodsb
        lea di, delimeters
        mov cx, delimeters_length
        repne scasb
        jne not_delimeter
            cmp bl, 0
            je continue_with_non_word
                cmp dh, dl
                jne continue_with_non_word
                    push cx
                    mov cx, si
                    mov bh, cl
                    dec bh
                    dec bh
                    pop cx       
            continue_with_non_word:
            mov bl, 0
            jmp end_of_that_cycle        
        not_delimeter:
            cmp bl, 0
            jne already_in_word
                ;start of word
                inc dl
                cmp dh, dl
                jne already_in_word
                    ; save first index
                    push cx
                    mov cx, si
                    mov ah, cl
                    dec ah
                    pop cx
            already_in_word: 
            mov bl, al
        end_of_that_cycle:
        pop cx            
    loop get_words_count_loop
    mov al, bh
    mov [input_string], dl
    pop bx
    pop cx
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
    
    ; add space and $ at the end
    xor cx, cx      
    mov bx, si      
    mov cl, [si]    
    inc cl        
    add si, cx     
    mov al,' '     
    mov [si], al
    inc si
    mov al, '$'
    mov [si], al
    mov al, [input_string+1]
    inc al
    mov [input_string+1], al      

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
    mov si, bx
    ; di points to the ending index
    mov di, cx
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
   ;lea si, input_string + 2 ;                            
  ; lea di, input_string + 2 ;
   call   reverse_string    
  
    mov bl,ah
    mov ch,dl              ; 
    sub ch,dh
    add ch,ah
    mov cl,ch
    xor ch,ch
   ;lea si, input_string + 2 ;                            
  ; lea di, input_string + 2 ;
   call   reverse_string    
    
    mov bh,al
    sub bh,ah 
    mov bl,dl
    sub bl,bh              
    mov cl,dl
    xor bh,bh
   ;lea si, input_string + 2 ;                            
   ;lea di, input_string + 2 ;
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
  ; lea si, input_string + 2 ;                            
   ;lea di, input_string + 2 ;
   call   reverse_string                 
   ret           
swap endp          
            
             
exit proc
    mov ah, 4ch
    int 21h
exit endp     

end start