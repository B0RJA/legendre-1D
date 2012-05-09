# Legendre second order method for linear advection
# Copyright (C) 2012 Borja Latorre - borja.latorre@csic.es
#
# For the latest updates, please visit:
# https://github.com/B0RJA/legendre-1D
#
# This program is free software: you can redistribute it
# and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any
# later version.
#
# This program is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
# PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public
# License along with this program. If not, see:
# http://www.gnu.org/licenses/

# TODO: Optimize the maxima API by keeping the command open
#       or compute all the integrals at the same time.

# Program configuration
$cell_count  = 100;
$domain_size = 1.0;
$scalar      = 'exp( - 50 * ( x - 1 / 2 ) ^ 2 )';
$velocity    = '1 / ( 1 + cos( 2 * 3.141592653589793 * x ) / 5 )';
$order       = 2;
# Change depending on your OS
$program     = 'maxima';	# UNIX
#$program    = 'maxima.exe';	# Windows

$dx = $domain_size / $cell_count;

# Check maxima installation
$int = maxima('romberg(legendre_p(0,x),x,-1,1)');
$int = sprintf("%.7f", $int);
if($int ne '2.0000000')
{
	print "Maxima 5.21.1 or greater is required.\n";
	print "http://maxima.sourceforge.net\n";
	print "If already installed, check variable \$program in the script.\n";
	exit;
}

# Integrate scalar
open F, '>'.'initial/m_'.$cell_count.'.txt';
for( $i=0 ; $i<$cell_count ; $i++ )
{
	$x1 = $i * $dx;
	$x2 = ( $i + 1 ) * $dx;
	$f = $scalar;
	$f =~ s/exp/eyp/g;
	$f =~ s/x/\($i\+\(x\+1\)\/2\)\*$domain_size\/$cell_count/g;
	$f =~ s/eyp/exp/g;
	for( $l=1 ; $l<=$order ; $l++ )
	{
		$int = maxima('(2*'.$l.'-1)/2*romberg(legendre_p('.$l.'-1,x)*'.$f.',x,-1,1)');
		if($int eq ''){
			$int = 0.0;
		}
		$int = sprintf("%.13le", $int);
		print F $int."\n";
	}
}
close F;

# Integrate velocity
open F, '>'.'initial/v_'.$cell_count.'.txt';
for( $i=0 ; $i<$cell_count ; $i++ )
{
	$x1 = $i * $dx;
	$x2 = ( $i + 1 ) * $dx;
	$f = $velocity;
	$f =~ s/exp/eyp/g;
	$f =~ s/x/\($i\+\(x\+1\)\/2\)\*$domain_size\/$cell_count/g;
	$f =~ s/eyp/exp/g;
	for( $l=1 ; $l<=$order ; $l++ )
	{
		$int = maxima('(2*'.$l.'-1)/2*romberg(legendre_p('.$l.'-1,x)*'.$f.',x,-1,1)');
		if($int eq ''){
			$int = 0.0;
		}
		$int = sprintf("%.13le", $int);
		print F $int."\n";
	}
}
close F;

exit;

# Maxima API
sub maxima
{
	my $start = $program . ' --batch-string="display2d:false;';
	my $end = ';" -q';
	my $cmd = $start . 'float('.$_[0].')' . $end;
	open(PS_F, "$cmd|");
	my $save = 0;
	my $line = '';
	while (<PS_F>)
	{
		my $x = $_;
		if( $x =~ /\(\%o2\)/ )
		{
			$save = 1;
			$x =~ s/\(\%o2\)//g;
		}
		if( $save == 1 )
		{
			$line = $line . $x;
		}
	}
	close(PS_F);
	$line =~ s/[\r\n\s]+//g;
	return $line;
}