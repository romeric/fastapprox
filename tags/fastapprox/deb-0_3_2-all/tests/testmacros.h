#define test_scalar(estf, exactf, gen, maxerr, ntime)           \
  static void                                                   \
  test_ ## estf ## _once (double* erracc,                       \
                          float*  max,                          \
                          float*  argmax)                       \
    {                                                           \
      float x = (gen);                                          \
      float exact = exactf (x);                                 \
      float est = estf (x);                                     \
      float err = fabsf (est - exact) /                         \
                  (fabsf (1e-4f) + fabsf (est) + fabsf (exact));\
                                                                \
      if (err > *max) { *max = err; *argmax = x; }              \
      *erracc += err;                                           \
    }                                                           \
                                                                \
  static void                                                   \
  test_ ## estf (void)                                          \
    {                                                           \
      unsigned int i;                                           \
      double err = 0;                                           \
      float argmax = 0;                                         \
      float max = 0;                                            \
                                                                \
      for (i = 0; i < 100000; ++i)                              \
        {                                                       \
          test_ ## estf ## _once (&err, &max, &argmax);         \
        }                                                       \
                                                                \
      err /= i;                                                 \
                                                                \
      fprintf (stderr,                                          \
               "%s average relative error = %g\n",              \
               #estf,                                           \
               err);                                            \
      fprintf (stderr,                                          \
               "%s max relative error (at %g) = %g\n",          \
               #estf,                                           \
               argmax,                                          \
               max);                                            \
      assert (err < maxerr);                                    \
    }                                                           \
                                                                \
    static void                                                 \
    time_ ## estf (void)                                        \
      {                                                         \
        struct timeval start;                                   \
        struct timeval end;                                     \
        unsigned int i;                                         \
        float sum = 0;                                          \
        volatile float xd = 1.0f;                               \
                                                                \
        gettimeofday (&start, NULL);                            \
                                                                \
        for (i = 0; i < ntime; ++i)                             \
          {                                                     \
            sum += estf (xd);                                   \
          }                                                     \
                                                                \
        gettimeofday (&end, NULL);                              \
                                                                \
        fprintf (stderr,                                        \
                 "%g\r%s million calls per second = %g\n",      \
                 sum,                                           \
                 #estf,                                         \
                   ((double) i) /                               \
                    (1e+6 * (double) end.tv_sec                 \
                     - 1e+6 * (double) start.tv_sec             \
                     + (double) end.tv_usec                     \
                     - (double) start.tv_usec));                \
      }

#define test_scalar2(estf, exactf, genx, geny, maxerr, ntime)   \
  static void                                                   \
  test_ ## estf ## _once (double* erracc,                       \
                          float*  max,                          \
                          float*  argmaxx,                      \
                          float*  argmaxy)                      \
    {                                                           \
      float x = (genx); /* ah ... the generation gap ... */     \
      float y = (geny);                                         \
      float exact = exactf (x, y);                              \
      float est = estf (x, y);                                  \
      float err = fabsf (est - exact) /                         \
                  (fabsf (1e-4f) + fabsf (est) + fabsf (exact));\
                                                                \
      if (err > *max) { *max = err; *argmaxx = x; *argmaxy = y; } \
      *erracc += err;                                           \
    }                                                           \
                                                                \
  static void                                                   \
  test_ ## estf (void)                                          \
    {                                                           \
      unsigned int i;                                           \
      double err = 0;                                           \
      float argmaxx = 0;                                        \
      float argmaxy = 0;                                        \
      float max = 0;                                            \
                                                                \
      for (i = 0; i < 100000; ++i)                              \
        {                                                       \
          test_ ## estf ## _once (&err, &max, &argmaxx, &argmaxy); \
        }                                                       \
                                                                \
      err /= i;                                                 \
                                                                \
      fprintf (stderr,                                          \
               "%s average relative error = %g\n",              \
               #estf,                                           \
               err);                                            \
      fprintf (stderr,                                          \
               "%s max relative error (at %g, %g) = %g\n",      \
               #estf,                                           \
               argmaxx,                                         \
               argmaxy,                                         \
               max);                                            \
      assert (err < maxerr);                                    \
    }                                                           \
                                                                \
    static void                                                 \
    time_ ## estf (void)                                        \
      {                                                         \
        struct timeval start;                                   \
        struct timeval end;                                     \
        unsigned int i;                                         \
        float sum = 0;                                          \
        volatile float xd = 1.0f;                               \
        volatile float yd = 1.0f;                               \
                                                                \
        gettimeofday (&start, NULL);                            \
                                                                \
        for (i = 0; i < ntime; ++i)                             \
          {                                                     \
            sum += estf (xd, yd);                               \
          }                                                     \
                                                                \
        gettimeofday (&end, NULL);                              \
                                                                \
        fprintf (stderr,                                        \
                 "%g\r%s million calls per second = %g\n",      \
                 sum,                                           \
                 #estf,                                         \
                   ((double) i) /                               \
                    (1e+6 * (double) end.tv_sec                 \
                     - 1e+6 * (double) start.tv_sec             \
                     + (double) end.tv_usec                     \
                     - (double) start.tv_usec));                \
      }

