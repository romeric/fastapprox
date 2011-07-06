/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __SSE_H_
#define __SSE_H_

#ifdef __SSE2__

#include <emmintrin.h>

#ifdef __cplusplus
namespace {
#endif // __cplusplus

typedef int myv4si __attribute__ ((__vector_size__ (16)));
typedef __m128 v4sf;
typedef __m128i v4si;

#define v4si_to_v4sf _mm_cvtepi32_ps
#define v4sf_to_v4si _mm_cvttps_epi32

#define v4sfl(x) ((const v4sf) { (x), (x), (x), (x) })
#define v4sil(x) ((const v4si) (const myv4si) { (x), (x), (x), (x) })

typedef union { v4sf f; float array[4]; } v4sfindexer;
#define v4sf_index(_findx, _findi)      \
  ({                                    \
     v4sfindexer _findvx = { _findx } ; \
     _findvx.array[_findi];             \
  })
typedef union { v4si i; int array[4]; } v4siindexer;
#define v4si_index(_iindx, _iindi)      \
  ({                                    \
     v4siindexer _iindvx = { _iindx } ; \
     _iindvx.array[_iindi];             \
  })

typedef union { v4sf f; v4si i; } v4sfv4sipun;
#define v4sf_fabs(x)                    \
  ({                                    \
     v4sfv4sipun vx = { x };            \
     vx.i &= v4sil (0x7FFFFFFF);        \
     vx.f;                              \
  })

#ifdef __cplusplus
} // end namespace
#endif // __cplusplus

#endif // __SSE2__

#endif // __SSE_H_
/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __FAST_EXP_H_
#define __FAST_EXP_H_

#include <stdint.h>

static inline float
fastpow2 (float p)
{
  float offset = (p < 0) ? 1.0f : 0.0f;
  int w = p;
  float z = p - w + offset;
  union { uint32_t i; float f; } v = { (1 << 23) * (p + 121.2740838f + 27.7280233f / (4.84252568f - z) - 1.49012907f * z) };

  return v.f;
}

static inline float
fastexp (float p)
{
  return fastpow2 (1.442695040f * p);
}

static inline float
fasterpow2 (float p)
{
  union { uint32_t i; float f; } v = { (1 << 23) * (p + 126.94269504f) };
  return v.f;
}

static inline float
fasterexp (float p)
{
  return fasterpow2 (1.442695040f * p);
}

#ifdef __SSE2__

static inline v4sf
vfastpow2 (const v4sf p)
{
  v4si w = v4sf_to_v4si (p);
  v4sf ltzero = _mm_cmplt_ps (p, v4sfl (0.0f));
  v4sf offset = _mm_and_ps (ltzero, v4sfl (1.0f));
  v4sf z = p - v4si_to_v4sf (w) + offset;

  const v4sf c_121_2740838 = v4sfl (121.2740838f);
  const v4sf c_27_7280233 = v4sfl (27.7280233f);
  const v4sf c_4_84252568 = v4sfl (4.84252568f);
  const v4sf c_1_49012907 = v4sfl (1.49012907f);
  union { v4si i; v4sf f; } v = {
    v4sf_to_v4si (
      v4sfl (1 << 23) * 
      (p + c_121_2740838 + c_27_7280233 / (c_4_84252568 - z) - c_1_49012907 * z)
    )
  };

  return v.f;
}

static inline v4sf
vfastexp (const v4sf p)
{
  const v4sf c_invlog_2 = v4sfl (1.442695040f);

  return vfastpow2 (c_invlog_2 * p);
}

static inline v4sf
vfasterpow2 (const v4sf p)
{
  const v4sf c_126_94269504 = v4sfl (126.94269504f);
  union { v4si i; v4sf f; } v = { v4sf_to_v4si (v4sfl (1 << 23) * (p + c_126_94269504)) };
  return v.f;
}

static inline v4sf
vfasterexp (const v4sf p)
{
  const v4sf c_invlog_2 = v4sfl (1.442695040f);

  return vfasterpow2 (c_invlog_2 * p);
}

#endif //__SSE2__

