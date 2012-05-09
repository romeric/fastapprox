#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fastlog.h"

#include "testmacros.h"

test_scalar (fastlog, logf, 1e-10f + 10.0f * drand48 (), 1e-4f, 100000000)
test_scalar (fasterlog, logf, 1e-10f + 10.0f * drand48 (), 2e-2f, 100000000)

test_vector (vfastlog, logf, 1e-10f + 10.0f * drand48 (), 1e-4f, 100000000)
test_vector (vfasterlog, logf, 1e-10f + 10.0f * drand48 (), 2e-2f, 100000000)

int 
main (int   argc,
      char *argv[])
{
  char buf[4096];

  (void) argc;

  srand48 (69);

  strncpy (buf, argv[0], sizeof (buf) - 5);
  strncat (buf, ".out", 5);

  fclose (stderr);
  stderr = fopen (buf, "w");

  test_fastlog ();
  test_fasterlog ();
  test_vfastlog ();
  test_vfasterlog ();

  time_fastlog ();
  time_fasterlog ();
  time_vfastlog ();
  time_vfasterlog ();

  return 0;
}
