default: simplerecurse2

make simplecam: simplecam0 simplecam1 simplecam2 simplecam3

simplecam0: compile
	./raytracer +W640 +H480 +Isimp_cam0.pov
simplecam1: compile
	./raytracer +W640 +H480 +Isimp_cam1.pov
simplecam2: compile
	./raytracer +W640 +H480 +Isimp_cam2.pov
simplecam3: compile
	./raytracer +W640 +H480 +Isimp_cam3.pov

simplerecurse2: compile
	./raytracer +W640 +H480 +Irecurse_simp2.pov

marble2: compile
	./raytracer +W640 +H480 +Imarble2.pov

compile:
	g++ -g -o raytracer main.cpp matrix4x4.cpp vector3.cpp

simple: compile
	./raytracer +W640 +H480 +Isimple.pov

simplereflect: compile
	./raytracer +W640 +H480 +Isimple_reflect.pov

simplereflect2: compile
	./raytracer +W640 +H480 +Isimple_reflect2.pov

simplerefract: compile
	./raytracer +W800 +H600 +Isimple_refract.pov

simpletri: compile
	./raytracer +W640 +H480 +Isimple_tri.pov

simplespecular: compile
	./raytracer +W640 +H480 +Isimple_spec.pov

testv3:
	g++ -o test vector3_test.cpp vector3.cpp && ./test

clean:
	rm -f a.out *.ppm *.tga *.png test raytracer

test:
	g++ test.cpp vector3.cpp && ./a.out

matrixtest:
	g++ -o matrix_test vector3.cpp matrix_test.cpp matrix4x4.cpp && ./matrix_test

required: clean simplereflect simplereflect2 simplerefract simpletri simplerecurse2

handin:
	handin zwood csc473p1p3 *.h *.cpp *.pov *.ppm README Makefile

