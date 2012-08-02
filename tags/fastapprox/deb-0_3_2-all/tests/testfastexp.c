#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fastexp.h"

#include "testmacros.h"

test_scalar (fastexp, expf, -5.0f + 10.0f * drand48 (), 1e-4f, 100000000)
test_scalar (fasterexp, expf, -5.0f + 10.0f * drand48 (), 2e-2f, 100000000)

test_vector (vfastexp, expf, -5.0f + 10.0f * drand48 (), 1e-4f, 100000000)
test_vector (vfasterexp, expf, -5.0f + 10.0f * drand48 (), 2e-2f, 100000000)

int 
main (int   argc,
      char *argv[])
{
  char buf[4096];

  (void) argc;

  float x;
  for (x = -50; x > -1000; x -= 100)
    {
      assert (fastexp (x) >= 0);
      assert (fasterexp (x) >= 0);
#ifdef __SSE2__
      v4sf vx = v4sfl (x);
      assert (v4sf_index (vfastexp (vx), 0) >= 0);
      assert (v4sf_index (vfasterexp (vx), 0) >= 0);
#endif
    }

  srand48 (69);

  strncpy (buf, argv[0], sizeof (buf) - 5);
  strncat (buf, ".out", 5);

  fclose (stderr);
  stderr = fopen (buf, "w");


  test_fastexp ();
  test_fasterexp ();
  test_vfastexp ();
  test_vfasterexp ();

  time_fastexp ();
  time_fasterexp ();
  time_vfastexp ();
  time_vfasterexp ();

  return 0;
}
