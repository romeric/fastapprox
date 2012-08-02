#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fastpow.h"

#include "testmacros.h"

test_scalar2 (fastpow, powf, 3.0f * drand48 (), -3.0f + 6.0f * drand48 (), 1e-4f, 100000000)
test_scalar2 (fasterpow, powf, 3.0f * drand48 (), -3.0f + 6.0f * drand48 (), 2e-2f, 100000000)

test_vector2 (vfastpow, powf, 3.0f * drand48 (), -3.0f + 6.0f * drand48 (), 1e-4f, 100000000)
test_vector2 (vfasterpow, powf, 3.0f * drand48 (), -3.0f + 6.0f * drand48 (), 2e-2f, 100000000)

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

  test_fastpow ();
  test_fasterpow ();
  test_vfastpow ();
  test_vfasterpow ();

  time_fastpow ();
  time_fasterpow ();
  time_vfastpow ();
  time_vfasterpow ();

  return 0;
}
