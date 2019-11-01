//
// Created by 16182 on 10/27/2019.
//

#include <winuser.h>
#include <windows.h>


int main(){
    for(int i = 0; i < 200; i ++) {
        SetCursorPos(5 * i, 5 * i);
        Sleep(16);
    }
}