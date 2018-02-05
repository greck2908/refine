
/* Copyright 2014 United States Government as represented by the
 * Administrator of the National Aeronautics and Space
 * Administration. No copyright is claimed in the United States under
 * Title 17, U.S. Code.  All Other Rights Reserved.
 *
 * The refine platform is licensed under the Apache License, Version
 * 2.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef REF_GRID_H
#define REF_GRID_H

#include "ref_defs.h"

BEGIN_C_DECLORATION
typedef struct REF_GRID_STRUCT REF_GRID_STRUCT;
typedef REF_GRID_STRUCT * REF_GRID;
END_C_DECLORATION

#include "ref_mpi.h"
#include "ref_node.h"
#include "ref_cell.h"
#include "ref_geom.h"
#include "ref_gather.h"
#include "ref_adapt.h"

BEGIN_C_DECLORATION

struct REF_GRID_STRUCT {
  REF_MPI mpi;
  REF_NODE node;

  REF_CELL cell[5];

  REF_CELL edg;
  REF_CELL tri;
  REF_CELL qua;

  REF_GEOM geom;
  REF_GATHER gather;
  REF_ADAPT adapt;

  REF_BOOL twod;
};

REF_STATUS ref_grid_create( REF_GRID *ref_grid, REF_MPI ref_mpi );
REF_STATUS ref_grid_free( REF_GRID ref_grid );

REF_STATUS ref_grid_deep_copy( REF_GRID *ref_grid, REF_GRID original );

#define ref_grid_mpi(ref_grid) ((ref_grid)->mpi)
#define ref_grid_once(ref_grid) ref_mpi_once(ref_grid_mpi(ref_grid))

#define ref_grid_node(ref_grid) ((ref_grid)->node)
#define ref_grid_cell(ref_grid,group) ((ref_grid)->cell[(group)])

#define ref_grid_tet(ref_grid) ref_grid_cell(ref_grid,0)
#define ref_grid_pyr(ref_grid) ref_grid_cell(ref_grid,1)
#define ref_grid_pri(ref_grid) ref_grid_cell(ref_grid,2)
#define ref_grid_hex(ref_grid) ref_grid_cell(ref_grid,3)

#define ref_grid_edg(ref_grid) ((ref_grid)->edg)
#define ref_grid_tri(ref_grid) ((ref_grid)->tri)
#define ref_grid_qua(ref_grid) ((ref_grid)->qua)

#define ref_grid_geom(ref_grid) ((ref_grid)->geom)
#define ref_grid_gather(ref_grid) ((ref_grid)->gather)
#define ref_grid_adapt(ref_grid,param) (((ref_grid)->adapt)->param)

#define ref_grid_twod(ref_grid) ((ref_grid)->twod)

#define each_ref_grid_ref_cell( ref_grid, group, ref_cell )		\
  for ( (group) = 0, (ref_cell) = ref_grid_cell(ref_grid,group) ;	\
	(group) < 4;							\
	(group)++  , (ref_cell) = ref_grid_cell(ref_grid,group) )

#define ref_grid_guess_twod_status( ref_grd )		\
  if ( 0 == ref_cell_n(ref_grid_tet(ref_grd)) &&	\
       0 == ref_cell_n(ref_grid_pyr(ref_grd)) &&	\
       ( 0 != ref_cell_n(ref_grid_pri(ref_grd)) ||	\
	 0 != ref_cell_n(ref_grid_hex(ref_grd)) ) )	\
    ref_grid_twod(ref_grd) = REF_TRUE;

REF_STATUS ref_grid_inspect( REF_GRID ref_grid );
REF_STATUS ref_grid_tattle( REF_GRID ref_grid, REF_INT node );

REF_STATUS ref_grid_cell_with( REF_GRID ref_grid, REF_INT node_per,
			       REF_CELL *ref_cell );
REF_STATUS ref_grid_face_with( REF_GRID ref_grid, REF_INT node_per,
			       REF_CELL *ref_cell );

REF_STATUS ref_grid_cell_has_face( REF_GRID ref_grid, 
				   REF_INT *face_nodes,
				   REF_BOOL *has_face );

REF_STATUS ref_grid_boundary_nodes( REF_GRID ref_grid, 
				    REF_INT boundary_tag, 
				    REF_INT *nnode, REF_INT *nface, 
				    REF_INT **g2l, REF_INT **l2g );
REF_STATUS ref_grid_edge_nodes( REF_GRID ref_grid, 
				REF_INT edge_tag, 
				REF_INT *nnode, REF_INT *nedge, 
				REF_INT **g2l, REF_INT **l2g );

REF_STATUS ref_grid_outward_boundary_orientation( REF_GRID ref_grid );
REF_STATUS ref_grid_inward_boundary_orientation( REF_GRID ref_grid );

REF_STATUS ref_grid_identity_interp_guess( REF_GRID ref_grid );

REF_STATUS ref_grid_enclosing_tri( REF_GRID ref_grid, REF_DBL *xyz,
				   REF_INT *tri, REF_DBL *bary );
REF_STATUS ref_grid_enclosing_tet( REF_GRID ref_grid, REF_DBL *xyz,
				   REF_INT *tet, REF_DBL *bary );


END_C_DECLORATION

#endif /* REF_GRID_H */
