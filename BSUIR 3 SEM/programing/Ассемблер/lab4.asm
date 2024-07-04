.model small
.stack 100h

.data
	video_data equ 0B800h
	screen_size equ 2000		;80x25 
	row_gap equ 80
	double_row_gap equ 160				;doubled 80
	scan_code_space equ 39h
	score_board_size equ 5
	blinking_blue equ 10011111b
	blue equ 1Fh
	red equ 5Fh
	brown equ 6Fh
	block_width equ 11
	block_height equ 3
	block_radius_x equ 5
	block_radius_y equ 1
	
	block_max_x equ 75
	block_max_y equ 24
	block_min_x equ 6
	block_min_y equ 3;2
	
	block_center_x db block_min_x
	block_center_y db block_min_y
	game_score dw 0
	move_direction db 0 			; 0 - right, 1 - left

	background db screen_size dup(' ', blue)
	game_over_background db screen_size dup(' ', red)
	
	block_brick dw 206Fh
	;block_brick dw 2B6Fh
	block_sprite_piece db block_width dup(' ', brown)
	mass_center db 0
	last_block_center db ?
	score_board db score_board_size dup('0', blue)
	decimal_base dw 10
	try_again_msg db "PRESS ANY KEY TO TRY AGAIN"
	
.code
start proc
	call init_data
	call init_video_mode
	jump_start_new_game:
	call hide_cursor
	mov [game_score], 0
	mov [mass_center], 0
	mov [last_block_center], 0
	outter_cycle:
	call print_score_board
	call randomize_spawn
	call spawn_block
	inner_cycle:
	call move_block
	call check_press_space
	cmp dx, 0FFFFh
	jne inner_cycle
	call move_block_down
	call check_mass_center
	cmp dx, 0FFFFh
	je jump_stop_game_fail
	call calculate_mass_center
	inc [game_score]
	call clear_keyboard_buffer
	cmp [game_score], 4
	jng outter_cycle
	call scroll_background
	jmp outter_cycle
	
	jump_stop_game_fail:
	
	mov cx, 5
	destroy_building:
	call scroll_background
	call fill_background
	call delay
	call delay
	call delay
	call delay
	loop destroy_building
	call clear_keyboard_buffer
	call print_try_again_message
	call console_pause
	jmp jump_start_new_game
	
	
	;call print_game_over
	call console_pause
	call exit_null
start endp

print_try_again_message proc
	push es
	push di
	push cx
	push si
	push ax
	
	push 0B800h
	pop es
	mov di, 1760 ; 80*2 * 11 + 50
	add di, 50 
	mov cx, 52	 ; 26 (message_length) * 2
	cld
	lea si, try_again_msg
	jump_print_message_loop:
	movsb
	mov ax, blinking_blue
	stosb
	loop jump_print_message_loop
	
	pop ax
	pop si
	pop cx
	pop di
	pop es
	ret
print_try_again_message endp

clear_keyboard_buffer proc
	push ax
	push es
	push cx
	push di

	mov ax, 0000h
	mov es, ax
	mov cx, 16
	cld
	mov di, 41Ah
	jump_clear_buff_word:
	mov ax, 0
	stosw
	loop jump_clear_buff_word
	
	pop di
	pop cx
	pop es
	pop ax
	ret
clear_keyboard_buffer endp

hide_cursor proc
	push ax
	push bx
	push dx
	
	mov ah, 02h
	mov bh, 0
	mov dh, 25
	mov dl, 0
	int 10h
	
	pop dx
	pop bx
	pop ax
	ret
hide_cursor endp

resolve_score proc
	push cx
	push ax
	push bx
	push dx

	jump_get_score:
	xor dx, dx
	mov cx, 8
	mov ax, [game_score]
	cmp ax, 0FFFFh
	jne jump_resolve_score_loop
	mov [game_score], 5
	jmp jump_get_score
	jump_resolve_score_loop:	
	div [decimal_base]
	add dx, 48
	lea bx, score_board
	add bx, cx
	mov [bx],dl 
	cmp cx, 0
	je jump_exit_resolve_score_loop
	sub cx, 2             
	xor dx, dx
	jmp jump_resolve_score_loop
	jump_exit_resolve_score_loop:
	
	pop dx
	pop bx
	pop ax
	pop cx
	ret
