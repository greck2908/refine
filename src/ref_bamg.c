
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

#include "ref_export.h"
#include "ref_grid.h"
#include "ref_histogram.h"
#include "ref_import.h"
#include "ref_part.h"
#include "ref_validation.h"

int main(int argc, char *argv[]) {
  REF_MPI ref_mpi;
  REF_GRID ref_grid;
  REF_BOOL valid_inputs;

  valid_inputs = (3 == argc);

  if (!valid_inputs) {
    printf("usage: %s project.extension project.metric\n", argv[0]);
    return 0;
  }

  RSS(ref_mpi_create(&ref_mpi), "create");

  RSS(ref_import_by_extension(&ref_grid, ref_mpi, argv[1]), "import");
  RSS(ref_export_by_extension(ref_grid, "bamg.msh"), "export");
  RSS(ref_part_metric(ref_grid_node(ref_grid), argv[2]), "part metric");
  RSS(ref_export_metric2d(ref_grid, "bamg.metric2d"), "export");

  RSS(ref_validation_cell_volume(ref_grid), "vol");
  RSS(ref_histogram_quality(ref_grid), "gram");
  RSS(ref_histogram_ratio(ref_grid), "gram");

  RSS(ref_grid_free(ref_grid), "free");

  REIS(0, system("bamg -M bamg.metric2d -b bamg.msh -o bamg-out.msh"),
       "bamg failed");

  RSS(ref_import_by_extension(&ref_grid, ref_mpi, "bamg-out.msh"), "import");
  RSS(ref_export_by_extension(ref_grid, "ref_bamg_test.b8.ugrid"), "export");
  RSS(ref_export_tec_surf(ref_grid, "ref_bamg_test.tec"), "ex");

  RSS(ref_grid_free(ref_grid), "free");
  RSS(ref_mpi_free(ref_mpi), "free");
  return 0;
}
