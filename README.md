Legendre second-order method for linear advection
=================================================

Latorre, B., Garcia‐Navarro, P., Murillo, J., & Burguete, J. (2011). Accurate and efficient simulation of transport in multidimensional flow. International Journal for Numerical Methods in Fluids, 65(4), 405-431. ([preprint PDF](https://github.com/B0RJA/legendre-1D/raw/master/LatorreB_IntJNumMethFluids_2011.pdf))

Abstract: A new numerical method to obtain high-order approximations of the solution of the linear advection equation in multidimensional problems is presented. The proposed conservative formulation is explicit and based on a single updating step. Piecewise polynomial spatial discretization using Legendre polynomials provides the required spatial accuracy. The updating scheme is built from the functional approximation of the exact solution of the advection equation and a direct evaluation of the resulting integrals. The numerical details for the schemes in one and two spatial dimensions are provided and validated using a set of numerical experiments. Test cases have been oriented to the convergence and the computational efficiency analysis of the schemes.

Installation and usage
----------------------

* Download [the latest release](https://github.com/B0RJA/legendre-1D/zipball/master).
* Compile with `make`.
* Run `legendre`.

Windows users are recommended to install [strawberry perl](http://strawberryperl.com/), wich includes gcc and gmake, double-click in `winMake.bat` and then in `legendre.exe`.

The file `results.txt` will be created containing the computed scalar distribution:

    #x  m
    0.000000000e+00  -1.968156519e-06
    1.000000000e-02	 -2.212635966e-06
    1.000000000e-02	 -2.228769983e-06
    2.000000000e-02  -2.423903046e-06
    ...

As the scalar field m(x) is represented using piecewise-linear functions, two points are given for each cell.

Configuration
-------------

Edit the [JSON](http://www.json.org/) file `config.js` to change the simulation parameters:

    {
      "domain-size"     : 1.0,
      "cell-count"      : 100,
      "simulation-time" : 100.0,
      "cfl"             : 0.5,
      "results-file"    : "results.txt"
    }

Config file and source code are saved using Unix/Linux line endings. Windows users must use an appropriate text editor, i.e. [Notepad++](http://notepad-plus-plus.org/download/).

Initial Legendre series for the scalar and velocity distributions are pre-computed and stored in the folder `initial/`.

Files corresponding to:

    Domain:
      x = [ 0 , 1 ]
    Scalar distribution:
      m(x) = exp( - 50 * ( x - 1 / 2 ) ^ 2 )
    Velocity distribution:
      v(x) = 1 / ( 1 + cos( 2 * pi * x ) / 5  )

using 10, 100, 1000 and 10000 cells are included in the release.

Edit the perl script `series.pl` to compute the initial Legendre series for different cell number or in order to change the expressions of the scalar or velocity distributions:

    $cell_count  = 100;
    $domain_size = 1.0;
    $scalar      = 'exp( - 50 * ( x - 1 / 2 ) ^ 2 )';
    $velocity    = '1 / ( 1 + cos( 2 * 3.141592653589793 * x ) / 5 )';

Run the script:

    perl series.pl

to create the corresponding initial files that will be automatically saved in the folder `initial/`.

The integrals involved in the Legendre series are numerically computed using [maxima](http://maxima.sourceforge.net/), a GNU computer algebra system, that must be installed on your system. Windows users have to edit the script `series.pl` and change the value of the variable `$program` to `maxima.exe`.

Any problems?
-------------
Feel free to [write an issue](https://github.com/B0RJA/legendre-1D/issues) if you have any questions or problems.

Acknowledgements
----------------
The parsing of the config file is made thanks to the [cJSON](http://cjson.sourceforge.net/) implementation by Dave Gamble.

Copyright and license
---------------------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

See the file `COPYING` in this directory or  http://www.gnu.org/licenses/, for a description of the GNU General Public License terms under which you can copy the files.