#endif // __FAST_EXP_H_
/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __FAST_LOG_H_
#define __FAST_LOG_H_

#include <stdint.h>

static inline float 
fastlog2 (float x)
{
  union { float f; uint32_t i; } vx = { x };
  union { uint32_t i; float f; } mx = { (vx.i & 0x007FFFFF) | (0x7e << 23) };
  float y = vx.i;
  y *= 1.0f / (1 << 23);

  return y - 124.22544637f 
           - 1.498030302f * mx.f 
           - 1.72587999f / (0.3520887068f + mx.f);
}

static inline float
fastlog (float x)
{
  return 0.69314718f * fastlog2 (x);
}

inline float 
fasterlog2 (float x)
{
  union { float f; uint32_t i; } vx = { x };
  float y = vx.i;
  y *= 1.0 / (1 << 23);
  return y - 126.94269504f;
}

inline float
fasterlog (float x)
{
  return 0.69314718f * fasterlog2 (x);
}

#ifdef __SSE2__

static inline v4sf
vfastlog2 (v4sf x)
{
  union { v4sf f; v4si i; } vx = { x };
  union { v4si i; v4sf f; } mx = { (vx.i & v4sil (0x007FFFFF)) | v4sil (0x7e << 23) };
  v4sf y = v4si_to_v4sf (vx.i);
  y *= v4sfl (1.0f / (1 << 23));

  const v4sf c_124_22544637 = v4sfl (124.22544637f);
  const v4sf c_1_498030302 = v4sfl (1.498030302f);
  const v4sf c_1_725877999 = v4sfl (1.72587999f);
  const v4sf c_0_3520087068 = v4sfl (0.3520887068f);

  return y - c_124_22544637 
           - c_1_498030302 * mx.f 
           - c_1_725877999 / (c_0_3520087068 + mx.f);
}

static inline v4sf
vfastlog (v4sf x)
{
  const v4sf c_0_69314718 = v4sfl (0.69314718f);

  return c_0_69314718 * vfastlog2 (x);
}

static inline v4sf 
vfasterlog2 (v4sf x)
{
  union { v4sf f; v4si i; } vx = { x };
  v4sf y = v4si_to_v4sf (vx.i);
  y *= v4sfl (1.0f / (1 << 23));

  const v4sf c_126_94269504 = v4sfl (126.94269504f);

  return y - c_126_94269504;
}

static inline v4sf
vfasterlog (v4sf x)
{
  const v4sf c_0_69314718 = v4sfl (0.69314718f);

  return c_0_69314718 * vfasterlog2 (x);
}

#endif // __SSE2__

#endif // __FAST_LOG_H_
/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __FAST_ERF_H_
#define __FAST_ERF_H_

#include <math.h>
#include <stdint.h>

// fasterfc: not actually faster than erfcf(3) on newer machines!
// ... although vectorized version is interesting
//     and fastererfc is very fast

static inline float
fasterfc (float x)
{
  static const float k = 3.3509633149424609f;
  static const float a = 0.0846486355377475f;
  static const float b = 1.6732800045663587f;
  static const float c = 5.604430819154318f;

  float bx = b * x;
  union { float f; uint32_t i; } vc = { c * x };
  float bxsq = bx * bx;
  float bxquad = bxsq * bxsq;
  float bxfive = bxquad * bx;

  vc.i |= 0x80000000;

  return 2.0f / (1.0f + fastpow2 (k * x)) - a * (bxfive - x) * fasterpow2 (vc.f);
}

static inline float
fastererfc (float x)
{
  static const float k = 3.3509633149424609f;

  return 2.0f / (1.0f + fasterpow2 (k * x));
}

// fasterf: not actually faster than erff(3) on newer machines! 
// ... although vectorized version is interesting
//     and fastererf is very fast

static inline float
fasterf (float x)
{
  return 1.0f - fasterfc (x);
}

static inline float
fastererf (float x)
{
  return 1.0f - fastererfc (x);
}

static inline float
fasterinverseerf (float x)
{
  static const float invk = 0.30004578719350504;

  return invk * fasterlog2 ((1.0f + x) / (1.0f - x));
}

