/*
    Copyright (C) 2016 Fredrik Johansson

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_dirichlet.h"

void acb_zeta_si(acb_t z, slong s, slong prec);

void
acb_dirichlet_zeta(acb_t res, const acb_t s, slong prec)
{
    acb_t a;
    double cutoff;

    if (acb_is_int(s) &&
        arf_cmpabs_2exp_si(arb_midref(acb_realref(s)), FLINT_BITS - 1) < 0)
    {
        acb_zeta_si(res, arf_get_si(arb_midref(acb_realref(s)), ARF_RND_DOWN), prec);
        return;
    }

    cutoff = 24.0 * prec * sqrt(prec);

    /* todo: support non-exact s properly in RS */
    if (acb_is_exact(s) &&
        arf_cmpabs_d(arb_midref(acb_imagref(s)), cutoff) >= 0 &&
        arf_cmpabs_d(arb_midref(acb_realref(s)), 10 + prec * 0.1) <= 0)
    {
        acb_dirichlet_zeta_rs(res, s, 0, prec);
        return;
    }

    acb_init(a);
    acb_one(a);

    if (arf_sgn(arb_midref(acb_realref(s))) < 0)
    {
        acb_t t, u, v;
        slong wp = prec + 6;

        acb_init(t);
        acb_init(u);
        acb_init(v);

        acb_sub_ui(t, s, 1, wp);

        /* 2 * (2pi)^(s-1) */
        arb_const_pi(acb_realref(u), wp);
        acb_mul_2exp_si(u, u, 1);
        acb_pow(u, u, t, wp);
        acb_mul_2exp_si(u, u, 1);

        /* sin(pi*s/2) */
        acb_mul_2exp_si(v, s, -1);
        acb_sin_pi(v, v, wp);
        acb_mul(u, u, v, wp);

        /* gamma(1-s) zeta(1-s) */
        acb_neg(t, t);
        acb_gamma(v, t, wp);
        acb_mul(u, u, v, wp);
        acb_hurwitz_zeta(v, t, a, wp);
        acb_mul(res, u, v, prec);

        acb_clear(t);
        acb_clear(u);
        acb_clear(v);
    }
    else
    {
        acb_hurwitz_zeta(res, s, a, prec);
    }

    acb_clear(a);
}

