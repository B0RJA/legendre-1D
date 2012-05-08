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

////////////////////////////////////////////////////////////
// Read initial values from file
////////////////////////////////////////////////////////////

int read_input(char *fn, double *m, long n){

	long i;
	FILE *fp;

	if((fp=fopen(fn, "r")) == NULL) {
		printf("Inital file not found: %s\n", fn);
		printf("Get help on how to create it on:\n");
		printf("https://github.com/B0RJA/legendre-1D\n");
		return(1);
	}
	for( i = 0 ; i < n ; i++ ){
		fscanf(fp, "%lf", &(m[i*2]));
		fscanf(fp, "%lf", &(m[i*2+1]));
	}
	fclose(fp);

	return(0);

}

////////////////////////////////////////////////////////////
// Write results distribution to file
////////////////////////////////////////////////////////////

int write_result(char *fn, double *m, double dx, long n){

	long i;
	double x;
	FILE *fp;

	if((fp=fopen(fn, "w")) == NULL) {
		printf("Can't write to file: %s\n", fn);
		return(1);
	}
	fprintf(fp, "#x\tm\n");
	x = 0.0;
	for( i = 0 ; i < n ; i++ ){
		fprintf(fp, "%.9e\t%.9e\n", x   , m[i*2]-m[i*2+1]);
		fprintf(fp, "%.9e\t%.9e\n", x+dx, m[i*2]+m[i*2+1]);
		x = x + dx;
	}
	fclose(fp);

	return(0);

}

////////////////////////////////////////////////////////////
// Maximum velocity
////////////////////////////////////////////////////////////

double max_velocity(double *v, long n){

	long i;
	double x1;
	double max, min;

	max = -1.E15;
	min =  1.E15;

	for( i = 0 ; i < n ; i++ ){
		x1 = v[i*2] - v[i*2+1];
		if( x1 > max ){
			max = x1;
		}
		if( x1 < min ){
			min = x1;
		}
		x1 = v[i*2] + v[i*2+1];
		if( x1 > max ){
			max = x1;
		}
		if( x1 < min ){
			min = x1;
		}
	}

	if(min <= 0.0){
		printf("Negative velocities are not supported.\n");
		printf("Write an issue on:\n");
		printf("https://github.com/B0RJA/legendre-1D\n");
		return(0.0);
	}

	return(max);

}

////////////////////////////////////////////////////////////
// T matrix calculus
////////////////////////////////////////////////////////////

void compute_matrix_T(
	double *T, double a, double b, double c, double d){

	T[0] = a;
	T[1] = a*b;
	T[2] = 3.0*a*d;
	T[3] = a*(3.0*b*d+a*c);

}

////////////////////////////////////////////////////////////
// B matrix calculus
////////////////////////////////////////////////////////////

void compute_matrix_A(double *A, double c1, double c2){

	double x;
	x = 1.0 / (1.0 + c2 - c1);
	compute_matrix_T(A, c2*x, (1.0-c1)*x, c2, c2-1.0);

}

////////////////////////////////////////////////////////////
// A matrix calculus
////////////////////////////////////////////////////////////

void compute_matrix_B(double *B, double c1, double c2){

	double x;
	x = 1.0 / (1.0 + c2 - c1);
	compute_matrix_T(B, (1.0-c1)*x, -c2*x, 1.0-c1, c1);

}

////////////////////////////////////////////////////////////
// Compute node displacements
////////////////////////////////////////////////////////////

void displacements_RK2(
	double *v, double dx, double dt, long n, double *d){

	double x1, v1, v2;
	long i;

	for( i = 0 ; i < n ; i++ ){
		x1 = -1.0;
		v1 = v[i*2]+x1*v[i*2+1];
		x1 = -1.0 + 2.0*v1*dt/dx;
		v2 = v[i*2]+x1*v[i*2+1];
		d[i] = (v1+v2)*dt/dx; // 2nd order
		//d[i] = 2.0*v1*dt/dx; // 1st order
	}

}

