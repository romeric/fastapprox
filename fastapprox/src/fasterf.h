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
#include "sse.h"
#include "fastexp.h"

// fasterfc: not actually faster on newer machines!
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

// fasterf: not actually faster on newer machines! 
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

#endif //__SSE2__

#endif // __FAST_ERF_H_