resolve_score endp

print_score_board proc
	push es
	push cx
	push si
	push di
	push ax

	call resolve_score
	mov ax, @data
	mov es, ax
	lea si, score_board
	lea di, background
	mov cx, score_board_size
	cld
	rep movsw

	pop ax
	pop di
	pop si
	pop cx
	pop es
	ret
print_score_board endp

randomize_spawn proc
	push ax
	push cx
	push dx
	
	mov cl, [block_center_x]
	mov [last_block_center], cl
	xor cl, cl

	mov ah, 00h
    int 1Ah
    mov al, dl
	mov dl, 4
    div dl
    add al, block_min_x
    test ah, 10b
    jz jump_random_is_set
    xor ah, 10b
	jump_random_is_set:
	
	cmp ah, 0
	je jump_init_left
	mov [block_center_x], block_max_x
	jmp jump_ret_position
	jump_init_left:
	mov [block_center_x], block_min_x
	
	jump_ret_position:
	mov [block_center_y], block_min_y
	
	pop dx
	pop cx
	pop ax
	ret
randomize_spawn endp

scroll_background proc
	push ax
	push es
	push cx
	push si
	push di
	push bx

	mov ax, @data
	mov es, ax
	
	mov bx, 3040 ; 3520
	  
	jump_make_scroll:
	lea si, background   
	add si, bx
	
	lea di, background
	add di, bx
	add di, 480
	
	mov cx, 240
	scrolling:  
	cld
	lodsw
	stosw
	loop scrolling
	sub bx, 480
	cmp bx, 640
	jne jump_make_scroll
	
	pop bx
	pop di
	pop si
	pop cx
	pop es
	pop ax
	ret
scroll_background endp

check_last_center proc
	push bx

	mov dx, 0						; correct mass center by default
	cmp [mass_center], 0			
	je jump_correct_last_center	; if mass center hasn't been initialized yet
	
	
	mov bl, [last_block_center]			; right border
	add bl, block_radius_x
	mov bh, [last_block_center]			; left border
	sub bh, block_radius_x
	
	cmp [block_center_x], bl
	jg jump_fail_last_center
	cmp [block_center_x], bh
	jl jump_fail_last_center
	jmp jump_correct_last_center
	
	jump_fail_last_center:
	not dx
	jump_correct_last_center:
	
	pop bx
	ret
check_last_center endp

check_mass_center proc
	push bx

	mov dx, 0						; correct mass center by default
	call check_last_center
	cmp dx, 0FFFFh
	je jump_skip_check_mass_center
	
	cmp [mass_center], 0			
	je jump_correct_mass_center	; if mass center hasn't been initialized yet

	mov bl, [mass_center]			; right border
	add bl, block_radius_x
	mov bh, [mass_center]			; left border
	sub bh, block_radius_x
	
	cmp [block_center_x], bl
	jg jump_fail_mass_center
	cmp [block_center_x], bh
	jl jump_fail_mass_center
	jmp jump_correct_mass_center
	
	jump_fail_mass_center:
	not dx
	jump_correct_mass_center:
	
	jump_skip_check_mass_center:
	
	pop bx
	ret
check_mass_center endp

calculate_mass_center proc
	push ax
	push dx
	
	mov al, [block_center_x]
	add al, [mass_center]
	cmp [mass_center], 0
	je jump_init_mass_center
	mov dl, 2
	div dl
	
	jump_init_mass_center:
	mov [mass_center], al
	
	pop dx
	pop ax
	ret
calculate_mass_center endp


check_press_space proc
	push ax

	mov dx, 0 				;not pressed by default
	mov ah, 01h
	int 16h
	jz jump_not_pressed
	mov ah, 00h
	int 16h
	cmp ah, scan_code_space
	jne jump_not_pressed
	not dx
	jump_not_pressed:
	
	pop ax
	ret
check_press_space endp

move_block_down proc
	push dx
	
	fall_block:
	call delay
	call check_collision
	cmp dx, 0FFFFh
	je jump_stop_block
	inc [block_center_y]
	call spawn_block
	jmp fall_block
	
	jump_stop_block:
	call release_block_sprite

	pop dx
	ret
move_block_down endp

