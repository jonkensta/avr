#!/usr/bin/env python2

from __future__ import division

import numpy as np


def build_table(xvals):
    from sympy import var, sin, integrate, pi

    x = var('x')
    f = sin((pi / 255) * x) ** 2
    F = integrate(f, x)
    norm = F.evalf(n=15, subs={x: 255})
    F = 100 * F / norm

    @np.vectorize
    def forward(x_):
        return float(F.evalf(n=15, subs={x: x_}))

    xp = np.linspace(0, 255, num=1000, endpoint=True)
    yp = forward(xp)

    def inverse(y_):
        return np.interp(y_, yp, xp)

    yvals = inverse(xvals).astype(np.uint8)

    return yvals


def main():
    xvals = np.arange(0, 100+1)
    yvals = build_table(xvals)
    np.savetxt('lookup_table.inc', yvals, fmt='%d', newline=',\n')


if __name__ == '__main__':
    main()
