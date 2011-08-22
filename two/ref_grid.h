
#ifndef REF_GRID_H
#define REF_GRID_H

#include "ref_defs.h"

BEGIN_C_DECLORATION
typedef struct REF_GRID_STRUCT REF_GRID_STRUCT;
typedef REF_GRID_STRUCT * REF_GRID;
END_C_DECLORATION

#include "ref_node.h"
#include "ref_cell.h"

BEGIN_C_DECLORATION

struct REF_GRID_STRUCT {
  REF_NODE node;

  REF_CELL tet;
  REF_CELL pyr;
  REF_CELL pri;
  REF_CELL hex;

  REF_CELL tri;
  REF_CELL qua;
};

REF_STATUS ref_grid_create( REF_GRID *ref_grid );
REF_STATUS ref_grid_free( REF_GRID ref_grid );

#define ref_grid_node(ref_grid) ((ref_grid)->node)

#define ref_grid_tet(ref_grid) ((ref_grid)->tet)
#define ref_grid_pyr(ref_grid) ((ref_grid)->pyr)
#define ref_grid_pri(ref_grid) ((ref_grid)->pri)
#define ref_grid_hex(ref_grid) ((ref_grid)->hex)

#define ref_grid_tri(ref_grid) ((ref_grid)->tri)
#define ref_grid_qua(ref_grid) ((ref_grid)->qua)

END_C_DECLORATION

#endif /* REF_GRID_H */
