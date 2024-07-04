.model small
.stack 100h

.data
    new_line db 0Ah, 0Dh, "$"
    allowed_chars_for_frist db "-0123456789ABCDEF"
    allowed_chars db "0123456789ABCDEF" 
    input_message_1 db "input number 1: $"
    input_message_2 db "input number 2: $"
    input_again_message db "input again: $"
    and_output_message db "AND: $"
    or_output_message db "OR: $"
    xor_output_message db "XOR: $"
    not_output_message_1 db "NOT 1: $"
    not_output_message_2 db "NOT 2: $"
    string_max_size equ 6 
    input_string db string_max_size, ?, string_max_size dup("$")        

.code
start proc
    mov ax, @data
    mov ds,ax  
    mov es,ax
    
    lea dx, input_message_1
    call print
    
    lea dx, input_string
    lea si, input_string + 1
    call get_user_input
    mov ax, dx
    
    push ax   
    lea dx, input_message_2
    call print
    
    lea dx, input_string
    lea si, input_string + 1
    call get_user_input
    pop ax
    mov bx, dx
    
    ;-- AND --
    push ax
    push bx
    and ax, bx
    lea si, input_string
    call word_to_hex_string
    lea dx, and_output_message
    call print
    lea dx, input_string
    call print
    lea dx, new_line
    call print
    pop bx
    pop ax
    
    ;-- OR --
    push ax
    push bx
    or ax, bx
    lea si, input_string
    call word_to_hex_string
    lea dx, or_output_message
    call print
    lea dx, input_string
    call print
    lea dx, new_line
    call print
    pop bx
    pop ax 
    
    ;-- XOR --
    push ax
    push bx
    xor ax, bx
    lea si, input_string
    call word_to_hex_string
    lea dx, xor_output_message
    call print
    lea dx, input_string
    call print
    lea dx, new_line
    call print
    pop bx
    pop ax
    
    ;-- NOT 1 --
    push ax
    push bx
    not ax
    lea si, input_string
    call word_to_hex_string
    lea dx, not_output_message_1
    call print
    lea dx, input_string
    call print
    lea dx, new_line
    call print
    pop bx
    pop ax
    
    ;-- NOT 2 --
    push ax
    push bx
    not bx
    lea si, input_string
    mov ax, bx
    call word_to_hex_string
    lea dx, not_output_message_2
    call print
    lea dx, input_string
    call print
    lea dx, new_line
    call print
    pop bx
    pop ax  
    
    call exit    
start endp

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
    mov al, '$'
    mov [si], al                

    ; go to the new line in console
    mov ah, 9
    push dx
    xor dx, dx
    lea dx, new_line
    int 21h
    pop dx
    
    lea si, input_string
    call validate_16bit_hex_string 
        
    lea si, input_string
    call hex_string_to_word 

    pop cx
    pop ax
    ret
get_user_input endp

