// Tuesday, January 22, 2018

#include <jtk/support/log/Logger.h>
#include <com/onecube/zen/virtual-machine/memory/allocator/BestFitAllocator.h>

/*******************************************************************************
 * BestFitAllocator                                                            *
 *******************************************************************************/

/* Allocate */

uint8_t* zen_BestFitAllocator_allocate(zen_BestFitAllocator_t* allocator,
    int32_t headSize, int32_t bodySize) {
    jtk_Assert_assertObject(allocator, "The specified best-fit allocator is null.");
    jtk_Assert_assertTrue(headSize >= 0, "The specified head size is negative.");
    jtk_Assert_assertTrue(bodySize >= 0, "The specified body size is negative.");
    
    /* Initially, the allocator assumes that there is no best cell. */
    uint8_t* bestCell = NULL;
    /* Initially, the allocator assumes that the best cell size is "infinite". */
    int32_t bestCellSize = JTK_INTEGER_MAX_VALUE;
    
    /* Initially, the allocator assume that the previous cell processed as the
     * head of the free-list.
     */
    uint8_t* previousCell = allocator->m_head;
    
    /* The request size is equal to the sum of head and body sizes. */
    int32_t requestSize = headSize + bodySize;
    
    while (true) {
        /* The current cell is the cell next to the previous cell. For the first
         * iteration, it is always the second cell because we assume that the
         * head cell was processed.
         */
        uint8_t* current = zen_BestFitAllocator_getNextFreeCell(allocator);
        /* Compute the size of the current cell. */
        int32_t currentCellSize = zen_BestFitAllocator_getFreeCellSize(current);
        
        /* Check if the current cell is null if the current cell size is equal
         * to the request size.
         */
        if ((current == NULL) || (currentCellSize == requestSize)) {
            if (current != NULL) {
                /* If the current cell is not null, the allocator has found the best
                 * cell.
                 */
                previousBest = previous;
                best = current;
            }
            else if (best == NULL) {
                xjtk_Logger_debug(ZEN_BEST_FIT_ALLOCATOR_TAG, "Failed to allocate memory. (size = %d, headSize = %d, bodySize = %d)",
                    requestSize, headSize, bodySize);
                
                /* At this point, the memory chunk assigned to this allocator
                 * is exhausted.
                 */
                return NULL;
            }
            /* Allocate and move the free cell to the list of allocated cells. */
            return zen_BestFitAllocator_allocateFreeCell(allocator, previousBestCell, bestCell, requestSize);
        }
        /* Check if the current cell size is smaller than the request size, or if 
         * the best cell size less than the current cell size.
         */
        else if ((currentCellSize < requestSize) || (bestCellSize < currentCellSize)) {
            previousCell = currentCell;
        }
        else {
            /* Update the best cell, the best cell size, and previous best cell
             * and hope for the best (no pun intended).
             */
            bestCell = currentCell;
            bestCellSize = zen_BestFitAllocator_getFreeCellSize(allocator, currentCell);
            previousBestCell = previousCell;
        }
    }
    
    /* Control should never reach here. */
    return NULL;
}