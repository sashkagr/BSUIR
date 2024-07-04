.model small 
.stack 100h 
.data 
  message db "Hello world! 16-bit DOS assembly here!", 0Dh, 0Ah,"Hi", "$"
.code                 
  _start PROC         
      mov ax, @data   
      mov ds,ax 	    
      lea dx,message  
      mov ah, 9 	   
      int 21h 	    
      mov ah, 2       
      mov dl, 7       
      int 21h        
      mov ax,4C00h   
      int 21h	    
  _start ENDP	    

end _start
