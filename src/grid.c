
/* Michael A. Park
 * Computational Modeling & Simulation Branch
 * NASA Langley Research Center
 * Phone:(757)864-6604
 * Email:m.a.park@larc.nasa.gov 
 */
  
/* $Id$ */

#include <stdlib.h>
#include <stdio.h>
#include "grid.h"

typedef struct N2C N2C;

struct N2C {
  int id;
  N2C *next;
};

#define MAXDEG 200

struct Grid {
  int maxnode;
  int nnode;
  int maxcell;
  int ncell;
  int blankcell;
  N2C **first;
  N2C *current;
  N2C *blank;
  N2C *n2c;
  int *c2n;

  int ngem;
  int gem[MAXDEG];

  int nequ;
  int equ[MAXDEG];
};

//#define EBUG

Grid* gridCreate(int maxnode, int maxcell)
{
  int i, nlist;
  Grid *grid;

  grid = malloc(sizeof(Grid));

  grid->maxnode = maxnode;
  grid->nnode   = 0;
  grid->maxcell = maxcell;
  grid->ncell   = 0;
  nlist         = grid->maxcell*4;

  grid->c2n = malloc(4 * grid->maxcell * sizeof(int));
  for (i=0;i < grid->maxcell; i++ ) {
    grid->c2n[0+4*i] = EMPTY; 
    grid->c2n[1+4*i] = i+1; 
  }
  grid->c2n[1+4*(grid->maxcell-1)] = EMPTY; 
  grid->blankcell = 0;


  grid->first = (N2C **)malloc(grid->maxnode * sizeof(N2C *));

  for (i=0;i < grid->maxnode; i++ ) grid->first[i] = NULL; 
  grid->n2c = (N2C *)malloc( nlist * sizeof(N2C));
  for (i=0;i < nlist-1; i++ ) { // pointer majic?
    grid->n2c[i].id   = EMPTY;
    grid->n2c[i].next = &grid->n2c[i+1];
  }
  grid->n2c[nlist-1].id   = EMPTY;
  grid->n2c[nlist-1].next = NULL;
  grid->blank   = grid->n2c;
  grid->current = NULL;

  grid->ngem = 0;

  return  grid;
}

void gridFree(Grid *grid)
{
  free(grid->c2n);
  free(grid->first);
  free(grid->n2c);
  free(grid);
}

int gridMaxNode(Grid *grid)
{
  return grid->maxnode;
}

int gridNNode(Grid *grid)
{
  return grid->nnode;
}

int gridMaxCell(Grid *grid)
{
  return grid->maxcell;
}

int gridNCell(Grid *grid)
{
  return grid->ncell;
}

int gridNGem(Grid *grid)
{
  return grid->ngem;
}

int gridGem(Grid *grid, int index)
{
  return grid->gem[index];
}

int gridNEqu(Grid *grid)
{
  return grid->nequ;
}

int gridEqu(Grid *grid, int index)
{
  return grid->equ[index];
}

int gridNodeDeg(Grid *grid, int id)
{
  int n;
  n =0;
  for ( gridFirstNodeCell(grid,id); 
	gridValidNodeCell(grid); 
	gridNextNodeCell(grid)) n++;
  return n;
}

bool gridCellExists(Grid *grid, int nodeId, int cellId)
{
  bool exist;
  exist = FALSE;
  for ( gridFirstNodeCell(grid,nodeId); 
	!exist && gridValidNodeCell(grid); 
	gridNextNodeCell(grid)) 
    exist = (cellId == gridCurrentNodeCell(grid));
  return exist;
}

Grid* gridRegisterNodeCell(Grid *grid, int nodeId, int cellId)
{
  N2C *new;
  if (nodeId > grid->maxnode-1) return NULL;
  if (grid->blank == NULL) return NULL;
  new = grid->blank;
  grid->blank = grid->blank->next;
  new->next = grid->first[nodeId];
  grid->first[nodeId] = new;
  new->id = cellId;

  return grid;
}

Grid* gridRemoveNodeCell(Grid *grid, int nodeId, int cellId)
{
  N2C *remove, *previous;
  remove = NULL;

  for ( gridFirstNodeCell(grid,nodeId); 
	gridValidNodeCell(grid); 
	gridNextNodeCell(grid)) {
    if (gridCurrentNodeCell(grid)==cellId) 
      remove = grid->current;
  }

  if (remove == NULL) return NULL;
 
  previous = NULL;

  for ( gridFirstNodeCell(grid,nodeId); 
	gridValidNodeCell(grid); 
	gridNextNodeCell(grid)) {
    if (grid->current != NULL && grid->current->next == remove) 
      previous = grid->current;
  }

  if ( previous == NULL ) {
    grid->first[nodeId] = remove->next;
  }else{
    previous->next = remove->next;
  }

  remove->id = EMPTY;
  remove->next = grid->blank;
  grid->blank = remove;

  return grid;
}

