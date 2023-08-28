/*
 * HookCode.cpp
 *
 *  Created on: 26 Aug 2023
 *      Author: Jackson Smackson
 */

#include "pch.h"
#include "HookCode.h"
#include "GlobalVars.h"

void __stdcall HookCode() {
    __asm
    {
        pushad
        pushfd
        mov eax, g_LocalPlayerBase
        cmp eax, ebx
        jne MODIFIED
        // @MODIFIED:
        mov eax, 0x4740C000
        mov[ebx + 0x40], eax
        movss xmm0, [ebx + 0x40]
        jmp ENDING
        // @ORIGINAL:
        MODIFIED :
        movss xmm0, [ebx + 0x2C8]
            // @ENDING:
            ENDING :
            popfd
            popad
    }

}



