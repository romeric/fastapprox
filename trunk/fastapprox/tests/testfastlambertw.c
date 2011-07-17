#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fastlambertw.h"

#include "testmacros.h"

static inline float
lambertwrange (void)
{
  if (drand48 () < 0.5) 
    {
      return -0.36787944117144232f + 1.36787944117144232f * drand48 ();
    }
  else
    {
      return 100.0f * drand48 ();
    }
}

static inline float
lambertwf (float x)
{
  float w = (x < 5) ? 0 : log (x) - log (log (x)) + log (log (x)) / log (x);
  unsigned int n;

  for (n = 0; n < 10; ++n)
    {
      w = (w * w + exp (-w) * x) / (1.0 + w);
    }

  return w;
}

test_scalar (fastlambertw, lambertwf, lambertwrange (), 1e-4f, 100000000)
test_scalar (fasterlambertw, lambertwf, lambertwrange (), 1e-2f, 100000000)

test_vector (vfastlambertw, lambertwf, lambertwrange (), 1e-4f, 100000000)
test_vector (vfasterlambertw, lambertwf, lambertwrange (), 1e-2f, 100000000)

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

  test_fastlambertw ();
  test_fasterlambertw ();
  test_vfastlambertw ();
  test_vfasterlambertw ();

  time_fastlambertw ();
  time_fasterlambertw ();
  time_vfastlambertw ();
  time_vfasterlambertw ();

  return 0;
}
