
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ref_fixture.h"

#include "ref_export.h"

#include "ref_adj.h"
#include "ref_cell.h"
#include "ref_grid.h"
#include "ref_list.h"
#include "ref_matrix.h"
#include "ref_node.h"

#include "ref_dict.h"
#include "ref_face.h"
#include "ref_mpi.h"
#include "ref_sort.h"

#include "ref_edge.h"
#include "ref_migrate.h"
#include "ref_validation.h"

#include "ref_metric.h"

int main(int argc, char *argv[]) {
  REF_MPI ref_mpi;
  RSS(ref_mpi_start(argc, argv), "start");
  RSS(ref_mpi_create(&ref_mpi), "create");

  if (REF_FALSE) { /* export for viz */
    REF_GRID ref_grid;

    RSS(ref_fixture_twod_brick_grid(&ref_grid, ref_mpi), "fix");

    RSS(ref_validation_cell_node(ref_grid), "invalid pri");

    RSS(ref_export_tec(ref_grid, "twod.tec"), "tec");

    RSS(ref_grid_free(ref_grid), "free");
  }

  {
    REF_GRID ref_grid;

    RSS(ref_fixture_pri_grid(&ref_grid, ref_mpi), "fix");

    RSS(ref_validation_cell_node(ref_grid), "invalid pri");

    RSS(ref_grid_free(ref_grid), "free");
  }

  {
    REF_GRID ref_grid;

    RSS(ref_fixture_tet2_grid(&ref_grid, ref_mpi), "fix");

    RSS(ref_validation_cell_node(ref_grid), "invalid pri");

    RSS(ref_grid_free(ref_grid), "free");
  }

  {
    REF_GRID ref_grid;
    REF_INT nodes[REF_CELL_MAX_SIZE_PER];
    REF_BOOL valid;

    RSS(ref_fixture_pri_grid(&ref_grid, ref_mpi), "fix");

    RSS(ref_cell_nodes(ref_grid_tri(ref_grid), 0, nodes), "tri0");
    RSS(ref_node_tri_twod_orientation(ref_grid_node(ref_grid), nodes, &valid),
        "valid");
    RAS(valid, "expected valid");
    RSS(ref_cell_nodes(ref_grid_tri(ref_grid), 1, nodes), "tri0");
    RSS(ref_node_tri_twod_orientation(ref_grid_node(ref_grid), nodes, &valid),
        "valid");
    RAS(valid, "expected valid");

    RSS(ref_grid_free(ref_grid), "free");
  }

  {
    REF_GRID ref_grid;

    RSS(ref_fixture_pri_stack_grid(&ref_grid, ref_mpi), "fix");

    if (ref_mpi_para(ref_grid_mpi(ref_grid)))
      RSS(ref_export_tec_part(ref_grid, "ref_fixture_orig_stack"), "see");

    RSS(ref_validation_cell_node(ref_grid), "invalid stack");

    RSS(ref_migrate_to_balance(ref_grid), "bal");

    if (ref_mpi_para(ref_grid_mpi(ref_grid)))
      RSS(ref_export_tec_part(ref_grid, "ref_fixture_bal_stack"), "see");

    RSS(ref_grid_free(ref_grid), "free");
  }

  {
    REF_GRID ref_grid;

    RSS(ref_fixture_pri_grid(&ref_grid, ref_mpi), "fix");

    if (ref_mpi_para(ref_grid_mpi(ref_grid)))
      RSS(ref_export_tec_part(ref_grid, "ref_fixture_orig_pri"), "see");

    RSS(ref_validation_cell_node(ref_grid), "invalid pri");

    RSS(ref_migrate_to_balance(ref_grid), "bal");

    if (ref_mpi_para(ref_grid_mpi(ref_grid)))
      RSS(ref_export_tec_part(ref_grid, "ref_fixture_bal_pri"), "see");

    RSS(ref_grid_free(ref_grid), "free");
  }

  if (2 == argc) {
    REF_GRID ref_grid;
    RSS(ref_fixture_tet_brick_grid(&ref_grid, ref_mpi), "fix");
    RSS(ref_export_by_extension(ref_grid, argv[1]), "export");
    RSS(ref_grid_free(ref_grid), "free");
  }

  RSS(ref_mpi_free(ref_mpi), "free");
  RSS(ref_mpi_stop(), "stop");
  return 0;
}
