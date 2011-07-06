#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fasterf.h"

#include "testmacros.h"

static float
slowinverseerf (float x)
{
  float y0 = 0.886227f * x;
  float sqrtpi = 1.7724538509055160f;

  y0 += 0.5f * exp (y0 * y0) * sqrtpi * (x - erff (y0));
  y0 += 0.5f * exp (y0 * y0) * sqrtpi * (x - erff (y0));
  y0 += 0.5f * exp (y0 * y0) * sqrtpi * (x - erff (y0));
  y0 += 0.5f * exp (y0 * y0) * sqrtpi * (x - erff (y0));
  y0 += 0.5f * exp (y0 * y0) * sqrtpi * (x - erff (y0));

  return y0;
}

test_scalar (fasterf, erff, -6.0f + 12.0f * drand48 (), 1e-3f, 100000000)
test_scalar (fastererf, erff, -6.0f + 12.0f * drand48 (), 2e-2f, 100000000)
test_vector (vfasterf, erff, -6.0f + 12.0f * drand48 (), 1e-3f, 100000000)
test_vector (vfastererf, erff, -6.0f + 12.0f * drand48 (), 2e-2f, 100000000)

test_scalar (fastinverseerf, slowinverseerf, -0.99f + 1.98f * drand48 (), 2e-3f, 100000000)
test_vector (vfastinverseerf, slowinverseerf, -0.99f + 1.98f * drand48 (), 2e-3f, 100000000)
test_scalar (fasterinverseerf, slowinverseerf, -0.99f + 1.98f * drand48 (), 4e-2f, 100000000)
test_vector (vfasterinverseerf, slowinverseerf, -0.99f + 1.98f * drand48 (), 4e-2f, 100000000)

test_scalar (fasterfc, erfcf, -2.0f + 4.0f * drand48 (), 5e-3f, 100000000)
test_scalar (fastererfc, erfcf, -2.0f + 4.0f * drand48 (), 8e-2f, 100000000)
test_vector (vfasterfc, erfcf, -2.0f + 4.0f * drand48 (), 5e-3f, 100000000)
test_vector (vfastererfc, erfcf, -2.0f + 4.0f * drand48 (), 8e-2f, 100000000)

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

  test_fasterf ();
  test_fastererf ();
  test_vfasterf ();
  test_vfastererf ();
  test_fasterfc ();
  test_fastererfc ();
  test_vfasterfc ();
  test_vfastererfc ();
  test_fastinverseerf ();
  test_vfastinverseerf ();
  test_fasterinverseerf ();
  test_vfasterinverseerf ();

  time_fasterf ();
  time_fastererf ();
  time_vfasterf ();
  time_vfastererf ();
  time_fasterfc ();
  time_fastererfc ();
  time_vfasterfc ();
  time_vfastererfc ();
  time_fastinverseerf ();
  time_vfastinverseerf ();
  time_fasterinverseerf ();
  time_vfasterinverseerf ();

  return 0;
}
