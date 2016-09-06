/*
    Copyright (C) 2015 Jonathan Bober
    Copyright (C) 2016 Fredrik Johansson
    Copyright (C) 2016 Pascal Molin

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_dirichlet.h"

void
acb_dirichlet_chi(acb_t res, const acb_dirichlet_group_t G, const acb_dirichlet_char_t chi, ulong n, slong prec)
{
    ulong expo;
    expo = acb_dirichlet_ui_chi(G, chi, n);
    if (expo == ACB_DIRICHLET_CHI_NULL)
        acb_zero(res);
    else
    {
        fmpq_t t;
        fmpq_init(t);
        fmpq_set_si(t, 2 * expo , chi->order.n);
        arb_sin_cos_pi_fmpq(acb_imagref(res), acb_realref(res), t, prec);
        fmpq_clear(t);
    }
}