#ifdef __SSE2__

static inline v4sf
vfasterfc (v4sf x)
{
  static const v4sf k = v4sfl (3.3509633149424609f);
  static const v4sf a = v4sfl (0.0846486355377475f);
  static const v4sf b = v4sfl (1.6732800045663587f);
  static const v4sf c = v4sfl (5.604430819154318f);

  union { v4sf f; v4si i; } vc = { c * x };
  vc.i |= v4sil (0x80000000);

  v4sf bx = b * x;
  v4sf bxsq = bx * bx;
  v4sf bxquad = bxsq * bxsq;
  v4sf bxfive = bxquad * bx;

  return v4sfl (2.0f) / (v4sfl (1.0f) + vfastpow2 (k * x)) - a * (bxfive - x) * vfasterpow2 (vc.f);
}

static inline v4sf
vfastererfc (const v4sf x)
{
  static const v4sf k = v4sfl (3.3509633149424609f);

  return v4sfl (2.0f) / (v4sfl (1.0f) + vfasterpow2 (k * x));
}

static inline v4sf
vfasterf (v4sf x)
{
  return v4sfl (1.0f) - vfasterfc (x);
}

static inline v4sf
vfastererf (const v4sf x)
{
  return v4sfl (1.0f) - vfastererfc (x);
}

static inline v4sf
vfasterinverseerf (v4sf x)
{
  static const v4sf invk = v4sfl (0.30004578719350504);

  return invk * vfasterlog2 ((v4sfl (1.0f) + x) / (v4sfl (1.0f) - x));
}

#endif //__SSE2__

#endif // __FAST_ERF_H_
/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __FAST_GAMMA_H_
#define __FAST_GAMMA_H_

#include <stdint.h>

/* gamma/digamma functions only work for positive inputs */

static inline float
fastlgamma (float x)
{
  float logterm = fastlog (x * (1.0f + x) * (2.0f + x));
  float xp3 = 3.0f + x;

  return - 2.081061466f 
         - x 
         + 0.0833333f / xp3 
         - logterm 
         + (2.5f + x) * fastlog (xp3);
}

static inline float
fasterlgamma (float x)
{
  return - 0.0810614667f 
         - x
         - fasterlog (x)
         + (0.5f + x) * fasterlog (1.0f + x);
}

static inline float
fastdigamma (float x)
{
  float twopx = 2.0f + x;
  float logterm = fastlog (twopx);

  return - (1.0f + 2.0f * x) / (x * (1.0f + x)) 
         - (13.0f + 6.0f * x) / (12.0f * twopx * twopx) 
         + logterm;
}

static inline float
fasterdigamma (float x)
{
  float onepx = 1.0f + x;

  return -1.0f / x - 1.0f / (2 * onepx) + fasterlog (onepx);
}

#ifdef __SSE2__

static inline v4sf
vfastlgamma (v4sf x)
{
  const v4sf c_1_0 = v4sfl (1.0f);
  const v4sf c_2_0 = v4sfl (2.0f);
  const v4sf c_3_0 = v4sfl (3.0f);
  const v4sf c_2_081061466 = v4sfl (2.081061466f);
  const v4sf c_0_0833333 = v4sfl (0.0833333f);
  const v4sf c_2_5 = v4sfl (2.5f);

  v4sf logterm = vfastlog (x * (c_1_0 + x) * (c_2_0 + x));
  v4sf xp3 = c_3_0 + x;

  return - c_2_081061466
         - x 
         + c_0_0833333 / xp3 
         - logterm 
         + (c_2_5 + x) * vfastlog (xp3);
}

static inline v4sf
vfasterlgamma (v4sf x)
{
  const v4sf c_0_0810614667 = v4sfl (0.0810614667f);
  const v4sf c_0_5 = v4sfl (0.5f);
  const v4sf c_1 = v4sfl (1.0f);

  return - c_0_0810614667
         - x
         - vfasterlog (x)
         + (c_0_5 + x) * vfasterlog (c_1 + x);
}

