#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MEMORY_BUFFER_CAP 1024
#define sp_mov(sp, inc) sp = (sp + MEMORY_BUFFER_CAP + inc) % MEMORY_BUFFER_CAP

typedef char byte;

int main()
{
	// BEGIN PROGRAM
	byte mbuff[MEMORY_BUFFER_CAP];
	memset(&mbuff, 0, MEMORY_BUFFER_CAP);

	size_t sp = 0;

	sp_mov(sp, 1);
	mbuff[sp]+= 8;
	while(mbuff[sp]) {
		sp_mov(sp, 4294967295);
		mbuff[sp]+= 9;
		sp_mov(sp, 1);
		mbuff[sp]+= 4294967295;
	}
	sp_mov(sp, 4294967295);
	putchar(mbuff[sp]);
	sp_mov(sp, 1);
	mbuff[sp]+= 4;
	while(mbuff[sp]) {
		sp_mov(sp, 4294967295);
		mbuff[sp]+= 7;
		sp_mov(sp, 1);
		mbuff[sp]+= 4294967295;
	}
	sp_mov(sp, 4294967295);
	mbuff[sp]+= 1;
	putchar(mbuff[sp]);
	mbuff[sp]+= 7;
	putchar(mbuff[sp]);
	putchar(mbuff[sp]);
	mbuff[sp]+= 3;
	putchar(mbuff[sp]);
	sp_mov(sp, 2);
	mbuff[sp]+= 6;
	while(mbuff[sp]) {
		sp_mov(sp, 4294967295);
		mbuff[sp]+= 7;
		sp_mov(sp, 1);
		mbuff[sp]+= 4294967295;
	}
	sp_mov(sp, 4294967295);
	mbuff[sp]+= 2;
	putchar(mbuff[sp]);
	mbuff[sp]+= 4294967284;
	putchar(mbuff[sp]);
	sp_mov(sp, 1);
	mbuff[sp]+= 6;
	while(mbuff[sp]) {
		sp_mov(sp, 4294967295);
		mbuff[sp]+= 9;
		sp_mov(sp, 1);
		mbuff[sp]+= 4294967295;
	}
	sp_mov(sp, 4294967295);
	mbuff[sp]+= 1;
	putchar(mbuff[sp]);
	sp_mov(sp, 4294967295);
	putchar(mbuff[sp]);
	mbuff[sp]+= 3;
	putchar(mbuff[sp]);
	mbuff[sp]+= 4294967290;
	putchar(mbuff[sp]);
	mbuff[sp]+= 4294967288;
	putchar(mbuff[sp]);
	sp_mov(sp, 3);
	mbuff[sp]+= 4;
	while(mbuff[sp]) {
		sp_mov(sp, 4294967295);
		mbuff[sp]+= 8;
		sp_mov(sp, 1);
		mbuff[sp]+= 4294967295;
	}
	sp_mov(sp, 4294967295);
	mbuff[sp]+= 1;
	putchar(mbuff[sp]);
	// EXIT PROGRAM
}
