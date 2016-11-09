# Make sure current directory has MyJumplist_C.dll and MyJumplist_C.lib

JumplistDemo_C:
	gcc -s -o JumplistDemo_C.exe main.c -Wl,MyJumplist_C.lib

.PHONY: clean
clean :
	del /q /f *.o
