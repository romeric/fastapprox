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

#ifndef __FAST_LAMBERT_W_H_
#define __FAST_LAMBERT_W_H_

#include <stdint.h>
#include "fastexp.h"
#include "fastlog.h"
#include "sse.h"

// these functions compute the upper branch aka W_0

static inline float
fastlambertw (float x)
{
  static const float threshold = 2.26445f;

  float c = (x < threshold) ? 1.546865557f : 1.0f;
  float d = (x < threshold) ? 2.250366841f : 0.0f;
  float offset = (x < threshold) ? -0.737769969f : 0.0f;

  float logterm = fastlog (c * x + d);
  float loglogterm = fastlog (logterm);

  float w = offset + logterm - loglogterm + loglogterm / logterm;
  float expw = fastexp (w);
  float z = w * expw;
  float p = x + z;

  return (2.0f * x + w * (4.0f * x + w * p)) /
         (2.0f * expw + p * (2.0f + w));
}

static inline float
fasterlambertw (float x)
{
  static const float threshold = 2.26445f;

  float c = (x < threshold) ? 1.546865557f : 1.0f;
  float d = (x < threshold) ? 2.250366841f : 0.0f;
  float offset = (x < threshold) ? -0.737769969f : 0.0f;

  float logterm = fasterlog (c * x + d);
  float loglogterm = fasterlog (logterm);

  float w = offset + logterm - loglogterm + loglogterm / logterm;
  float expw = fasterexp (-w);

  return (w * w + expw * x) / (1.0f + w);
}

#ifdef __SSE2__

static inline v4sf
vfastlambertw (v4sf x)
{
  static const v4sf threshold = v4sfl (2.26445f);

  v4sf under = _mm_cmplt_ps (x, threshold);
  v4sf c = _mm_or_ps (_mm_and_ps (under, v4sfl (1.546865557f)),
                      _mm_andnot_ps (under, v4sfl (1.0f)));
  v4sf d = _mm_and_ps (under, v4sfl (2.250366841f));
  v4sf offset = _mm_and_ps (under, v4sfl (-0.737769969f));

  v4sf logterm = vfastlog (c * x + d);
  v4sf loglogterm = vfastlog (logterm);

  v4sf w = offset + logterm - loglogterm + loglogterm / logterm;
  v4sf expw = vfastexp (w);
  v4sf z = w * expw;
  v4sf p = x + z;

  return (v4sfl (2.0f) * x + w * (v4sfl (4.0f) * x + w * p)) /
         (v4sfl (2.0f) * expw + p * (v4sfl (2.0f) + w));
}

static inline v4sf
vfasterlambertw (v4sf x)
{
  static const v4sf threshold = v4sfl (2.26445f);

  v4sf under = _mm_cmplt_ps (x, threshold);
  v4sf c = _mm_or_ps (_mm_and_ps (under, v4sfl (1.546865557f)),
                      _mm_andnot_ps (under, v4sfl (1.0f)));
  v4sf d = _mm_and_ps (under, v4sfl (2.250366841f));
  v4sf offset = _mm_and_ps (under, v4sfl (-0.737769969f));

  v4sf logterm = vfasterlog (c * x + d);
  v4sf loglogterm = vfasterlog (logterm);

  v4sf w = offset + logterm - loglogterm + loglogterm / logterm;
  v4sf expw = vfasterexp (-w);

  return (w * w + expw * x) / (v4sfl (1.0f) + w);
}

#endif // __SSE2__

#endif // __FAST_LAMBERT_W_H_
