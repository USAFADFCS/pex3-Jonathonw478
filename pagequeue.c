/** pagequeue.c
 * ===========================================================
 * Name: _______________________, __ ___ 2026
 * Section: CS483 / ____
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue* Queue = NULL;
    Queue = (PageQueue*)malloc(sizeof(PageQueue) * (maxSize + 1)); // check if i need this + 1?

    if (Queue == NULL) {
        // Handle Failure
        return NULL;
    }

    Queue->head = NULL;
    Queue->tail = NULL;
    Queue->size = 0;
    Queue->maxSize = maxSize;
    return Queue;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    int curPosition = 0;
    PqNode* curNode = pq->tail;

    if (curNode == NULL){
        printf("List is Empty");
        return -1;
    }

    while (curNode->pageNum != pageNum){
        if (curPosition == pq->size-1){ // check if size - 1 is correct here...
            return -1;
        }
        curNode = curNode->prev;
        curPosition++;
    }

    return curPosition;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode* curNode = pq->head;
    PqNode* nextNode = NULL;

    // Loop through all nodes and free
    while (curNode != NULL) {
        nextNode = curNode->next;
        free(curNode);
        curNode = nextNode;
    }

    // Free final node and all data
    pq->head = NULL;
    pq->tail = NULL;
    pq->size = 0;

    free(pq);
    pq = NULL;
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    int count = 0;
    PqNode* curNode = pq->head;
    
    if (curNode == NULL){
        printf("List is Empty");
        exit(1);
    }

    while (pq->size > count){
        if (count == 0) {
            printf("Head: %ld\n", curNode->pageNum);
        }
        else if(count == pq->size - 1) {
            printf("Tail: %ld\n", curNode->pageNum);
        }
        else {
            printf("%d: %ld\n", count, curNode->pageNum);
        }
        curNode = curNode->next;
        count++;
    }
}