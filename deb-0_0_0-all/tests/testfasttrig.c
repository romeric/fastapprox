#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fasttrig.h"

#include "testmacros.h"

static const float pi = 3.1415926535897932f;
static const float twopi = 6.2831853071795865f;
static const float halfpi = 1.5707963267948966f;

static inline float
no_half_pi ()
{
  float rv = -100.0 + 200.0 * drand48 ();
  int k = rv / halfpi;
  float delta = rv - k * halfpi;

  while (fabsf (delta) < 0.001f)
    {
      rv = -100.0 + 200.0 * drand48 ();
      k = rv / halfpi;
      delta = rv - k * halfpi;
    }

  return rv;
}

test_scalar (fastsin, sinf, -pi + twopi * drand48 (), 1e-4f, 100000000)
test_scalar (fastersin, sinf, -pi + twopi * drand48 (), 2e-2f, 100000000)
test_scalar (fastsinfull, sinf, -100.0 + 200.0 * drand48 (), 1e-4f, 100000000)
test_scalar (fastersinfull, sinf, -100.0 + 200.0 * drand48 (), 2e-2f, 100000000)

test_vector (vfastsin, sinf, -pi + twopi * drand48 (), 1e-4f, 100000000)
test_vector (vfastersin, sinf, -pi + twopi * drand48 (), 2e-2f, 100000000)
test_vector (vfastsinfull, sinf, -100.0 + 200.0 * drand48 (), 1e-4f, 100000000)
test_vector (vfastersinfull, sinf, -100.0 + 200.0 * drand48 (), 2e-2f, 100000000)

test_scalar (fastcos, cosf, -pi + twopi * drand48 (), 1e-4f, 100000000)
test_scalar (fastercos, cosf, -pi + twopi * drand48 (), 2e-2f, 100000000)
test_scalar (fastcosfull, cosf, -100.0 + 200.0 * drand48 (), 1e-4f, 100000000)
test_scalar (fastercosfull, cosf, -100.0 + 200.0 * drand48 (), 2e-2f, 100000000)

test_vector (vfastcos, cosf, -pi + twopi * drand48 (), 1e-4f, 100000000)
test_vector (vfastercos, cosf, -pi + twopi * drand48 (), 2e-2f, 100000000)
test_vector (vfastcosfull, cosf, -100.0 + 200.0 * drand48 (), 1e-4f, 100000000)
test_vector (vfastercosfull, cosf, -100.0 + 200.0 * drand48 (), 2e-2f, 100000000)

test_scalar (fasttan, tanf, -0.5f * pi + pi * drand48 (), 1e-4f, 100000000)
test_scalar (fastertan, tanf, -0.5f * pi + pi * drand48 (), 2e-2f, 100000000)
test_scalar (fasttanfull, tanf, no_half_pi (), 1e-4f, 100000000)
test_scalar (fastertanfull, tanf, no_half_pi (), 2e-2f, 100000000)

test_vector (vfasttan, tanf, -0.5f * pi + pi * drand48 (), 1e-4f, 100000000)
test_vector (vfastertan, tanf, -0.5f * pi + pi * drand48 (), 2e-2f, 100000000)
test_vector (vfasttanfull, tanf, no_half_pi (), 1e-4f, 100000000)
test_vector (vfastertanfull, tanf, no_half_pi (), 2e-2f, 100000000)

int 
main (int   argc,
      char *argv[])
{
  (void) argc;
  (void) argv;

  char buf[4096];

  srand48 (69);

  strncpy (buf, argv[0], sizeof (buf) - 5);
  strncat (buf, ".out", 5);

  fclose (stderr);
  stderr = fopen (buf, "w");

  test_fastsin ();
  test_fastersin ();
  test_fastsinfull ();
  test_fastersinfull ();
  test_fastcos ();
  test_fastercos ();
  test_fastcosfull ();
  test_fastercosfull ();
  test_fasttan ();
  test_fastertan ();
  test_fasttanfull ();
  test_fastertanfull ();
  test_vfastsin ();
  test_vfastersin ();
  test_vfastsinfull ();
  test_vfastersinfull ();
  test_vfastcos ();
  test_vfastercos ();
  test_vfastcosfull ();
  test_vfastercosfull ();
  test_vfasttan ();
  test_vfastertan ();
  test_vfasttanfull ();
  test_vfastertanfull ();

  time_fastsin ();
  time_fastersin ();
  time_fastsinfull ();
  time_fastersinfull ();
  time_fastcos ();
  time_fastercos ();
  time_fastcosfull ();
  time_fastercosfull ();
  time_fasttan ();
  time_fastertan ();
  time_fasttanfull ();
  time_fastertanfull ();
  time_vfastsin ();
  time_vfastersin ();
  time_vfastsinfull ();
  time_vfastersinfull ();
  time_vfastcos ();
  time_vfastercos ();
  time_vfastcosfull ();
  time_vfastercosfull ();
  time_vfasttan ();
  time_vfastertan ();
  time_vfasttanfull ();
  time_vfastertanfull ();

  return 0;
}
