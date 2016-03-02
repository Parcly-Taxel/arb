/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2016 Arb authors

******************************************************************************/

#include "bool_mat.h"

/*
 * Cycle detection, following
 * https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search
 */

typedef struct
{
    int *u; /* 'temporary mark' */
    int *v; /* 'permanent mark' */
    slong size;
} _cycle_detection_s;

static void
_cycle_detection_init(_cycle_detection_s *s, slong size)
{
    s->size = size;
    s->u = flint_calloc(size, sizeof(int));
    s->v = flint_calloc(size, sizeof(int));
}

static void
_cycle_detection_clear(_cycle_detection_s *s)
{
    flint_free(s->u);
    flint_free(s->v);
}

static int
_cycle_detection_visit(_cycle_detection_s *s, const bool_mat_t A, slong n)
{
    if (s->u[n])
        return 1;
    if (!s->v[n])
    {
        slong m;
        s->u[n] = 1;
        for (m = 0; m < s->size; m++)
            if (bool_mat_get_entry(A, n, m))
                if (_cycle_detection_visit(s, A, m))
                    return 1;
        s->v[n] = 1;
        s->u[n] = 0;
    }
    return 0;
}

int
bool_mat_is_nilpotent(const bool_mat_t A)
{
    slong n;

    if (!bool_mat_is_square(A))
    {
        flint_printf("bool_mat_is_nilpotent: a square matrix is required!\n");
        abort();
    }

    if (bool_mat_is_empty(A))
        return 0;

    n = bool_mat_nrows(A);

    if (n == 1)
    {
        return !bool_mat_get_entry(A, 0, 0);
    }
    else
    {
        _cycle_detection_s s;
        slong i;
        int has_cycle;

        _cycle_detection_init(&s, n);

        for (has_cycle = 0, i = 0; !has_cycle && i < n; i++)
            if (!s.v[i])
                has_cycle = _cycle_detection_visit(&s, A, i);

        _cycle_detection_clear(&s);

        return !has_cycle;
    }
}
