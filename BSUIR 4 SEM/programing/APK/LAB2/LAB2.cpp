#include <dos.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void printRegister(int reg, char far* pos) {
  for (int i = 0; i < 8; i++) {
    *pos = reg % 2 + '0';
    pos += 2;
    reg = reg >> 1;
  }
}

void print()
{
  char temp;
  int i, val;
  char far* screen = (char far*)MK_FP(0xB800, 0);
  screen += 20;

  val = inp(0x21);                    // get mask Master registor                     
  printRegister(val, screen);

  screen += 18;

  val = inp(0xA1);                    // регистр масок ведомого контроллера
  printRegister(val, screen);

  screen += 142;

  outp(0x20, 0x0A);
  val = inp(0x20);
  printRegister(val, screen);          // get Masters's request register   

  screen += 18;

  outp(0xA0, 0x0A);
  val = inp(0xA0);                     // get Slave's request register   
  printRegister(val, screen);

  screen += 142;

  outp(0x20, 0x0B);
  val = inp(0x20);                     // get Master's service register 
  printRegister(val, screen);

  screen += 18;

  outp(0xA0, 0x0B);
  val = inp(0xA0);                     // get Slave's service register  
  printRegister(val, screen);
}

void prepare() {
  _disable();
  print();
}

void interrupt(*oldint1[8])(...); // IRQ 0-7
void interrupt(*oldint2[8])(...); // IRQ 8-15

void interrupt  newint08(...) { _disable(); print(); oldint1[0](); _enable(); }
void interrupt  newint09(...) { prepare(); oldint1[1](); _enable(); }
void interrupt  newint0A(...) { prepare(); oldint1[2](); _enable(); }
void interrupt  newint0B(...) { prepare(); oldint1[3](); _enable(); }
void interrupt  newint0C(...) { prepare(); oldint1[4](); _enable(); }
void interrupt  newint0D(...) { prepare(); oldint1[5](); _enable(); }
void interrupt  newint0E(...) { prepare(); oldint1[6](); _enable(); }
void interrupt  newint0F(...) { prepare(); oldint1[7](); _enable(); }

void interrupt  newint98(...) { prepare(); oldint2[0](); _enable(); }
void interrupt  newint99(...) { prepare(); oldint2[1](); _enable(); }
void interrupt  newintA0(...) { prepare(); oldint2[2](); _enable(); }
void interrupt  newintA1(...) { prepare(); oldint2[3](); _enable(); }
void interrupt  newintA2(...) { prepare(); oldint2[4](); _enable(); }
void interrupt  newintA3(...) { prepare(); oldint2[5](); _enable(); }
void interrupt  newintA4(...) { prepare(); oldint2[6](); _enable(); }
void interrupt  newintA5(...) { prepare(); oldint2[7](); _enable(); }

void initialize()
{
  int i;
  for (i = 0x70; i <= 0x77; i++) {
    oldint2[i - 0x70] = getvect(i);
  }

  for (i = 0x08; i <= 0x0F; i++) {
    oldint1[i - 0x08] = getvect(i);
  }
  //set new handlers for IRQ 0-7
  setvect(0x98, newint08);
  setvect(0x99, newint09);
  setvect(0xA0, newint0A);
  setvect(0xA1, newint0B);
  setvect(0xA2, newint0C);
  setvect(0xA3, newint0D);
  setvect(0xA4, newint0E);
  setvect(0xA5, newint0F);
  //set new handlers for IRQ8-15  
  setvect(0x08, newint98);
  setvect(0x09, newint99);
  setvect(0x0A, newintA0);
  setvect(0x0B, newintA1);
  setvect(0x0C, newintA2);
  setvect(0x0D, newintA3);
  setvect(0x0E, newintA4);
  setvect(0x0F, newintA5);

  _disable(); // CLI
  //interrupt initializtion for Master
  outp(0x20, 0x11);  //ICW1 - initialize master (00010001, 0001 - base, last 1 - next will be) 
  outp(0x21, 0x98);   //ICW2 - base vector for master (10101000, )
  outp(0x21, 0x04);  //ICW3 - the port bit of Slave (in binary format)
  outp(0x21, 0x01);  //ICW4 - default
  //interrupt initialization for Slave
  outp(0xA0, 0x11);  //ICW1 - initialize Slave  
  outp(0xA1, 0x08);  //ICW2 - base vector for slave
  outp(0xA1, 0x02);  //ICW3 - the port number of connected port on Master 
  outp(0xA1, 0x01);  //ICW4 - default 
  _enable(); // STI
}

int main()
{
  unsigned far* fp;
  initialize();
  system("cls");

  printf("mask:   \n");
  printf("obsl:\n");
  printf("requ:\n");

  FP_SEG(fp) = _psp;
  FP_OFF(fp) = 0x2c;
  _dos_freemem(*fp);

  _dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);

  return 0;
}