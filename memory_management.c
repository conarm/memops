#include "kernel/types.h"
#include "user/user.h"
#include "user/memory_management.h"

// initialise head to 0 (NULL)
node * head = 0;

void * 
_malloc(int size)
{
    if (size <= 0 || size > 409600) {
        // return 0 on failure
        return 0;
    }

    void * ptr = 0;

    if (head == 0) {
        // we do not yet have a head - we are calling malloc for the first time
        // create the heap using sbrk()
        head = (node*) sbrk(4096);
        head->size   = 4096 - sizeof(node);
        head->next   = 0;

    }

    // head is defined
    // we will allocate using best-fit!
    node* n = head;
    node* best_fit = 0;
    int best_size = 99999999; // arbitrarily large best size value
    while (n != 0) {
        // compare size of each free node to best so far
        if ( (n->size + sizeof(node) < best_size) && (n->size >= size) ) {
            // if it is a better fit, update our saved best fit pointer and size
            best_size = n->size;
            best_fit = n;
        }
        n = n->next;
    }


    if (best_fit == 0) {
        // there isn't a free location of a size that fits
        // expand the heap by 4096
        node* expanded_head = (node*) sbrk(4096);
        expanded_head->size = 4096 - sizeof(node);
        expanded_head->next = 0;
        node* n = head;
        while (n != 0) {
            if (n->next == 0) {
                // add the start of the expanded heap to the end of the original heap
                n->next = expanded_head;
                break;
            }
            n = n->next;
        }
        // coalesce the added space
        _coalesce();

        // now try to allocate
        return malloc(size);
    }

    // by now the best-fitting block has been identified
    // if there are more than one best free location, the first is picked
    // shrink the free block by the size we want to allocate
    int block_size = size + sizeof(header);
    best_fit->size = best_fit->size - block_size;

    // set up a header at the right before our newly allocated block, which is currently just a gap in memory
    // cast best_fit as void* in order to perform byte-wise pointer arithmetic
    header* hptr = (header *) (((void *)best_fit) + sizeof(node) + best_fit->size);
    hptr->size = size;
    hptr->magic = MAGIC;

    // set our void pointer to the start of the allocated block (after the header)
    ptr = (void*) (hptr + 1);

    return ptr;
}

void 
_free(void *ptr)
{
    if (ptr == 0) {
        // return on NULL (0) ptr
        return;
    }
    header* hptr = (header*) ptr - 1;
    
    // integrity check
    if (hptr->magic != MAGIC) return;

    // start freeing at the address pointed to by hptr - add this location to the free list
    // the new node's size should be set to hptr->size + sizeof(header) - sizeof(node)
    // this gives the full size of the allocated section (including header) minus the size of the new node
    node* new_node = (node *) (void *) hptr;
    new_node->size = hptr->size + sizeof(header) - sizeof(node);

    node* n = head;
    node* prev_n = 0;

    while (n != 0) {
        // iterate through all the free locations in the linked list
        // if our address is HIGHER than our hptr address, place the free block before it
        // otherwise, if our address is LOWER than our hptr address move to the next node
        if ((void *) n > (void *) new_node) break;
        prev_n = n;
        n = n->next;
        continue;
    }

    node* tmp = 0;
    tmp = prev_n->next;
    prev_n->next = (node*) new_node;
    new_node->next = tmp;

    // perform a coalesce operation so to avoid fragmentation
    _coalesce();
}

void 
_coalesce()
{
    node* n = head;
    node* next_n = 0;
    while (n != 0) {
        // iterate over every free block
        next_n = n->next;
        if (((void *) n + (sizeof(node) + n->size)) == (void *) next_n && next_n != 0) {
            // in this case we have found a free block with a free block next to it!
            // combine the blocks' sizes into the first block and change its next value to skip the coalesced block
            n->size = n->size + (sizeof(node) + next_n->size);
            n->next = next_n->next;
            n = head;
            continue;
        }
        n = next_n;
    }
}
