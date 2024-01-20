#include "mem.h"
#include "printf.h"
#include "riscv.h"
#include "strings.h"

// Saved registers for kernel context switches.
struct context {
	uint64_t ra;
	uint64_t sp;

	// callee-saved
	uint64_t s0;
	uint64_t s1;
	uint64_t s2;
	uint64_t s3;
	uint64_t s4;
	uint64_t s5;
	uint64_t s6;
	uint64_t s7;
	uint64_t s8;
	uint64_t s9;
	uint64_t s10;
	uint64_t s11;
};

void swtch(struct context*, struct context*);
void userret();
void uservec();

struct trapframe {
	/*   0 */ uint64_t kernel_satp;   // kernel page table
	/*   8 */ uint64_t kernel_sp;     // top of process's kernel stack
	/*  16 */ uint64_t kernel_trap;   // usertrap()
	/*  24 */ uint64_t epc;           // saved user program counter
	/*  32 */ uint64_t kernel_hartid; // saved kernel tp
	/*  40 */ uint64_t ra;
	/*  48 */ uint64_t sp;
	/*  56 */ uint64_t gp;
	/*  64 */ uint64_t tp;
	/*  72 */ uint64_t t0;
	/*  80 */ uint64_t t1;
	/*  88 */ uint64_t t2;
	/*  96 */ uint64_t s0;
	/* 104 */ uint64_t s1;
	/* 112 */ uint64_t a0;
	/* 120 */ uint64_t a1;
	/* 128 */ uint64_t a2;
	/* 136 */ uint64_t a3;
	/* 144 */ uint64_t a4;
	/* 152 */ uint64_t a5;
	/* 160 */ uint64_t a6;
	/* 168 */ uint64_t a7;
	/* 176 */ uint64_t s2;
	/* 184 */ uint64_t s3;
	/* 192 */ uint64_t s4;
	/* 200 */ uint64_t s5;
	/* 208 */ uint64_t s6;
	/* 216 */ uint64_t s7;
	/* 224 */ uint64_t s8;
	/* 232 */ uint64_t s9;
	/* 240 */ uint64_t s10;
	/* 248 */ uint64_t s11;
	/* 256 */ uint64_t t3;
	/* 264 */ uint64_t t4;
	/* 272 */ uint64_t t5;
	/* 280 */ uint64_t t6;
};

uint8_t usercode[] = {
	0x93, 0x08, 0x70, 0x00, // li a7, 7
	0x73, 0x00, 0x00, 0x00, // ecall
	0xef, 0xf0, 0x9f, 0xff  // jal start
};

void syscall_hello() {
	println(STRING("Userspace says hello!"));
}

struct context kcontext;
struct context ucontext;
struct trapframe utrapframe;

void syscall_handler() {
	println(STRING("Handling syscall\n"));
	switch (utrapframe.a7) {
		case 7:
		syscall_hello();
		break;
	}
}

void usertrapret() {
	unsigned long x = r_sstatus();
	x &= ~SSTATUS_SPP; // clear SPP to 0 for user mode
	x |= SSTATUS_SPIE; // enable interrupts in user mode
	// w_sstatus(x); Test out usercode in supervisor mode for now.

	asm volatile("sret"); // skip user trap handling code for now.
}

void main() {
	printf(STRING("hello\n"));

	printf(STRING("%d %d\n"), 0, 1);
	printf(STRING("%d %d\n"), 1, -1);
	printf(STRING("%d\n"), -1);
	printf(STRING("%d\n"), 0x7FFFFFFFFFFFFFFF);
	printf(STRING("%d\n"), -0x7FFFFFFFFFFFFFFF);

	char c = '%';
	println(string_init(&c, 1));

	printf(STRING("%d%%\n"), 99);

	printf(STRING("usercode addr: %d\n"), usercode);
	print_hex_data(usercode, sizeof(usercode));

	// Start userspace code.
	w_stvec((uint64_t)syscall_handler);

	memset(&ucontext, 0, sizeof(ucontext));
	ucontext.ra = (uint64_t)usertrapret;

	memmove((void *)(uint64_t)0x80001000, &usercode, sizeof(usercode));
	utrapframe.kernel_trap = (uint64_t)&syscall_handler;

	w_sepc((uint64_t)usercode);
	usertrapret(); // Skip context switching for now.

	for (;;) {
		swtch(&kcontext, &ucontext);
	}
}
