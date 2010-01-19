class Camera
{
    public:
        Camera(void);
        ~Camera();
    protected:
        double loc_x, loc_y, loc_z;
        double up_x, up_y, up_z;
        double right_x, right_y, right_z;
        double look_x, look_y, look_z;
};

class Light_Source
{
    public:
        Light_Source(void);
        ~Light_Source(void);
    protected:
        double x, y, z;
        double r, g, b;
};

class geomObj
{
    public:
        geomObj();
        //~geomObj();
        void parse_pigment(FILE *fp);
    protected:
        double tx, ty, tz;
        double sx, sy, sz;
        double theta;
};
geomObj::geomObj(){}
class Box:public geomObj
{
    public:
        Box(void);
        ~Box();
    protected:
        double x1, y1, z1;
        double x2, y2, z2;
};

class Sphere:public geomObj
{
    public:
        Sphere(void);
        ~Sphere();
    protected:
        double cx, cy, cz;
        double rad;
};

class Cone:public geomObj
{
    public:
        Cone(void);
        ~Cone();
    protected:
        double cen_x1, cen_y1, cen_z1;
        double rad1;
        double cen_x2, cen_y2, cen_z2;
        double rad2;
};

class Triangle:public geomObj
{
    public:
        Triangle(void);
        ~Triangle();
        Triangle& parse(FILE *);
    protected:
        double corn_x1, corn_y1, corn_z1;
        double corn_x2, corn_y2, corn_z2;
        double corn_x3, corn_y3, corn_z3;
};

Triangle* parse(FILE *) {
   return new Triangle();
}

class Plane:public geomObj
{
    public:
        Plane(void);
        ~Plane();
    protected:
        double norm_x, norm_y, norm_z;
        double distance;
};

class Attribute
{
    public:
        Attribute(void);
        ~Attribute();
    protected:
};

class Scale:public Attribute
{
    public:
        Scale(void);
        ~Scale();
    protected:
        double x, y, z;
};

class Rotate:public Attribute
{
    public:
        Rotate(void);
        ~Rotate();
    protected:
        double x, y, z;
};

class Pigment:public Attribute
{
    public:
        Pigment(void);
        ~Pigment();
    protected:
        double r, g, b;
};

Triangle::Triangle(void){}
Attribute::Attribute(void){}
Attribute::~Attribute(void){}
Scale::Scale(void){}
Scale::~Scale(void){}
Rotate::Rotate(void){}
Rotate::~Rotate(void){}
Pigment::Pigment(void){}
Pigment::~Pigment(void){}
