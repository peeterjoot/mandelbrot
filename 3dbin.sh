#./mpauli --xmin -2.0 --xmax 0.6 --ymin -1.2 --ymax 1.2 --nx 400 \
#--zmin -1.2 --zmax 1.2 \
#--progress=25 \
#--netcdf \
#--filename ~/Downloads/mp3Dcolor.nc
#--bw \

./mprojection --xmin -0.6 --xmax 0.6 --ymin -0.6 --ymax 0.6 --nx 400 \
--asimage \
--thresh 30 \
--filename fp.png
#--bw \


#./mpauli --xmin -2.0 --xmax 0.6 --ymin -1.2 --ymax 1.2 --nx 400 \
#--bw \
#--filename mpbw.png --asimage
#
#./mpauli --xmin -2.0 --xmax 0.6 --ymin -1.2 --ymax 1.2 --nx 400 \
#--filename mp.png --asimage

#./mpauli --xmin -2.0 --xmax 0.6 --ymin -1.2 --ymax 1.2 --nx 400 \
#--filename mp.txt

#./mpauli --xmin -2.0 --xmax 0.6 --ymin -1.2 --ymax 1.2 --nx 400 \
#--bw \
#--filename ~/winhome/OneDrive/Documents/mp.bin --binary
