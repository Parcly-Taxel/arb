/*
    Copyright (C) 2016 Pascal Molin

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb_dirichlet.h"

int
acb_dirichlet_ui_parity(const acb_dirichlet_group_t G, ulong a)
{
    int par;

    par = 0;

    if (G->neven && a % 4 == 3)
        par++;

    if (n_jacobi_unsigned(a, G->rad_q) == -1)
        par++;

    return par % 2;
}
