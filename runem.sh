#!/usr/bin/perl

my $HOME = $ENV{HOME};

# where mathematica will find them:
my $dir = "$HOME/winhome/OneDrive/Documents"
#my $dir = "$HOME/Downloads"

my $params = '--xmin -2.0 --xmax 0.6 --ymin -1.2 --ymax 1.2 --nx 400';
system(qq(
time ./mpauli $params --output $dir/mp.txt
time ./mga20 $params --output $dir/mg.txt
time ./mcomplex $params --output $dir/mc.txt
));

#printslices();

exit 0;

sub printslices
{
    my $j = 0;
    for ( my $i = 0.0 ; $i <= 1.2 ; $i += 0.1 )
    {
        $j++;
        system(qq(time ./mpauli \\
            --xmin -2.0 --xmax 0.6 \\
            --ymin -1.2 --ymax 1.2 \\
            --zmin $i --zmax $i \\
            --nx 400 \\
            --thresh 4 \\
            --output $dir/mp2_400_$j.txt
        ));
    }
}