#ifdef __SSE2__

#define test_vector(estf, exactf, gen, maxerr, ntime)           \
  static void                                                   \
  test_ ## estf ## _once (double* erracc,                       \
                          float*  max,                          \
                          float*  argmax)                       \
    {                                                           \
      v4sf x = v4sfl ((float) (gen));                           \
      v4sf exact = (v4sf) { exactf (v4sf_index (x, 0)),         \
                            exactf (v4sf_index (x, 1)),         \
                            exactf (v4sf_index (x, 2)),         \
                            exactf (v4sf_index (x, 3)) };       \
      v4sf est = estf (x);                                      \
      v4sf err = v4sf_fabs (est - exact) /                      \
                 (v4sfl (1e-4) + v4sf_fabs (est) + v4sf_fabs (exact));  \
                                                                \
      unsigned int k;                                           \
      for (k = 0; k < 4; ++k)                                   \
        {                                                       \
          if (v4sf_index (err, k) > *max)                       \
            {                                                   \
              *max = v4sf_index (err, k);                       \
              *argmax = v4sf_index (x, k);                      \
            }                                                   \
                                                                \
          *erracc += v4sf_index (err, k);                       \
        }                                                       \
    }                                                           \
                                                                \
  static void                                                   \
  test_ ## estf (void)                                          \
    {                                                           \
      unsigned int i;                                           \
      double err = 0;                                           \
      float argmax = 0;                                         \
      float max = 0;                                            \
                                                                \
      for (i = 0; i < 100000; ++i)                              \
        {                                                       \
          test_ ## estf ## _once (&err, &max, &argmax);         \
        }                                                       \
                                                                \
      err /= (4.0f * i);                                        \
                                                                \
      fprintf (stderr,                                          \
               "%s average relative error = %g\n",              \
               #estf,                                           \
               err);                                            \
      fprintf (stderr,                                          \
               "%s max relative error (at %g) = %g\n",          \
               #estf,                                           \
               argmax,                                          \
               max);                                            \
      assert (err < maxerr);                                    \
    }                                                           \
                                                                \
    static void                                                 \
    time_ ## estf (void)                                        \
      {                                                         \
        struct timeval start;                                   \
        struct timeval end;                                     \
        unsigned int i;                                         \
        v4sf sum = v4sfl (0.0f);                                \
        volatile v4sf xd = v4sfl (1.0f);                        \
                                                                \
        gettimeofday (&start, NULL);                            \
                                                                \
        for (i = 0; i < ntime; ++i)                             \
          {                                                     \
            sum += estf (xd);                                   \
          }                                                     \
                                                                \
        gettimeofday (&end, NULL);                              \
                                                                \
        fprintf (stderr,                                        \
                 "%g\r%s million calls per second = %g\n",      \
                 v4sf_index (sum, 0),                           \
                 #estf,                                         \
                   ((double) i) /                               \
                    (1e+6 * (double) end.tv_sec                 \
                     - 1e+6 * (double) start.tv_sec             \
                     + (double) end.tv_usec                     \
                     - (double) start.tv_usec));                \
      }