static inline v4sf
vfastdigamma (v4sf x)
{
  const v4sf c_1_0 = v4sfl (1.0f);
  const v4sf c_2_0 = v4sfl (2.0f);
  const v4sf c_6_0 = v4sfl (6.0f);
  const v4sf c_12_0 = v4sfl (12.0f);
  const v4sf c_13_0 = v4sfl (13.0f);
  v4sf twopx = c_2_0 + x;
  v4sf logterm = vfastlog (twopx);

  return - (c_1_0 + c_2_0 * x) / (x * (c_1_0 + x))
         - (c_13_0 + c_6_0 * x) / (c_12_0 * twopx * twopx)
         + logterm;
}

static inline v4sf
vfasterdigamma (v4sf x)
{
  const v4sf c_1_0 = v4sfl (1.0f);
  const v4sf c_2_0 = v4sfl (2.0f);
  v4sf onepx = c_1_0 + x;

  return -c_1_0 / x - c_1_0 / (c_2_0 * onepx) + vfasterlog (onepx);
}

#endif //__SSE2__

#endif // __FAST_GAMMA_H_
/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __FAST_HYPERBOLIC_H_
#define __FAST_HYPERBOLIC_H_

#include <stdint.h>

static inline float
fastsinh (float p)
{
  return 0.5f * (fastexp (p) - fastexp (-p));
}

static inline float
fastersinh (float p)
{
  return 0.5f * (fasterexp (p) - fasterexp (-p));
}

static inline float
fastcosh (float p)
{
  return 0.5f * (fastexp (p) + fastexp (-p));
}

static inline float
fastercosh (float p)
{
  return 0.5f * (fasterexp (p) + fasterexp (-p));
}

static inline float
fasttanh (float p)
{
  return -1.0f + 2.0f / (1.0f + fastexp (-2.0f * p));
}

static inline float
fastertanh (float p)
{
  return -1.0f + 2.0f / (1.0f + fasterexp (-2.0f * p));
}

#ifdef __SSE2__

static inline v4sf
vfastsinh (const v4sf p)
{
  const v4sf c_0_5 = v4sfl (0.5f);

  return c_0_5 * (vfastexp (p) - vfastexp (-p));
}

static inline v4sf
vfastersinh (const v4sf p)
{
  const v4sf c_0_5 = v4sfl (0.5f);

  return c_0_5 * (vfasterexp (p) - vfasterexp (-p));
}

static inline v4sf
vfastcosh (const v4sf p)
{
  const v4sf c_0_5 = v4sfl (0.5f);

  return c_0_5 * (vfastexp (p) + vfastexp (-p));
}

static inline v4sf
vfastercosh (const v4sf p)
{
  const v4sf c_0_5 = v4sfl (0.5f);

  return c_0_5 * (vfasterexp (p) + vfasterexp (-p));
}

static inline v4sf
vfasttanh (const v4sf p)
{
  const v4sf c_1 = v4sfl (1.0f);
  const v4sf c_2 = v4sfl (2.0f);

  return -c_1 + c_2 / (c_1 + vfastexp (-c_2 * p));
}

static inline v4sf
vfastertanh (const v4sf p)
{
  const v4sf c_1 = v4sfl (1.0f);
  const v4sf c_2 = v4sfl (2.0f);

  return -c_1 + c_2 / (c_1 + vfasterexp (-c_2 * p));
}

#endif //__SSE2__

#endif // __FAST_HYPERBOLIC_H_
/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __FAST_SIGMOID_H_
#define __FAST_SIGMOID_H_

#include <stdint.h>

static inline float
fastsigmoid (float x)
{
  return 1.0f / (1.0f + fastexp (-x));
}

static inline float
fastersigmoid (float x)
{
  return 1.0f / (1.0f + fasterexp (-x));
}

#ifdef __SSE2__

static inline v4sf
vfastsigmoid (const v4sf x)
{
  const v4sf c_1 = v4sfl (1.0f);

  return c_1 / (c_1 + vfastexp (-x));
}

static inline v4sf
vfastersigmoid (const v4sf x)
{
  const v4sf c_1 = v4sfl (1.0f);

  return c_1 / (c_1 + vfasterexp (-x));
}

#endif //__SSE2__

