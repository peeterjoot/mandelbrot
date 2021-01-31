#!/usr/bin/perl

#time ./mpauli --xmin -2.0 --xmax 0.6 --ymin -1.2 --ymax 1.2 --nx 400 --output ~/Downloads/mp.txt
#time ./mcomplex --xmin -2.0 --xmax 0.6 --ymin -1.2 --ymax 1.2 --nx 400 --output ~/Downloads/mc.txt

# 8s : --nx 50 --thresh 4 
#time ./mpauli \
#--xmin -2.0 --xmax 0.6 \
#--ymin -1.2 --ymax 1.2 \
#--zmin -1.2 --zmax 1.2 \
#--nx 50 \
#--thresh 4 \
#--output ~/Downloads/mp3_50_4.txt

my $j = 0;
my $HOME = $ENV{HOME};
for ( my $i = 0.0 ; $i <= 1.2 ; $i += 0.1 )
{
$j++;
system(qq(
time ./mpauli \\
--xmin -2.0 --xmax 0.6 \\
--ymin -1.2 --ymax 1.2 \\
--zmin $i --zmax $i \\
--nx 400 \\
--thresh 4 \\
--output $HOME/Downloads/mp2_400_$j.txt
));
}
