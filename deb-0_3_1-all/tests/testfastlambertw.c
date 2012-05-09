#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/fastlambertw.h"

#include "testmacros.h"

#include "../src/config.h"
#ifdef HAVE_GSL_GSL_SF_LAMBERT_H 
#include <gsl/gsl_sf_lambert.h>
#endif

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

  for (n = 0; n < 20; ++n)
    {
      w = (w * w + exp (-w) * x) / (1.0 + w);
    }

  return w;
}

static inline float
lambertwexpxf (float x)
{
  return lambertwf (expf (x));
}

test_scalar (fastlambertw, lambertwf, lambertwrange (), 1e-4f, 100000000)
test_scalar (fasterlambertw, lambertwf, lambertwrange (), 1e-2f, 100000000)
test_scalar (fastlambertwexpx, lambertwexpxf, -3.0f + 6.0f * drand48 (), 1e-3f, 100000000)
test_scalar (fasterlambertwexpx, lambertwexpxf, -3.0f + 6.0f * drand48 (), 1e-2f, 100000000)

#ifdef HAVE_GSL_GSL_SF_LAMBERT_H 
test_scalar (gsl_sf_lambert_W0, lambertwf, lambertwrange (), 1e-2f, 1000000)
#endif

test_vector (vfastlambertw, lambertwf, lambertwrange (), 1e-4f, 100000000)
test_vector (vfasterlambertw, lambertwf, lambertwrange (), 1e-2f, 100000000)
test_vector (vfastlambertwexpx, lambertwexpxf, -3.0f + 6.0f * drand48 (), 1e-3f, 100000000)
test_vector (vfasterlambertwexpx, lambertwexpxf, -3.0f + 6.0f * drand48 (), 1e-2f, 100000000)

int 
main (int   argc,
      char *argv[])
{
  char buf[4096];

  (void) argc;

  srand48 (69);

//  fprintf (stderr, "fastlambertw (%g) = %g, "
//                   "fastlambertw (%g) = %g, "
//                   "fasterlambertwexpx (%g) = %g (%g)\n",
//           -0.36787944117144232f,
//           fastlambertw (-0.36787944117144232f),
//           -0.36787944117144232f + 0.01f,
//           fastlambertw (-0.36787944117144232f + 0.01f),
//           -5.0f,
//           fasterlambertwexpx (-5.0f),
//           v4sf_index (vfasterlambertwexpx (v4sfl (-5.0f)), 0));

  strncpy (buf, argv[0], sizeof (buf) - 5);
  strncat (buf, ".out", 5);

  fclose (stderr);
  stderr = fopen (buf, "w");

  test_fastlambertw ();
  test_fasterlambertw ();
  test_vfastlambertw ();
  test_vfasterlambertw ();
#ifdef HAVE_GSL_GSL_SF_LAMBERT_H 
  test_gsl_sf_lambert_W0 ();
#endif
  test_fastlambertwexpx ();
  test_fasterlambertwexpx ();
  test_vfastlambertwexpx ();
  test_vfasterlambertwexpx ();

  time_fastlambertw ();
  time_fasterlambertw ();
  time_vfastlambertw ();
  time_vfasterlambertw ();
#ifdef HAVE_GSL_GSL_SF_LAMBERT_H 
  time_gsl_sf_lambert_W0 ();
#endif
  time_fastlambertwexpx ();
  time_fasterlambertwexpx ();
  time_vfastlambertwexpx ();
  time_vfasterlambertwexpx ();

  return 0;
}