void gridFirstNodeCell(Grid *grid, int nodeId)
{
  if ( nodeId < grid->maxnode ) {
    grid->current = grid->first[nodeId];
  }else{
    grid->current = NULL;
  }
}

void gridNextNodeCell(Grid *grid)
{
  if ( grid->current != NULL) grid->current = grid->current->next;
}

int gridCurrentNodeCell(Grid *grid)
{
  if (grid->current == NULL ) return EMPTY;
  return grid->current->id;
}

bool gridValidNodeCell(Grid *grid)
{
  return (grid->current != NULL);
}

bool gridMoreNodeCell(Grid *grid)
{
  return ( (grid->current != NULL) && (grid->current->next != NULL) );
}

Grid *gridAddCell(Grid *grid, int n0, int n1, int n2, int n3)
{
  int cellId,icell;
  if ( grid->blankcell == EMPTY ) return NULL;
  cellId = grid->blankcell;
  grid->blankcell = grid->c2n[1+4*cellId];
  grid->ncell++;
  
  grid->c2n[0+4*cellId] = n0;
  grid->c2n[1+4*cellId] = n1;
  grid->c2n[2+4*cellId] = n2;
  grid->c2n[3+4*cellId] = n3;
  
  if ( NULL == gridRegisterNodeCell( grid, n0, cellId ) ) return NULL;
  if ( NULL == gridRegisterNodeCell( grid, n1, cellId ) ) return NULL;
  if ( NULL == gridRegisterNodeCell( grid, n2, cellId ) ) return NULL;
  if ( NULL == gridRegisterNodeCell( grid, n3, cellId ) ) return NULL;
  
  return grid;
}

Grid *gridRemoveCell(Grid *grid, int cellId )
{
  if (cellId >= grid->maxcell) return NULL;
  
  if ( grid->ncell <= 0) return NULL;
  grid->ncell--;

  if( ( NULL == gridRemoveNodeCell( grid, grid->c2n[0+4*cellId], cellId ) ) || 
      ( NULL == gridRemoveNodeCell( grid, grid->c2n[1+4*cellId], cellId ) ) ||
      ( NULL == gridRemoveNodeCell( grid, grid->c2n[2+4*cellId], cellId ) ) || 
      ( NULL == gridRemoveNodeCell( grid, grid->c2n[3+4*cellId], cellId ) ) ) 
    return NULL;  

  grid->c2n[0+4*cellId] = EMPTY;
  grid->c2n[1+4*cellId] = grid->blankcell;
  grid->blankcell = cellId;

  return grid;
}

Grid *gridCell(Grid *grid, int cellId, int *nodes )
{
  if ( cellId >= grid->maxcell ) return NULL;
  if ( grid->c2n[4*cellId] == EMPTY ) return NULL;

  nodes[0] = grid->c2n[0+4*cellId];
  nodes[1] = grid->c2n[1+4*cellId];
  nodes[2] = grid->c2n[2+4*cellId];
  nodes[3] = grid->c2n[3+4*cellId];

  return grid;
}

Grid *gridMakeGem(Grid *grid, int n0, int n1 )
{
  int cellId;
  grid->ngem = 0;

  for ( gridFirstNodeCell(grid,n0); 
	gridValidNodeCell(grid); 
	gridNextNodeCell(grid)) {

    cellId = gridCurrentNodeCell(grid);
    if ( n1 == grid->c2n[0+4*cellId] ||
	 n1 == grid->c2n[1+4*cellId] ||
	 n1 == grid->c2n[2+4*cellId] ||
	 n1 == grid->c2n[3+4*cellId] ) {
      if (grid->ngem >= MAXDEG) { 
	grid->ngem = 0; 
	return NULL; 
      }
      grid->gem[grid->ngem] = cellId;
      grid->ngem++;
    }
  }

  return grid;
}

