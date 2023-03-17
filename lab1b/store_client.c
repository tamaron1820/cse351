/*
 * CSE 351 Lab 1b (Manipulating Bits in C)
 *
 * Name(s):  Tatsuhiko Araki
 * NetID(s): taraki
 *
 * This is a file for managing a store of various aisles, represented by an
 * array of 64-bit integers. See aisle_manager.c for details on the aisle
 * layout and descriptions of the aisle functions that you may call here.
 *
 * Written by Porter Jones (pbjones@cs.washington.edu)
 */

#include <stddef.h>  // To be able to use NULL
#include "aisle_manager.h"
#include "store_client.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];


/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Prioritizes and fills sections with lower addresses
 * first. Sections with lower addresses should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom() {
  // TODO: implement this function
  int num =0;
  unsigned short id;
  for ( int i=0;i<NUM_AISLES ;i++) {
    for ( int j =0; j<SECTIONS_PER_AISLE; j++ ){
      num =10- num_items(&aisles[i],j);
      if (num >=0) {
	id = get_id(&aisles[i],j);
	add_items(&aisles[i],j,stockroom[id]);
	stockroom[id]-=num;
	if(stockroom[id] < 0) {
	  stockroom[id] =0;
	}
      }
    }
  }
}

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short id, int num) {
  // TODO: implement this function
  int total=0;
  int exist_num=0;
  int most =num;
  int stock_num,stock_left;
  unsigned short possible_id;
  for (int i=0;i<NUM_AISLES; i++ ) {
    for (int j=0; j<SECTIONS_PER_AISLE; j++) {
      possible_id = get_id(&aisles[i],j);
      if(id == possible_id) {
	exist_num=num_items(&aisles[i],j);
	remove_items(&aisles[i],j,num);
	num-=exist_num;
	total+=exist_num;
	if(num <0){
	  return most;
	}
      }
    }
  }
  if (num >0) {
    stock_num=stockroom[id];
    if(stock_num >= num) {
      stock_left= stock_num-num;
      stockroom[id]=stock_left;
      total+=num;
    } else{
      total+=stock_num;
      stockroom[id]=0;
    }
  }
  return total;
}

/* Return a pointer to the first section in the aisles with the given item id
 * that has no items in it or NULL if no such section exists. Only consider
 * items stored in sections in the aisles (i.e., ignore anything in the
 * stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* empty_section_with_id(unsigned short id) {
  // TODO: implement this function
  int num;
  unsigned short possible_id;
  unsigned short* point;
  for (int i=0;i<NUM_AISLES; i++ ) {
    for (int j=0; j<SECTIONS_PER_AISLE; j++) {
      possible_id = get_id(&aisles[i],j);
      if(id ==possible_id) {
	num = num_items(&aisles[i],j);
	if(num ==0) {
	  point=(unsigned short*)(&aisles[i]) + j;
	  return point;
	}
      }
    }
  }
  return NULL;
}

/* Return a pointer to the section with the most items in the store. Only
 * consider items stored in sections in the aisles (i.e., ignore anything in
 * the stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* section_with_most_items() {
  // TODO: implement this function
  int num;
  int max=0;
  unsigned short* point;
  for (int i=9;i>=0; i-- ) {
    for (int j=3; j>=0; j--) {
	num = num_items(&aisles[i],j);
	if(num >= max) {
	  max = num;
	  point = (unsigned short*)(&aisles[i]) +j;
	}
    }
  }
  return point;
}