#endif // __FAST_SIGMOID_H_
/*=====================================================================*
 *                   Copyright (C) 2011 Paul Mineiro                   *
 * All rights reserved.                                                *
 *                                                                     *
 * Redistribution and use in source and binary forms, with             *
 * or without modification, are permitted provided that the            *
 * following conditions are met:                                       *
 *                                                                     *
 *     * Redistributions of source code must retain the                *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer.                                       *
 *                                                                     *
 *     * Redistributions in binary form must reproduce the             *
 *     above copyright notice, this list of conditions and             *
 *     the following disclaimer in the documentation and/or            *
 *     other materials provided with the distribution.                 *
 *                                                                     *
 *     * Neither the name of Paul Mineiro nor the names                *
 *     of other contributors may be used to endorse or promote         *
 *     products derived from this software without specific            *
 *     prior written permission.                                       *
 *                                                                     *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND              *
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,         *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES               *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE             *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER               *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,                 *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES            *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE           *
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                *
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF          *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY              *
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE             *
 * POSSIBILITY OF SUCH DAMAGE.                                         *
 *                                                                     *
 * Contact: Paul Mineiro <paul@mineiro.com>                            *
 *=====================================================================*/

#ifndef __FAST_TRIG_H_
#define __FAST_TRIG_H_

#include <stdint.h>

// http://www.devmaster.net/forums/showthread.php?t=5784
// fast sine variants are for x \in [ -\pi, pi ]
// fast cosine variants are for x \in [ -\pi, pi ]
// fast tangent variants are for x \in [ -\pi / 2, pi / 2 ]
// "full" versions of functions handle the entire range of inputs
// although the range reduction technique used here will be hopelessly
// inaccurate for |x| >> 1000
//
// WARNING: fastsinfull, fastcosfull, and fasttanfull can be slower than
// libc calls on older machines (!) and on newer machines are only 
// slighly faster.  however:
//   * vectorized versions are competitive
//   * faster full versions are competitive

static inline float
fastsin (float x)
{
  static const float fouroverpi = 1.2732395447351627f;
  static const float fouroverpisq = 0.40528473456935109f;
  static const float q = 0.78444488374548933f;
  union { float f; uint32_t i; } p = { 0.20363937680730309f };
  union { float f; uint32_t i; } r = { 0.015124940802184233f };
  union { float f; uint32_t i; } s = { -0.0032225901625579573f };

  union { float f; uint32_t i; } vx = { x };
  uint32_t sign = vx.i & 0x80000000;
  vx.i = vx.i & 0x7FFFFFFF;

  float qpprox = fouroverpi * x - fouroverpisq * x * vx.f;
  float qpproxsq = qpprox * qpprox;
  float qpproxquad = qpproxsq * qpproxsq;
  float qpproxhex = qpproxquad * qpproxsq;

  p.i |= sign;
  r.i |= sign;
  s.i ^= sign;

  return q * qpprox + p.f * qpproxsq + r.f * qpproxquad + s.f * qpproxhex;
}

static inline float
fastersin (float x)
{
  static const float fouroverpi = 1.2732395447351627f;
  static const float fouroverpisq = 0.40528473456935109f;
  static const float q = 0.77633023248007499f;
  union { float f; uint32_t i; } p = { 0.22308510060189463f };

  union { float f; uint32_t i; } vx = { x };
  uint32_t sign = vx.i & 0x80000000;
  vx.i &= 0x7FFFFFFF;

  float qpprox = fouroverpi * x - fouroverpisq * x * vx.f;
  float qpproxsq = qpprox * qpprox;

  p.i |= sign;

  return q * qpprox + p.f * qpproxsq;
}

static inline float
fastsinfull (float x)
{
  static const float twopi = 6.2831853071795865f;
  static const float invtwopi = 0.15915494309189534f;

  int k = x * invtwopi;
  float half = -0.5f + ((x < 0) ? 0.0f : 1.0f);
  return fastsin ((half + k) * twopi - x);
}

