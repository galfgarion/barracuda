all:
	g++ -o raytracer main.cpp vector3.cpp && ./raytracer +W640 +H480 +Irecurse_simp.pov

testv3:
	g++ -o test vector3_test.cpp vector3.cpp && ./test

clean:
	rm -f a.out *.ppm *.tga test raytracer

handin:
	handin zwood csc473p1p2 *.h *.cpp simple.pov simple.ppm README Makefile *.vcproj *.sln