validate_16bit_hex_string proc
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    xor ax, ax
    
    inc si      
    xor cx, cx
    mov cl, [si]
    inc si
    lodsb
    cmp ax, '-'
    je skip_minus_check
    cmp cx, 5
    je validation_failed  
    skip_minus_check:
    lea di, allowed_chars_for_frist
    push cx
    mov cx, 17
    repne scasb
    pop cx
    jne validation_failed
    dec cx
    validate_loop:
        lodsb
        lea di, allowed_chars
        push cx
        mov cx, 16
        repne scasb
        pop cx
        jne validation_failed    
    loop validate_loop
    jmp validation_success
    
    validation_failed:
    lea dx, input_again_message
    call print
    lea dx, input_string
    lea si, input_string + 1
    call get_user_input        
    
    validation_success:
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
validate_16bit_hex_string endp
           
           
           
           
;ax   -> word number
;si   -> pointer to buffer
;[si] <- hex string
word_to_hex_string proc
    push ax
    push bx
    push cx
    push dx
    
    ; fill buffer with $
    push ax
    push di
    mov di, si
    mov al, '$'
    mov cx, 6
    fill_buffer_with_dollar_loop:
        stosb
    loop fill_buffer_with_dollar_loop
    pop di
    pop ax
    
    mov bx, ax
    shr bx, 15
    cmp bx, 1
    jne number_not_negative 
    neg ax
    mov [si], '-'
    inc si
    number_not_negative:
    mov dx, ax
    
    and dl, 00001111b  ;move bits to get small tetrade   
    call tetrage_to_char
    mov [si]+3, dl      
    
    mov dx, ax
    
    shr dl, 4  ;move bits to get big tetrade   
    call tetrage_to_char
    mov [si]+2, dl    
    
    mov dx, ax 
    mov dl, dh
    
    and dl, 00001111b  ;move bits to get small tetrade   
    call tetrage_to_char
    mov [si]+1, dl      
    
    mov dx, ax
    mov dl, dh
    
    shr dl, 4  ;move bits to get big tetrade   
    call tetrage_to_char
    mov [si], dl
    
    pop dx
    pop cx
    pop bx 
    pop ax
      
    ret 
word_to_hex_string endp
              
              
              
              
              
;si -> pointer to buffer with string 
;dx <- return word 
hex_string_to_word proc
    push ax
    push bx
    push cx
        
    xor cx, cx      ;cx == 0
    mov cl, [si]+1  ;string length to cx
    
    ;cmp cl, 4       ; if (cx > 4) oversized_string_error
    ;jg oversized_string_error
   
    xor dx, dx      ;dx == 0
                       
    inc si          ;si to string front  
    
    hex_string_to_word_loop:
        
        inc si          ;move in buffer
        mov bl, [si]
        cmp bl, '-'
        jne non_negative
        mov bh, bl
        inc si
        dec cx
        non_negative:
        shl dx, 4       ;move bits in dx (0000 0000 0000 0000) <-4
        
        xor ax, ax      ;ax == 0
       
        mov al, [si]    ;ax == &si    
        
        cmp al, '0'     ;if (ax < '0') not a number
        ;jl not_a_number_error
         
        cmp al, '9'     ;if (ax > '9') it is a letter
        jg letter 
        
        sub al, '0'     ;get tetrade number
        jmp not_a_letter_skip
        
        letter: 
            and al, 11011111b ;uppercase  
            
            ;cmp al, 'A'   ;if (ax < 'A') not a number
            ;jl not_a_number_error  
           
            ;cmp al, 'F'   ;if (ax > 'F') not a number
            ;jg not_a_number_error   
            
            sub al,'A'-10 ;get tetrade number
        
        not_a_letter_skip:
        
        or dx, ax       ;push into dx     
        
        
    loop hex_string_to_word_loop 
    
    cmp dx, 8000h
    jne not_8000h 
    cmp bh, '-'
    je not_overflow
    jne this_is_overflow   
    not_8000h:
    mov cx, dx
    shr cx, 15
    cmp cx, 1
    jne not_overflow
    this_is_overflow:
    lea dx, input_again_message
    call print
    lea dx, input_string
    lea si, input_string + 1
    call get_user_input
    
    not_overflow:
    cmp bh, '-'
    jne exit_conversion
    neg dx
    exit_conversion: 
    pop cx 
    pop bx    
    pop ax
    ret
hex_string_to_word endp  

;dl -> register with tetrade
;dl <- ascii of char
tetrage_to_char proc
        
        add dl, '0'    ;add ascii of '0' to get dec num char 
        
        cmp dl, '9'    ;if al > '9' it is a letter 
        jle tetrage_to_char_end
        
        add dl, 7      ;if letter get right letter by adding 7
                       ;(acsii codes)
    
    tetrage_to_char_end:    
    ret
tetrage_to_char endp


exit proc
    mov ah, 4ch
    int 21h
exit endp     


end start 