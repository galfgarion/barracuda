all: compile
	./raytracer +W640 +H480 +Irecurse_simp2.pov
	povray +W640 +H480 recurse_simp2.pov

compile:
	g++ -o raytracer main.cpp vector3.cpp

simple: compile
	./raytracer +W640 +H480 +Isimple.pov

simplereflect: compile
	./raytracer +W640 +H480 +Isimple_reflect.pov

simplerefract: compile
	./raytracer +W640 +H480 +Isimple_refract.pov

simplespecular: compile
	./raytracer +W640 +H480 +Isimple_spec.pov

testv3:
	g++ -o test vector3_test.cpp vector3.cpp && ./test

clean:
	rm -f a.out *.ppm *.tga *.png test raytracer

test:
	g++ test.cpp vector3.cpp && ./a.out

handin:
	handin zwood csc473p1p3 *.h *.cpp simple.pov simple.ppm README Makefile *.vcproj *.sln