Grid *gridOrient(Grid *grid, int *c, int *n )
{

  /* 0 leads */
  if ( n[0] == c[0] && n[1] == c[1] ){ 
    n[2] = c[2]; 
    n[3] = c[3]; 
    return grid; 
  }
  if ( n[0] == c[0] && n[1] == c[2] ){ 
    n[2] = c[3]; 
    n[3] = c[1]; 
    return grid; 
  }
  if ( n[0] == c[0] && n[1] == c[3] ) {
    n[2] = c[1];
    n[3] = c[2];
    return grid;
  }

  /* 1 leads */
  if ( n[0] == c[1] && n[1] == c[0] ) {
    n[2] = c[3];
    n[3] = c[2];
    return grid;
  }
  if ( n[0] == c[2] && n[1] == c[0] ) {
    n[2] = c[1];
    n[3] = c[3];
    return grid;
  }
  if ( n[0] == c[3] && n[1] == c[0] ) {
    n[2] = c[2];
    n[3] = c[1];
    return grid;
  }

  /* 2 leads */
  if ( n[0] == c[2] && n[1] == c[3] ) {
    n[2] = c[0];
    n[3] = c[1];
    return grid;
  }
  if ( n[0] == c[3] && n[1] == c[1] ) {
    n[2] = c[0];
    n[3] = c[2];
    return grid;
  }
  if ( n[0] == c[1] && n[1] == c[2] ) {
    n[2] = c[0];
    n[3] = c[3];
    return grid;
  }

  /* 3 leads */
  if ( n[0] == c[3] && n[1] == c[2] ) {
    n[2] = c[1];
    n[3] = c[0];
    return grid;
  }
  if ( n[0] == c[1] && n[1] == c[3] ) {
    n[2] = c[2];
    n[3] = c[0];
    return grid;
  }
  if ( n[0] == c[2] && n[1] == c[1] ) {
    n[2] = c[3];
    n[3] = c[0];
    return grid;
  }

  return NULL;
}

Grid *gridEquator(Grid *grid, int n0, int n1 )
{
  int igem, iequ, cell[4], nodes[4];
  bool gap, found;
  grid->nequ = 0;

  if ( NULL == gridMakeGem( grid, n0, n1) ) return NULL;

  if ( grid->ngem == (MAXDEG-1) ) return NULL;
  if ( grid->ngem == 0 ) return grid;

  nodes[0] = n0;
  nodes[1] = n1;

  gap = FALSE;

  gridOrient( grid, &grid->c2n[4*grid->gem[0]], nodes );

  grid->equ[0]= nodes[3];

  // put and not found in loops

  for ( iequ=1 ; iequ <= grid->ngem && !gap ; iequ++ ){
    found = FALSE;
    for( igem=0 ; igem<grid->ngem ; igem++ ){
      gridOrient( grid, &grid->c2n[4*grid->gem[igem]], nodes );
      if ( grid->equ[iequ-1] == nodes[2] ) {
	grid->equ[iequ] = nodes[3];
	found = TRUE;
      }
    }
    if (!found) {
      gap = TRUE;
      grid->equ[grid->ngem] = grid->equ[iequ-1];
    }
  }
  
  if ( gap ) {

    for ( iequ=grid->ngem-1 ; iequ >= 0 ; iequ-- ){
      found = FALSE;
      for( igem=0 ; igem<grid->ngem ; igem++ ){
	gridOrient( grid, &grid->c2n[4*grid->gem[igem]], nodes );
	if ( grid->equ[iequ+1] == nodes[3] ) {
	  grid->equ[iequ] = nodes[2];
	  found = TRUE;
	}
      }
      if (!found) return NULL;
    }
    grid->nequ = grid->ngem+1;
    grid->equ[grid->ngem+1] = grid->equ[0];
  }else{
    grid->nequ = grid->ngem;
  }

  return grid;
}

Grid *gridSwap(Grid *grid, int n0, int n1 )
{
  int i, nodes[4][4];
  if ( NULL == gridEquator( grid, n0, n1) ) return NULL;
  
  nodes[0][0]=n0;
  nodes[0][1]=grid->equ[0];
  nodes[0][2]=grid->equ[1];
  nodes[0][3]=grid->equ[2];
  nodes[1][0]=n0;
  nodes[1][1]=grid->equ[2];
  nodes[1][2]=grid->equ[3];
  nodes[1][3]=grid->equ[4];
  nodes[2][0]=n1;
  nodes[2][1]=grid->equ[0];
  nodes[2][2]=grid->equ[2];
  nodes[2][3]=grid->equ[1];
  nodes[3][0]=n1;
  nodes[3][1]=grid->equ[2];
  nodes[3][2]=grid->equ[0];
  nodes[3][3]=grid->equ[3];

  for ( i = 0 ; i < grid->ngem ; i++ ) 
    gridRemoveCell( grid, grid->gem[i] );
     
  for ( i = 0 ; i < 4 ; i++ )
    gridAddCell( grid, nodes[i][0], nodes[i][1], nodes[i][2], nodes[i][3] );

  return grid;
}

