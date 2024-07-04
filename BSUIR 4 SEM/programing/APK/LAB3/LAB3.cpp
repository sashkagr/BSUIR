#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>

int frequencies[] = {     
        262, 262, 294, 262, 349, 330, 262, 
        262, 262, 294, 262, 392, 349, 262, 
        262, 262, 523, 440, 349, 330, 294, 494,
        466, 466, 440, 349, 392, 349 
        };

int durations[] = {  200, 200, 400, 400, 400, 800, 400,
        200, 200, 400, 400, 400, 800, 400, 
        200, 200, 400, 400, 400, 400, 800, 400, 
        200, 200, 400, 400, 400, 800
 };

//  int frequencies[] = {
//     330, 330, 330, 262, 330, 392, 196, 262,
//     196, 330, 262, 392, 330, 262, 262, 330,
//     262, 196, 196, 262, 330, 392, 196, 262,
//     196, 330, 262, 392, 330, 262, 262, 330,
//     262, 392, 392, 330, 262, 196, 330, 262,
//     262, 330, 262, 196, 196, 330, 262, 392,
//     392, 330, 262, 196, 196, 262, 330, 262,
//     262, 330, 262, 392, 392, 330, 262, 196
// };

// int durations[] = {
//     250, 250, 500, 250, 250, 500, 250, 250,
//     250, 250, 250, 250, 250, 250, 250, 250,
//     250, 250, 250, 250, 250, 250, 250, 250,
//     250, 250, 250, 250, 250, 250, 250, 250,
//     250, 250, 250, 250, 250, 250, 250, 250,
//     250, 250, 250, 250, 250, 250, 250, 250,
//     250, 250, 250, 250, 250, 250, 250, 250,
//     250, 250, 250, 250, 250, 250, 250, 250
// };


void getSound(int freq, int time) {
    int count;

    outp(0x43, 0xb6); 
    
    count = (int)(1193180L / freq);
    outp(0x42, count & 0x00ff);
    outp(0x42, (count & 0xff00) >> 8);

    outp(0x61, inp(0x61) | 3);

    delay(time);

    outp(0x61, inp(0x61) & 0xfc);

}


void stateChannel() {
    unsigned int temp,del,dellow,delhigh;
    char* stateOfWord = new char[28];
    unsigned i;

    outp(0x43, 0xe2);

    temp = inp(0x40);
    itoa(temp, stateOfWord, 2);
    cout << "0 channel state word: " << stateOfWord << endl;

    outp(0x43, 0x06);
    outp(0x43, 0x36);
    dellow = inp(0x40);
    delhigh = inp(0x40);
    del = delhigh * 256 + dellow;
    itoa(del, stateOfWord, 16);
    cout << "del 0:   " << stateOfWord << endl;


    outp(0x43, 0xe4);

    temp = inp(0x41);
    itoa(temp, stateOfWord, 2);
    cout << "1 channel state word: " << stateOfWord << endl;


    outp(0x43, 0x46);
    outp(0x43, 0x76);
    dellow = inp(0x41);
    delhigh = inp(0x41);
    del = delhigh * 256 + dellow;
    itoa(del, stateOfWord, 16);
    cout << "del 1:  " << stateOfWord << endl;


    outp(0x43, 0xe8);

    temp = inp(0x42);
    itoa(temp, stateOfWord, 2);
    cout << "2 channel state word: " << stateOfWord << endl;

    outp(0x43, 0x86);
    outp(0x43, 0xb6);
    dellow = inp(0x42);
    delhigh = inp(0x42);
    del = delhigh * 256 + dellow;
    itoa(del, stateOfWord, 16);
    cout << "del 2:   " << stateOfWord << endl;

    free(stateOfWord);
}

int main() {
    for (int i = 0; i <28; i++)
    {
	getSound(frequencies[i], durations[i]);
    }
    stateChannel();
    return 0;
}
