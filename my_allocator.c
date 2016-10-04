/* 
    File: my_allocator.c

    Author: <your name>
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include<stdio.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

unsigned int total_mem = 0;
unsigned int min_block_size = 0;
Addr freelist = NULL;
struct node** ptr_array = NULL;

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

//Custom Function for Power
unsigned int power(unsigned int base, unsigned int exponent) {
    unsigned int result = 1;
    if (exponent == 0) {return  1;}
	for (unsigned int i = 1; i <= exponent; i++)
	{
		result = result * base;
	}
    return result;
}

//Function for innserting a pointer into pointer array
void ptr_array_insert(struct node* addr, unsigned int index) {
	if (ptr_array[index] == NULL) {
		ptr_array[index] = addr;
		ptr_array[index]->next = NULL;
		ptr_array[index]->free = true;
		ptr_array[index]->size = total_mem / power(2, index);
	}
	else {
		struct node* temp = ptr_array[index];
		ptr_array[index] = addr;
		ptr_array[index]->next = temp;
		ptr_array[index]->free = true;
		ptr_array[index]->size = total_mem / power(2, index);
	}
}

//Function for deleting a pointer from pointer array
void ptr_array_delete(struct node* addr, unsigned int index) {
	struct node* temp = ptr_array[index];
	if (temp != addr) {
		while (temp->next != addr) {
			temp = temp->next;
		}
		temp->next = addr->next;
		addr = NULL;
		
	}
	if (ptr_array[index]->next == NULL) {
		ptr_array[index] = NULL;
	}
	else {
		ptr_array[index] = ptr_array[index]->next;
	}
}

//Function for spliting the memory
Addr break_it(struct node* addr, unsigned int index) {
	struct node* temp = addr;
	unsigned int offset = total_mem / power(2, index);
	temp = (struct node*)((char*)temp + offset);
	ptr_array_delete(addr, index - 1);
	ptr_array_insert(temp, index);
	ptr_array_insert(addr, index);
	return addr;
}

//Resursive function for breaking the memory
Addr break_mem(unsigned int index) {
	if (ptr_array[index] != NULL) {
		if (ptr_array[index]->free == false) {
			return NULL;
		}
		return ptr_array[index];
	}
	else {
		if (index == 0) { return NULL; }
		Addr mod_addr = NULL;
		mod_addr = break_mem(index - 1);
		if (mod_addr != NULL) {
			mod_addr = break_it(mod_addr, index);
		}
		return mod_addr;
	}
}

//Function for printing the count of free blocks at each index of the pointer array
void PrintList() {
    unsigned int number_of_possible_block_sizes = 1;
    unsigned int count = 0;
    unsigned int block_size = min_block_size;
    unsigned int memory = total_mem;
    struct node* temp = NULL;
    
    while (block_size != memory) {
        block_size = block_size * 2;
        ++number_of_possible_block_sizes;
    }
    
    for(unsigned int i = 0; i < number_of_possible_block_sizes; ++i){
        count = 0;
        temp = ptr_array[i];
        while(temp != NULL){
            ++count;
            temp = temp->next;
        }
        printf("Count of Free Blocks in Index %u: %u\n", i, count);
    }
    printf("\n");
}

unsigned int init_allocator(unsigned int _block_size, unsigned int _memory_size_wanted) {
	/* This function will increase block size by doubling it until 
		at least the memory size desired is reached*/
	unsigned int check = 2;
	unsigned int number_of_possible_block_sizes = 1;

	//Make sure a valid block size is entered
	if (_block_size < 1) {
		return 0;
	}

	//Round block size up to the nearest 2 to the power of x
	if (!(_block_size > 0 && !(_block_size & (_block_size - 1)))) {
		while (check < _block_size) {
			check = check * 2;
		}
		_block_size = check;
		check = 2;
	}
    
    //Set the minimum block size
    min_block_size = _block_size;

	//Round memory size wanted to the nearest 2 to the power of x
	if (!(_memory_size_wanted > 0 && !(_memory_size_wanted & (_memory_size_wanted - 1)))) {
		while (check < _memory_size_wanted) {
			check = check * 2;
		}
		_memory_size_wanted = check;
	}

	//Determine the number of possible splits that can be done with memory size
	while (_block_size != _memory_size_wanted) {
		_block_size = _block_size * 2;
		++number_of_possible_block_sizes;
	}

    //Set the total memory size
	total_mem = _memory_size_wanted;
    
	//Create the pointer array
	ptr_array = (struct node**)malloc(number_of_possible_block_sizes * sizeof(struct node*));
    
	//Create the free list and place the pointer returned in the first index of the pointer array
	*ptr_array = malloc(_memory_size_wanted);
    
    //Save the pointer for use when releasing the memory
	freelist = *ptr_array;
    
    //Initialize the contents of the pointer array
	ptr_array[0]->next = NULL;
	ptr_array[0]->free = true;
	ptr_array[0]->size = _memory_size_wanted;
	for (unsigned int i = 1; i < number_of_possible_block_sizes; i++){
		ptr_array[i] = NULL;
	}
    
	return _memory_size_wanted;
}

void release_allocator(void) {
	/* This function returns any allocated memory to the operating system.
	After this function is called, any allocation fails.
	*/
	total_mem = 0;
	free(freelist);
	free(ptr_array);
}

extern Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
	unsigned int mem_needed = total_mem;
	unsigned int index_ptr_array = 0;
	struct node* addr;
	if (_length > mem_needed - sizeof(struct node) || sizeof(struct node) > mem_needed ) {
		return NULL; 
	}
	while (sizeof(struct node) <= mem_needed && _length <= mem_needed-sizeof(struct node) && mem_needed >= min_block_size) {
		mem_needed = mem_needed / 2;
		++index_ptr_array;
	}
	--index_ptr_array;

	addr = break_mem(index_ptr_array);
	if (addr != NULL) {
		addr->free = false;
		ptr_array_delete(addr, index_ptr_array);
	}

    //Give the user the remaining memory
    return (char*)addr + sizeof(struct node);
}

//Function for freeing memory. Calls interal free to perform the recursion
extern int my_free(Addr _a) {
    //Subtract the size of the node to obtain the correct address
	struct node* addr = (struct node*)((char *)_a - sizeof(struct node));
    return internal_free(addr);
}

//Recursive function to combine blocks if its buddy is free
extern int internal_free(Addr _a) {
    struct node* addr = (struct node*)_a;
    unsigned int mem_available = total_mem;
    unsigned int index_ptr_array = 0;
    while (addr->size != mem_available && sizeof(struct node) <= mem_available && mem_available > min_block_size) {
        mem_available = mem_available / 2;
        ++index_ptr_array;
    }
    ptr_array_insert(addr, index_ptr_array);
    if (index_ptr_array == 0) { return 0; }
    struct node* buddy = (struct node*)((((char *)addr-(char *)freelist) ^ (addr->size)) + (char *)freelist);
    
    //Check if the buddy is free as well. If so combine the blocks
    if (addr->size == buddy->size && buddy->free == true) {
        //Check if addr or buddy comes first in memory
        if(addr < buddy){
            ptr_array_delete(addr, index_ptr_array);
            ptr_array_delete(buddy, index_ptr_array);
            addr->size = addr->size * 2;
            internal_free(addr);
        }
        else{
            ptr_array_delete(buddy, index_ptr_array);
            ptr_array_delete(addr, index_ptr_array);
            buddy->size = buddy->size * 2;
            internal_free(buddy);
        }
    }
    
    return 0;
}
