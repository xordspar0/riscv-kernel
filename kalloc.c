#include "memlayout.h"
#include "riscv.h"
#include "printf.h"

void kfree(void *page) {
	//Placeholder for later
}

void * kalloc(void) {
	//TODO change 16 to the actual number of non-allocated pages
	static int64_t next_page = KERNBASE + 16 * PGSIZE;
	
	if(next_page == PHYSTOP) {
		println(STRING("Used all pages implement freeable memory"));
		for(;;){};
	}
	
	//TODO lock this operation
	void * r = (void *)next_page;
	next_page += PGSIZE;
	
	return r;
}
