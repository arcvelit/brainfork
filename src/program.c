#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MEMORY_BUFFER_CAP 1024

typedef char byte;

uint64_t sp_mov(uint64_t* sp, uint64_t move)
{
	*sp = (*sp + MEMORY_BUFFER_CAP + move) % MEMORY_BUFFER_CAP;
}

int main()
{
	byte mbuff[MEMORY_BUFFER_CAP];
	memset(&mbuff, 0, MEMORY_BUFFER_CAP);

	uint64_t sp = 0;

	sp_mov(&sp, 1);
	mbuff[sp]+= 8;
	while(mbuff[sp]) {
		sp_mov(&sp, -1);
		mbuff[sp]+= 9;
		sp_mov(&sp, 1);
		mbuff[sp]+= -1;
	}
	sp_mov(&sp, -1);
	putchar(mbuff[sp]);
	sp_mov(&sp, 1);
	mbuff[sp]+= 4;
	while(mbuff[sp]) {
		sp_mov(&sp, -1);
		mbuff[sp]+= 7;
		sp_mov(&sp, 1);
		mbuff[sp]+= -1;
	}
	sp_mov(&sp, -1);
	mbuff[sp]+= 1;
	putchar(mbuff[sp]);
	mbuff[sp]+= 7;
	putchar(mbuff[sp]);
	putchar(mbuff[sp]);
	mbuff[sp]+= 3;
	putchar(mbuff[sp]);
	sp_mov(&sp, 2);
	mbuff[sp]+= 6;
	while(mbuff[sp]) {
		sp_mov(&sp, -1);
		mbuff[sp]+= 7;
		sp_mov(&sp, 1);
		mbuff[sp]+= -1;
	}
	sp_mov(&sp, -1);
	mbuff[sp]+= 2;
	putchar(mbuff[sp]);
	mbuff[sp]+= -12;
	putchar(mbuff[sp]);
	sp_mov(&sp, 1);
	mbuff[sp]+= 6;
	while(mbuff[sp]) {
		sp_mov(&sp, -1);
		mbuff[sp]+= 9;
		sp_mov(&sp, 1);
		mbuff[sp]+= -1;
	}
	sp_mov(&sp, -1);
	mbuff[sp]+= 1;
	putchar(mbuff[sp]);
	sp_mov(&sp, -1);
	putchar(mbuff[sp]);
	mbuff[sp]+= 3;
	putchar(mbuff[sp]);
	mbuff[sp]+= -6;
	putchar(mbuff[sp]);
	mbuff[sp]+= -8;
	putchar(mbuff[sp]);
	sp_mov(&sp, 3);
	mbuff[sp]+= 4;
	while(mbuff[sp]) {
		sp_mov(&sp, -1);
		mbuff[sp]+= 8;
		sp_mov(&sp, 1);
		mbuff[sp]+= -1;
	}
	sp_mov(&sp, -1);
	mbuff[sp]+= 1;
	putchar(mbuff[sp]);
}
