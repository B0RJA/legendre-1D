Legendre second order method for linear advection
=================================================

Installation and usage
----------------------

Download [the latest release](https://github.com/B0RJA/legendre-1D/zipball/master), compile the source with `make` and run the program: `legendre`.

Windows users can install [strawberry perl](http://strawberryperl.com/), wich includes gcc, and type in a console window:

    gcc main.c -lm -o legendre.exe
    ./legendre.exe

The file `results.txt` will be created with the computed scalar distribution:

    #x  m
    0.000000000e+00	-1.968156519e-06
    1.000000000e-02	-2.212635966e-06
    1.000000000e-02	-2.228769983e-06
    2.000000000e-02  -2.423903046e-06
    ...

As the scalar field is represented using piecewise-linear functions, two pair of points are given for each cell.

Configuration
-------------

Edit the file `config.js` to change the simulation parameters:

    {
      "domain-size": 1.0,
      "cell-count": 200,
      "simulation-time": 1.0,
      "cfl": 0.9,
      "results-file": "results.txt"
    }

Initial Legendre series for the scalar and velocity distributions are pre-computed using perl and [maxima](http://maxima.sourceforge.net/) are stored in the folder `initial/`.

Files corresponding to:

    Scalar distribution:
      m(x) = exp( - 50.0 * ( x - 0.5 ) ^ 2 )
    Velocity distribution:
      v(x) = 1 / ( 1 + cos( 2 * pi * x ) / 5  )

using 10, 100, 1000 and 10000 cells are included in the release.

Edit the perl script `series.pl` to compute the initial Legendre series for different cell number or in order to change the expressions of the scalar or velocity distributions.

    $cell_count  = 100;
    $domain_size = 1.0;
    $scalar      = 'e( - 50 * ( x - 0.5 ) ^ 2 )';
    $velocity    = '1 / ( 1 + 0.2 * cos( 2.0 * 3.141592653589793 * x ) )';

Run the script using:

    perl series.pl

and the corresponding files will be created in the folder `initial/`.

Copyright and license
---------------------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
See the file `COPYING` in this directory or  http://www.gnu.org/licenses/, for a description of the GNU General Public License terms under which you can copy the files.