check_collision proc
	push ax
	push cx
	push es
	push di
	push bx
	
	mov ax, @data
	mov es, ax
	
	mov dx, 0 							; collision is false by default
	cmp [block_center_y], block_max_y
	je jump_set_collision_true
	
	call resolve_array_address
	
	mov cx, block_height
	add cx, block_height
	jump_add_one_more_row:
	add ax, row_gap
	loop jump_add_one_more_row 
	
	
	mov di, ax
	add di, 4
	mov ax, [block_brick]
	mov cx,	12
	repne scasw
	jz jump_set_collision_true
	jnz jump_return_no_collision
		
	jump_set_collision_true:
	not dx
	
	jump_return_no_collision: 
	pop bx
	pop di
	pop es
	pop cx
	pop ax
	ret
check_collision endp

release_block_sprite proc
	push si
	push ax
	push es
	push di
	push cx  
	push ds
	         
	push video_data
	pop ds         
	mov si, 0
	mov ax, @data
	mov es, ax
	lea di, background
	mov cx, screen_size
	cld
	rep movsw
	
	;mov cl, [block_center_x]
	;mov [last_block_center], cl
	     
	pop ds
	pop cx
	pop di
	pop es
	pop ax
	pop si
	ret
release_block_sprite endp

delay proc
	push cx
	push dx
	push ax
	
	mov cx, 0
	mov dx, 30000
	mov ah, 86h
	int 15h
	
	pop ax
	pop dx
	pop cx
	ret
delay endp

move_block proc
	call delay

	jump_decide_direction:
	cmp [move_direction], 0
	jne jump_move_left
	
	cmp [block_center_x], block_max_x
	je jump_change_directory
	inc [block_center_x]
	jmp jump_move_block
	
	jump_change_directory:
	not [move_direction]
	jmp jump_decide_direction
	
	jump_move_left:
	cmp [block_center_x], block_min_x
	je jump_change_directory
	dec [block_center_x]
	
	jump_move_block:
	call spawn_block

	ret
move_block endp

resolve_array_address proc
	push bx
	push dx

	mov bl, [block_center_x]
	sub bl, block_radius_x 		; bx = x0
	mov dl, [block_center_y]
	sub dl, block_radius_y		; dx = y0
	
	xor ah, ah
	mov al, row_gap				; AX =
	mul dl						; = row_gap * y0 +
	add ax, bx					; + x0 -
	sub ax, row_gap				; - row_gap -
	dec ax						; - 1 =
	mov dx, 2
	mul dx
	
	pop dx
	pop bx
	ret
resolve_array_address endp

spawn_block proc
	push es
	push bx
	push dx
	push ax
	push di
	push si
	push cx
	
	call fill_background

	push video_data
	pop es
		
	call resolve_array_address
	mov di, ax					; = DI (address of the left-top corner of the block)
	
	mov dx, 1
	jump_print_sprite:
	lea si, block_sprite_piece
	mov cx, block_width
	cld
	rep movsw
	add di, double_row_gap
	sub di, block_width
	sub di, block_width
	inc dx
	cmp dx, block_height
	jng jump_print_sprite
		
	pop cx
	pop si
	pop di
	pop ax
	pop dx
	pop bx
	pop es
	ret
spawn_block endp

init_data proc
	push ax
	mov ax, @data
	mov ds, ax
	mov es, ax
	pop ax
	ret
init_data endp

init_video_mode proc
	push ax
	mov ah, 00h
	mov al, 03h
	int 10h
	pop ax
	ret
init_video_mode endp

console_pause proc
	push ax
	mov ah, 01h
	int 21h
	cmp al, 'q'
	jne jump_not_exit
	call exit_null
	jump_not_exit:
	pop ax
	ret
console_pause endp

exit_null proc
	mov ah, 4Ch
	int 21h
exit_null endp	
	
fill_background proc
	push cx
	push di
	push si
	push es

	push video_data
	pop es
	cld
	mov cx, screen_size
	mov di, 0
	lea si, background
	rep movsw

	pop es
	pop si
	pop di
	pop cx
	ret
fill_background endp


print_game_over proc
	push cx
	push di
	push si
	push es

	push video_data
	pop es
	cld
	mov cx, screen_size
	mov di, 0
	lea si, game_over_background
	rep movsw

	pop es
	pop si
	pop di
	pop cx
	ret
print_game_over endp



end start