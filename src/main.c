/*
 Legendre second order method for linear advection
 Copyright (C) 2012 Borja Latorre - borja.latorre@csic.es

 For the latest updates, please visit:
 https://github.com/B0RJA/legendre-1D

 This program is free software: you can redistribute it
 and/or modify it under the terms of the GNU General Public
 License as published by the Free Software Foundation,
 either version 3 of the License, or (at your option) any
 later version.

 This program is distributed in the hope that it will be
 useful, but WITHOUT ANY WARRANTY; without even the implied
 warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public
 License along with this program. If not, see:
 http://www.gnu.org/licenses/
*/

#include <stdio.h>
#include <stdlib.h>
#include "legendre.h"
#include "cJSON.h"

int main(int argc, char *argv[])
{

	// Parse config file
	cJSON *root = cJSON_ParseFile("config.js");
	if (root == NULL)
	{
		fprintf(stderr, "Error parsing config.js\n");
		return 1;
	}

	// Read parameters
	double domain_size =
		cJSON_GetObjectItem(root, "domain-size")->valuedouble;
	double simulation_time =
		cJSON_GetObjectItem(root, "simulation-time")->valuedouble;
	double cfl =
		cJSON_GetObjectItem(root, "cfl")->valuedouble;
	int cell_count =
		cJSON_GetObjectItem(root, "cell-count")->valueint;
	char *results_file =
		cJSON_GetObjectItem(root, "results-file")->valuestring;

	// Variables
	double dx, time, dt;

	// Vectors
	double *m;		// Scalar
	double *mt;		// Updated scalar
	double *v;		// Velocity
	double *d;		// Node displacement

	// Matrix operators
	double A[4], B[4];

	// Auxiliary variables
	int   i, j, k, n;
	double c1, c2, c3;
	char file_name[1024];

	// Memory allocation
	n = cell_count * 2;
	m  = (double*) calloc(n, sizeof(double));
	mt = (double*) calloc(n, sizeof(double));
	v  = (double*) calloc(n, sizeof(double));
	n = cell_count;
	d  = (double*) calloc(n, sizeof(double));

	// Read initial values for m
	sprintf(file_name, "initial/m_%d.txt", cell_count);
	if(read_input(file_name, m, cell_count))
	{
		return 1;
	}

	// Read initial values for v
	sprintf(file_name, "initial/v_%d.txt", cell_count);
	if(read_input(file_name, v, cell_count))
	{
		return 1;
	}

	// Cell size
	dx = domain_size / cell_count;

	// Time step size
	dt = cfl * dx / max_velocity(v, cell_count);

	// Compute node displacements
	displacements_RK2(v, dx, dt, cell_count, d);

	// Main loop
	time = 0.0;
	while(time < simulation_time)
	{

		if(time + dt > simulation_time)
		{
			// Adjust final time step size
			dt = simulation_time - time;
			// Re-compute node displacements
			displacements_RK2(v, dx, dt, cell_count, d);
		}

		// New state calculus
		n = cell_count;
		for( i = 0 ; i < n ; i++ )
		{
			j = i - 1;
			if( j == -1 ){
				j = n-1;
			}
			k = i + 1;
			if( k == n ){
				k = 0;
			}
			c1 = d[j] / 2.0;
			c2 = d[i] / 2.0;
			c3 = d[k] / 2.0;
			compute_matrix_A(A, c1, c2);
			compute_matrix_B(B, c2, c3);
			mt[i*2  ] =  A[0] * m[j*2] + A[1] * m[j*2+1];
			mt[i*2+1] =  A[2] * m[j*2] + A[3] * m[j*2+1];
			mt[i*2  ] += B[0] * m[i*2] + B[1] * m[i*2+1];
			mt[i*2+1] += B[2] * m[i*2] + B[3] * m[i*2+1];
		}

		// Update scalar
		n = cell_count * 2;
		for( i = 0 ; i < n ; i++ )
		{
			m[i] = mt[i];
		}

		time += dt;

	}

	// Write results
	if(write_result(results_file, m, dx, cell_count))
	{
		return 1;
	}

	return 0;

}

