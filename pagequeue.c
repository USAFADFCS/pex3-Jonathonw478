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
#include <string.h>
#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue* Queue = NULL;
    Queue = (PageQueue*)malloc(sizeof(PageQueue)); // check if i need this + 1?

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
    
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    int depth = 0;
    PqNode* curNode = pq->tail;
    int headPos = 0;

    if (curNode == NULL){
        appendNode(pq, pageNum);
        return -1;
    }

    while (curNode != NULL){
        if (curNode->pageNum == pageNum){
            // HIT
            deleteNode(pq, pq->size - depth - 1);
            appendNode(pq,pageNum);
            return depth;
        }
        curNode = curNode->prev;
        depth++;
    }
    // MISS
    appendNode(pq,pageNum);

    if (pq->size > pq->maxSize) {
        deleteNode(pq, headPos);
    }
    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to  empty
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

/**
 * @brief - allocates a Node and returns a pointer to it
 */
PqNode* createNode(unsigned long pageNum) {
    PqNode* newNode = malloc(sizeof(PqNode));
    newNode->pageNum = pageNum;
    return newNode;
}

/**
 * @brief - addes a new node to the end of the queue
 * */
void appendNode(PageQueue* pq, unsigned long pageNum){
    PqNode* newNode = createNode(pageNum);
    newNode->next = NULL;
    newNode->prev = pq->tail;
    if (pq->tail != NULL){
        pq->tail->next = newNode;
    }
    else {
        pq->head = newNode;
    }
    pq->tail = newNode;
    pq->size++;
}

/**
 * @brief  - deletes a node in the queue
* */
void deleteNode(PageQueue* pq, int position) {

    PqNode* curNode = pq->head;
    int length = pq->size;
    
    // Check if list is empty
    if (curNode == NULL){
        printf("List is Empty\n");
        exit(1);
    }

    // Check if position outside of list
    if (position < 0 || position >= length) {
        printf("Position not part of list");
        exit(1);
    }

    // Check if position is the only remaining element
    if ((pq->head) == (pq->tail)){
        free(pq->head);
        pq->head = NULL;
        pq->tail = NULL;
        pq->size--;
    }   

    // Check if position is the first element in list
    else if (position == 0){
        PqNode* tempPtr = pq->head;
        pq->head = pq->head->next;
        pq->head->prev = NULL;
        free(tempPtr);
    }

    // Check if position is last element in list
    else if (position == length - 1){
        PqNode* nodeBeforeDelete = pq->head;

        while(nodeBeforeDelete->next != pq->tail){
            nodeBeforeDelete = nodeBeforeDelete->next;
        }
        PqNode* oldTail = pq->tail; // this was causing me trouble, forgot to free old tail
        nodeBeforeDelete->next = NULL;
        pq->tail = nodeBeforeDelete;
        free(oldTail);
    }

    // Delete something in the internal part of list
    else {
        PqNode* tempPtr = pq->head;
        PqNode* nodeBeforeDelete = NULL;
        int curPosition = 0;

        while(curPosition != position){
            nodeBeforeDelete = tempPtr;
            tempPtr = tempPtr->next;
            curPosition++;
        }
        nodeBeforeDelete->next = tempPtr->next;
        tempPtr->next->prev = nodeBeforeDelete;
        free(tempPtr);
    }

    pq->size--;
}

// //Keeping this for future reference, but do not need since I have append function now
// /**
//  * @brief - inserts a node into queue
//  * */
// void insertNode(PageQueue *pq, int position, PqNode* newNode, unsigned long pageNum){
//     PqNode* nodeBeforeInsert = NULL;
//     int length = pq->size;
//     int curPosition = 0;
//     PqNode* tempPtr = pq->head;

//     newNode->pageNum = pageNum;

//     if (pq->head == NULL) {
//         pq->head = newNode;
//         pq->tail = newNode;
//         newNode->next = NULL;
//         newNode->prev = NULL;
//     }

//     else if (position < 0 || position > length) {
//         printf("Position not part of list");
//         pqFree(pq);
//         exit(1);
//     }

//     else if (position == 0){
//         newNode->next = pq->head;
//         newNode->prev = NULL;
//         pq->head->prev = newNode;
//         pq->head = newNode;
//     }
//     // insert after everything (last existing node is at length -1)
//     else if (position == length ){
//         newNode->prev = pq->tail;
//         newNode->next = NULL;
//         pq->tail->next = newNode;
//         pq->tail = newNode;
//     }

//     else {    
//         while (curPosition != position){
//             nodeBeforeInsert = tempPtr;
//             tempPtr = tempPtr->next;
//             curPosition++;
//         }
//         newNode->next = tempPtr;
//         nodeBeforeInsert->next = newNode;
//         newNode->prev = nodeBeforeInsert;

//         if (newNode->next != NULL){
//             newNode->next->prev = newNode;
//         }
//     }
//     pq->size++;
// }