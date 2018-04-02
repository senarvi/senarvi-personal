
        global  init:near
        global  initmem:near,resetmem:near
        global  int00handler:far,int02handler:far,int06handler:far
        global  int12handler:far,int13handler:far,int14handler:far

        global  basememaddress:dword,basememfree:dword
        global  c32sel:dword,c32base:dword


        global  start:near

        global  memreq:dword,heapseg:dword,basememreq:dword