static inline float
fastersinfull (float x)
{
  static const float twopi = 6.2831853071795865f;
  static const float invtwopi = 0.15915494309189534f;

  int k = x * invtwopi;
  float half = -0.5f + ((x < 0) ? 0.0f : 1.0f);
  return fastersin ((half + k) * twopi - x);
}

static inline float
fastcos (float x)
{
  static const float twopi = 6.2831853071795865f;
  static const float halfpi = 1.5707963267948966f;
  float offset = halfpi - ((x > halfpi) ? twopi : 0.0f);
  return fastsin (x + offset);
}

static inline float
fastercos (float x)
{
  static const float twooverpi = 0.63661977236758134f;
  static const float p = 0.54641335845679634f;

  union { float f; uint32_t i; } vx = { x };
  vx.i &= 0x7FFFFFFF;

  float qpprox = 1.0f - twooverpi * vx.f;

  return qpprox + p * qpprox * (1.0f - qpprox * qpprox);
}

static inline float
fastcosfull (float x)
{
  static const float halfpi = 1.5707963267948966f;
  return fastsinfull (x + halfpi);
}

static inline float
fastercosfull (float x)
{
  static const float halfpi = 1.5707963267948966f;
  return fastersinfull (x + halfpi);
}

static inline float
fasttan (float x)
{
  static const float halfpi = 1.5707963267948966f;
  return fastsin (x) / fastsin (x + halfpi);
}

static inline float
fastertan (float x)
{
  return fastersin (x) / fastercos (x);
}

static inline float
fasttanfull (float x)
{
  static const float twopi = 6.2831853071795865f;
  static const float invtwopi = 0.15915494309189534f;

  int k = x * invtwopi;
  float half = -0.5f + ((x < 0) ? 0.0f : 1.0f);
  float xnew = x - (half + k) * twopi;

  return fastsin (xnew) / fastcos (xnew);
}

static inline float
fastertanfull (float x)
{
  static const float twopi = 6.2831853071795865f;
  static const float invtwopi = 0.15915494309189534f;

  int k = x * invtwopi;
  float half = -0.5f + ((x < 0) ? 0.0f : 1.0f);
  float xnew = x - (half + k) * twopi;

  return fastersin (xnew) / fastercos (xnew);
}

#ifdef __SSE2__

static inline v4sf
vfastsin (const v4sf x)
{
  static const v4sf fouroverpi = v4sfl (1.2732395447351627f);
  static const v4sf fouroverpisq = v4sfl (0.40528473456935109f);
  static const v4sf q = v4sfl (0.78444488374548933f);
  static const v4sf p = v4sfl (0.20363937680730309f);
  static const v4sf r = v4sfl (0.015124940802184233f);
  static const v4sf s = v4sfl (-0.0032225901625579573f);

  union { v4sf f; v4si i; } vx = { x };
  v4si sign = vx.i & v4sil (0x80000000);
  vx.i &= v4sil (0x7FFFFFFF);

  v4sf qpprox = fouroverpi * x - fouroverpisq * x * vx.f;
  v4sf qpproxsq = qpprox * qpprox;
  v4sf qpproxquad = qpproxsq * qpproxsq;
  v4sf qpproxhex = qpproxquad * qpproxsq;

  union { v4sf f; v4si i; } vy = { p * qpproxsq + r * qpproxquad + s * qpproxhex };
  vy.i ^= sign;

  return q * qpprox + vy.f;
}

static inline v4sf
vfastersin (const v4sf x)
{
  static const v4sf fouroverpi = v4sfl (1.2732395447351627f);
  static const v4sf fouroverpisq = v4sfl (0.40528473456935109f);
  static const v4sf q = v4sfl (0.77633023248007499f);
  static const v4sf plit = v4sfl (0.22308510060189463f);
  union { v4sf f; v4si i; } p = { plit };

  union { v4sf f; v4si i; } vx = { x };
  v4si sign = vx.i & v4sil (0x80000000);
  vx.i &= v4sil (0x7FFFFFFF);

  v4sf qpprox = fouroverpi * x - fouroverpisq * x * vx.f;
  v4sf qpproxsq = qpprox * qpprox;

  p.i |= sign;

  return q * qpprox + p.f * qpproxsq;
}

