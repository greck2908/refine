
/* Queue for storing grid transformations that can be shared to off-processors
 * 
 * Michael A. Park
 * Computational Modeling & Simulation Branch
 * NASA Langley Research Center
 * Phone:(757)864-6604
 * Email:m.a.park@larc.nasa.gov 
 */
  
/* $Id$ */

#include <stdlib.h>
#include "queue.h"

Queue* queueCreate(  )
{
  Queue *queue;
  queue = malloc( sizeof(Queue) );
  queue->maxTransactions = 100;
  queue->removedCells = malloc( queue->maxTransactions * sizeof(int) );
  queue->addedCells = malloc( queue->maxTransactions * sizeof(int) );
  queue->maxRemovedCells = 100;
  queue->removedCellNodes = malloc( 4 * queue->maxRemovedCells * sizeof(int) );
  queue->maxAddedCells = 100;
  queue->addedCellNodes = malloc( 5 * queue->maxAddedCells * sizeof(int) );
  queue->addedCellXYZs = malloc( 12 * queue->maxAddedCells * sizeof(double) );
  return queueReset(queue);
}

void queueFree( Queue *queue )
{
  free( queue->addedCellXYZs );
  free( queue->addedCellNodes );
  free( queue->removedCellNodes );
  free( queue->addedCells );
  free( queue->removedCells );
  free( queue );
}

Queue *queueReset( Queue *queue )
{
  queue->transactions = 1;
  queue->nAddedCells = 0;
  queue->nRemovedCells = 0;
  queue->addedCells[0] = 0;
  queue->removedCells[0] = 0;
  return queue;
}

int queueTransactions( Queue *queue )
{
  return queue->transactions;
}

Queue *queueNewTransaction( Queue *queue )
{
  if (queue->transactions>=queue->maxTransactions) {
    queue->maxTransactions += 100;
    queue->removedCells = realloc( queue->removedCells, 
				   queue->maxTransactions * sizeof(int) );
    queue->addedCells   = realloc( queue->addedCells, 
				   queue->maxTransactions * sizeof(int) );
  }
  queue->removedCells[queue->transactions] = 0;
  queue->addedCells[queue->transactions] = 0;
  queue->transactions++;
  return queue;
}

Queue *queueRemoveCell( Queue *queue, int *nodes )
{
  int i;
  queue->removedCells[queue->transactions-1]++;
  if (queue->nRemovedCells>=queue->maxRemovedCells) {
    queue->maxRemovedCells += 100;
    queue->removedCellNodes = realloc( queue->removedCellNodes, 
				       4 * queue->maxRemovedCells* sizeof(int));
  }
  for (i=0;i<4;i++) queue->removedCellNodes[i+4*queue->nRemovedCells]= nodes[i];
  queue->nRemovedCells++;
  return queue;
}

int queueRemovedCells( Queue *queue, int transaction )
{
  if ( transaction<0 || transaction>=queueTransactions(queue) ) return EMPTY;
  return queue->removedCells[transaction];
}

Queue *queueRemovedCellNodes( Queue *queue, int index, int *nodes )
{
  int i;
  if ( index<0 || index>queue->nRemovedCells ) return NULL;
  for (i=0;i<4;i++) nodes[i] = queue->removedCellNodes[i+4*index];
  return queue;
}

Queue *queueAddCell( Queue *queue, int *nodes, double *xyzs )
{
  int i;
  queue->addedCells[queue->transactions-1]++;
  if (queue->nAddedCells>=queue->maxAddedCells) {
    queue->maxAddedCells += 100;
    queue->addedCellNodes = realloc( queue->addedCellNodes, 
				     5 * queue->maxAddedCells * sizeof(int) );
    queue->addedCellXYZs  = realloc( queue->addedCellXYZs, 
				     12 * queue->maxAddedCells* sizeof(double));
  }
  for (i=0;i<5 ;i++) queue->addedCellNodes[i+5*queue->nAddedCells] = nodes[i];
  for (i=0;i<12;i++) queue->addedCellXYZs[i+12*queue->nAddedCells] = xyzs[i];
  queue->nAddedCells++;
  return queue;
}

int queueAddedCells( Queue *queue, int transaction )
{
  if ( transaction<0 || transaction>=queueTransactions(queue) ) return EMPTY;
  return queue->addedCells[transaction];
}

Queue *queueAddedCellNodes( Queue *queue, int index, int *nodes )
{
  int i;
  if ( index<0 || index>queue->nAddedCells ) return NULL;
  for (i=0;i<5;i++) nodes[i] = queue->addedCellNodes[i+5*index];
  return queue;
}

Queue *queueAddedCellXYZs( Queue *queue, int index, double *xyzs )
{
  int i;
  if ( index<0 || index>queue->nAddedCells ) return NULL;
  for (i=0;i<12;i++) xyzs[i] = queue->addedCellXYZs[i+12*index];
  return queue;
}

int queueRemovedFaces( Queue *queue, int transaction )
{
  if ( transaction<0 || transaction>=queueTransactions(queue) ) return EMPTY;
  return queue->removedFaces[transaction];
}

int queueAddedFaces( Queue *queue, int transaction )
{
  if ( transaction<0 || transaction>=queueTransactions(queue) ) return EMPTY;
  return queue->addedFaces[transaction];
}
