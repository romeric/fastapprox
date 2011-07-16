#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "../src/config.h"

#ifdef __cplusplus
#ifdef HAVE_BOOST_MATH_SPECIAL_FUNCTIONS_DIGAMMA_HPP
#include <boost/math/special_functions/digamma.hpp>
#endif
#endif

#include "../src/fastgamma.h"

#include "testmacros.h"

test_scalar (fastlgamma, lgammaf, 1e-2f + 10.0f * drand48 (), 5e-4f, 100000000)
test_scalar (fasterlgamma, lgammaf, 1e-2f + 10.0f * drand48 (), 1e-1f, 100000000)

test_vector (vfastlgamma, lgammaf, 1e-2f + 10.0f * drand48 (), 5e-4f, 100000000)
test_vector (vfasterlgamma, lgammaf, 1e-2f + 10.0f * drand48 (), 1e-1f, 100000000)

#ifdef __cplusplus
#ifdef HAVE_BOOST_MATH_SPECIAL_FUNCTIONS_DIGAMMA_HPP
test_scalar (fastdigamma, boost::math::digamma, 1e-2f + 10.0f * drand48 (), 5e-4f, 100000000)
test_scalar (fasterdigamma, boost::math::digamma, 1e-2f + 10.0f * drand48 (), 1e-1f, 100000000)

test_vector (vfastdigamma, boost::math::digamma, 1e-2f + 10.0f * drand48 (), 5e-4f, 100000000)
test_vector (vfasterdigamma, boost::math::digamma, 1e-2f + 10.0f * drand48 (), 1e-1f, 100000000)
#endif
#endif

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

  test_fastlgamma ();
  test_fasterlgamma ();
  test_vfastlgamma ();
  test_vfasterlgamma ();

#ifdef __cplusplus
#ifdef HAVE_BOOST_MATH_SPECIAL_FUNCTIONS_DIGAMMA_HPP
  test_fastdigamma ();
  test_fasterdigamma ();
  test_vfastdigamma ();
  test_vfasterdigamma ();
#endif
#endif

  time_fastlgamma ();
  time_fasterlgamma ();
  time_vfastlgamma ();
  time_vfasterlgamma ();

#ifdef __cplusplus
#ifdef HAVE_BOOST_MATH_SPECIAL_FUNCTIONS_DIGAMMA_HPP
  time_fastdigamma ();
  time_fasterdigamma ();
  time_vfastdigamma ();
  time_vfasterdigamma ();
#endif
#endif

#ifdef HAVE_BOOST_MATH_SPECIAL_FUNCTIONS_DIGAMMA_HPP
  return 0;
#else
  return 77;
#endif
}
