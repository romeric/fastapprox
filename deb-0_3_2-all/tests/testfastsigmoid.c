#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fastsigmoid.h"

#include "testmacros.h"

static inline float
sigmoidf (float x)
{
  return 1.0 / (1.0 + expf (-x));
}

test_scalar (fastsigmoid, sigmoidf, -50.0f + 100.0f * drand48 (), 1e-4f, 100000000)
test_scalar (fastersigmoid, sigmoidf, -50.0f + 100.0f * drand48 (), 2e-2f, 100000000)

test_vector (vfastsigmoid, sigmoidf, -50.0f + 100.0f * drand48 (), 1e-4f, 100000000)
test_vector (vfastersigmoid, sigmoidf, -50.0f + 100.0f * drand48 (), 2e-2f, 100000000)

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

  test_fastsigmoid ();
  test_fastersigmoid ();
  test_vfastsigmoid ();
  test_vfastersigmoid ();

  time_fastsigmoid ();
  time_fastersigmoid ();
  time_vfastsigmoid ();
  time_vfastersigmoid ();

  return 0;
}
