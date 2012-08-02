#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fasthyperbolic.h"

#include "testmacros.h"

test_scalar (fastsinh, sinhf, -25.0f + 50.0f * drand48 (), 1e-4f, 100000000)
test_scalar (fastersinh, sinhf, -25.0f + 50.0f * drand48 (), 2e-2f, 100000000)

test_vector (vfastsinh, sinhf, -25.0f + 50.0f * drand48 (), 1e-4f, 100000000)
test_vector (vfastersinh, sinhf, -25.0f + 50.0f * drand48 (), 2e-2f, 100000000)

test_scalar (fastcosh, coshf, -25.0f + 50.0f * drand48 (), 1e-4f, 100000000)
test_scalar (fastercosh, coshf, -25.0f + 50.0f * drand48 (), 2e-2f, 100000000)

test_vector (vfastcosh, coshf, -25.0f + 50.0f * drand48 (), 1e-4f, 100000000)
test_vector (vfastercosh, coshf, -25.0f + 50.0f * drand48 (), 2e-2f, 100000000)

test_scalar (fasttanh, tanhf, -25.0f + 50.0f * drand48 (), 1e-4f, 100000000)
test_scalar (fastertanh, tanhf, -25.0f + 50.0f * drand48 (), 2e-2f, 100000000)

test_vector (vfasttanh, tanhf, -25.0f + 50.0f * drand48 (), 1e-4f, 100000000)
test_vector (vfastertanh, tanhf, -25.0f + 50.0f * drand48 (), 2e-2f, 100000000)

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

  test_fastsinh ();
  test_fastersinh ();
  test_fastcosh ();
  test_fastercosh ();
  test_fasttanh ();
  test_fastertanh ();
  test_vfastsinh ();
  test_vfastersinh ();
  test_vfastcosh ();
  test_vfastercosh ();
  test_vfasttanh ();
  test_vfastertanh ();

  time_fastsinh ();
  time_fastersinh ();
  time_fastcosh ();
  time_fastercosh ();
  time_fasttanh ();
  time_fastertanh ();
  time_vfastsinh ();
  time_vfastersinh ();
  time_vfastcosh ();
  time_vfastercosh ();
  time_vfasttanh ();
  time_vfastertanh ();

  return 0;
}
