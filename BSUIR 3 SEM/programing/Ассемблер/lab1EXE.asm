.model tiny      ;64 Kilobite
.code      
  org 100h     ;reserved for some structures(carrete)

_start:     
      lea dx,message  
      mov ah, 9      
      int 21h       
      mov ah, 2      
      mov dl, 7       
      int 21h         
      mov ax,4C00h   
      int 21h         

message db "Hello world! 16-bit DOS assembly here!", 0Dh, 0Ah, "$"
end _start            