static inline v4sf
vfastsinfull (const v4sf x)
{
  static const v4sf twopi = v4sfl (6.2831853071795865f);
  static const v4sf invtwopi = v4sfl (0.15915494309189534f);

  v4si k = v4sf_to_v4si (x * invtwopi);

  v4sf half = v4sfl (-0.5f);
  v4sf ltzero = _mm_cmpnlt_ps (x, v4sfl (0.0f));
  half += _mm_and_ps (ltzero, v4sfl (1.0f));

  return vfastsin ((half + v4si_to_v4sf (k)) * twopi - x);
}

static inline v4sf
vfastersinfull (const v4sf x)
{
  static const v4sf twopi = v4sfl (6.2831853071795865f);
  static const v4sf invtwopi = v4sfl (0.15915494309189534f);

  v4si k = v4sf_to_v4si (x * invtwopi);

  v4sf half = v4sfl (-0.5f);
  v4sf ltzero = _mm_cmpnlt_ps (x, v4sfl (0.0f));
  half += _mm_and_ps (ltzero, v4sfl (1.0f));

  return vfastersin ((half + v4si_to_v4sf (k)) * twopi - x);
}

static inline v4sf
vfastcos (const v4sf x)
{
  static const v4sf twopi = v4sfl (6.2831853071795865f);
  static const v4sf halfpi = v4sfl (1.5707963267948966f);
  v4sf gthalfpi = _mm_cmpnlt_ps (x, halfpi);
  v4sf offset = halfpi - _mm_and_ps (gthalfpi, twopi);
  return vfastsin (x + offset);
}

static inline v4sf
vfastercos (v4sf x)
{
  static const v4sf twooverpi = v4sfl (0.63661977236758134f);
  static const v4sf p = v4sfl (0.54641335845679634);

  v4sf vx = v4sf_fabs (x);
  v4sf qpprox = v4sfl (1.0f) - twooverpi * vx;

  return qpprox + p * qpprox * (v4sfl (1.0f) - qpprox * qpprox);
}

static inline v4sf
vfastcosfull (const v4sf x)
{
  static const v4sf halfpi = v4sfl (1.5707963267948966f);
  return vfastsinfull (x + halfpi);
}

static inline v4sf
vfastercosfull (const v4sf x)
{
  static const v4sf halfpi = v4sfl (1.5707963267948966f);
  return vfastersinfull (x + halfpi);
}

static inline v4sf
vfasttan (const v4sf x)
{
  static const v4sf halfpi = v4sfl (1.5707963267948966f);
  return vfastsin (x) / vfastsin (x + halfpi);
}

static inline v4sf
vfastertan (const v4sf x)
{
  return vfastersin (x) / vfastercos (x);
}

static inline v4sf
vfasttanfull (const v4sf x)
{
  static const v4sf twopi = v4sfl (6.2831853071795865f);
  static const v4sf invtwopi = v4sfl (0.15915494309189534f);

  v4si k = v4sf_to_v4si (x * invtwopi);

  v4sf half = v4sfl (-0.5f);
  v4sf ltzero = _mm_cmpnlt_ps (x, v4sfl (0.0f));
  half += _mm_and_ps (ltzero, v4sfl (1.0f));
  
  v4sf xnew = x - (half + v4si_to_v4sf (k)) * twopi;

  return vfastsin (xnew) / vfastcos (xnew);
}

static inline v4sf
vfastertanfull (const v4sf x)
{
  static const v4sf twopi = v4sfl (6.2831853071795865f);
  static const v4sf invtwopi = v4sfl (0.15915494309189534f);

  v4si k = v4sf_to_v4si (x * invtwopi);

  v4sf half = v4sfl (-0.5f);
  v4sf ltzero = _mm_cmpnlt_ps (x, v4sfl (0.0f));
  half += _mm_and_ps (ltzero, v4sfl (1.0f));
  
  v4sf xnew = x - (half + v4si_to_v4sf (k)) * twopi;

  return vfastersin (xnew) / vfastercos (xnew);
}

#endif //__SSE2__

#endif // __FAST_TRIG_H_
