/* 
    File: my_allocator.h

    Author: R.Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/08

    Modified:

*/

#ifndef _my_allocator_h_                   // include file only once
#define _my_allocator_h_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

typedef void * Addr;

//Enum for bool
enum _bool{
    false,
    true
};
typedef enum _bool bool;

//Structure for node
struct node {
	//Header
	struct node* next;
	//Payload
	bool free;
	int size;
};
/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* MODULE   MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

Addr break_mem(unsigned int index);
/* This function recursively calls itself as need to break up the memory
 into different block sizes. */

Addr break_it(struct node* addr, unsigned int index);
/* This function is called by break_mem to actually break up the memory. */

void ptr_array_insert(struct node* addr, unsigned int index);
/* This function inserts a pointer into the pointer array. */

void ptr_array_delete(struct node* addr, unsigned int index);
/* This function deletes a pointer from the pointer array. */

unsigned int pow(unsigned int base, unsigned int exponent);
/* This is a custom implementation of the power function. */

void PrintList();
/* This function prints the pointer array with the amount of free blocks
 for each size. */

extern int internal_free(Addr _a);
/* This function is called by my_free and adds the memory back to the 
 free list. It recursively calls itself as need to combine the blocks if 
 the buddy is free as well. */

unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length);
/* This function initializes the memory allocator and makes a portion of 
   ’_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
   its minimal unit of allocation. The function returns the amount of 
   memory made available to the allocator. If an error occurred, 
   it returns 0. */

void release_allocator(void);
/* This function returns any allocated memory to the operating system. 
   After this function is called, any allocation fails. */

Addr my_malloc(unsigned int _length); 
/* Allocate _length number of bytes of free memory and returns the 
   address of the allocated portion. Returns 0 when out of memory. */ 

int my_free(Addr _a); 
/* Frees the section of physical memory previously allocated 
   using ’my_malloc’. Returns 0 if everything ok. */ 

#endif 