#define test_vector2(estf, exactf, genx, geny, maxerr, ntime)   \
  static void                                                   \
  test_ ## estf ## _once (double* erracc,                       \
                          float*  max,                          \
                          float*  argmaxx,                      \
                          float*  argmaxy)                      \
    {                                                           \
      v4sf x = v4sfl ((float) (genx));                          \
      v4sf y = v4sfl ((float) (geny));                          \
      v4sf exact = (v4sf) { exactf (v4sf_index (x, 0),          \
                                    v4sf_index (y, 0)),         \
                            exactf (v4sf_index (x, 1),          \
                                    v4sf_index (y, 1)),         \
                            exactf (v4sf_index (x, 2),          \
                                    v4sf_index (y, 2)),         \
                            exactf (v4sf_index (x, 3),          \
                                    v4sf_index (y, 3)) };       \
      v4sf est = estf (x, y);                                   \
      v4sf err = v4sf_fabs (est - exact) /                      \
                 (v4sfl (1e-4) + v4sf_fabs (est) + v4sf_fabs (exact));  \
                                                                \
      unsigned int k;                                           \
      for (k = 0; k < 4; ++k)                                   \
        {                                                       \
          if (v4sf_index (err, k) > *max)                       \
            {                                                   \
              *max = v4sf_index (err, k);                       \
              *argmaxx = v4sf_index (x, k);                     \
              *argmaxy = v4sf_index (y, k);                     \
            }                                                   \
                                                                \
          *erracc += v4sf_index (err, k);                       \
        }                                                       \
    }                                                           \
                                                                \
  static void                                                   \
  test_ ## estf (void)                                          \
    {                                                           \
      unsigned int i;                                           \
      double err = 0;                                           \
      float argmaxx = 0;                                        \
      float argmaxy = 0;                                        \
      float max = 0;                                            \
                                                                \
      for (i = 0; i < 100000; ++i)                              \
        {                                                       \
          test_ ## estf ## _once (&err, &max, &argmaxx, &argmaxy);   \
        }                                                       \
                                                                \
      err /= (4.0f * i);                                        \
                                                                \
      fprintf (stderr,                                          \
               "%s average relative error = %g\n",              \
               #estf,                                           \
               err);                                            \
      fprintf (stderr,                                          \
               "%s max relative error (at %g, %g) = %g\n",      \
               #estf,                                           \
               argmaxx,                                         \
               argmaxy,                                         \
               max);                                            \
      assert (err < maxerr);                                    \
    }                                                           \
                                                                \
    static void                                                 \
    time_ ## estf (void)                                        \
      {                                                         \
        struct timeval start;                                   \
        struct timeval end;                                     \
        unsigned int i;                                         \
        v4sf sum = v4sfl (0.0f);                                \
        volatile v4sf xd = v4sfl (1.0f);                        \
        volatile v4sf yd = v4sfl (1.0f);                        \
                                                                \
        gettimeofday (&start, NULL);                            \
                                                                \
        for (i = 0; i < ntime; ++i)                             \
          {                                                     \
            sum += estf (xd, yd);                               \
          }                                                     \
                                                                \
        gettimeofday (&end, NULL);                              \
                                                                \
        fprintf (stderr,                                        \
                 "%g\r%s million calls per second = %g\n",      \
                 v4sf_index (sum, 0),                           \
                 #estf,                                         \
                   ((double) i) /                               \
                    (1e+6 * (double) end.tv_sec                 \
                     - 1e+6 * (double) start.tv_sec             \
                     + (double) end.tv_usec                     \
                     - (double) start.tv_usec));                \
      }

#else // __SSE2__

#define test_vector(estf, exactf, gen, maxerr, ntime)           \
    static void                                                 \
    test_ ## estf (void)                                        \
      {                                                         \
        fprintf (stderr, "%s test SKIPPED (no SSE support)\n",  \
                 #estf);                                        \
      }                                                         \
                                                                \
    static void                                                 \
    time_ ## estf (void)                                        \
      {                                                         \
      }

#define test_vector2(estf, exactf, genx, geny, maxerr, ntime)   \
    static void                                                 \
    test_ ## estf (void)                                        \
      {                                                         \
        fprintf (stderr, "%s test SKIPPED (no SSE support)\n",  \
                 #estf);                                        \
      }                                                         \
                                                                \
    static void                                                 \
    time_ ## estf (void)                                        \
      {                                                         \
      }

#endif // __SSE2__
