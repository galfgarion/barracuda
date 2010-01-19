Pigment getPigment(char *line)
{
    string r, g, b;
    rule<> attr = ((+space_p) >> "pigment {color rgb <");
    rule<> value = (*digit_p) >> ch_p('.') >> (*digit_p);
    parse(line, (attr >>
                 value[assign_a(r)] >> str_p(", ") >>
                 value[assign_a(g)] >> str_p(", ") >>
                 value[assign_a(b)] >> ch_p('>')));
    cout << "r: " << r << " g: " << g << " b: " << b << endl;
}

Rotate getRotate(char *line)
{
    string x, y, z;
    rule<> attr = ((+space_p) >> (+alpha_p));
    rule<> number = *ch_p('-') >> ((*digit_p) | ((*digit_p) >> ch_p('.') >> (*digit_p)));
    parse(line, (attr >> str_p(" <") >> 
                 number[assign_a(x)] >> str_p(", ") >>
                 number[assign_a(y)] >> str_p(", ") >>
                 number[assign_a(z)] >> ch_p('>')));
    cout << "rotX: " << x << " rotY: " << y << " rotZ: " << z << endl;
}

Scale getScale(char *line)
{
    string x, y, z;
    rule<> attr = ((+space_p) >> (+alpha_p));
    rule<> number = ((*digit_p) | ((*digit_p) >> ch_p('.') >> (*digit_p)));
    parse(line, (attr >> str_p(" <") >> 
                 number[assign_a(x)] >> str_p(", ") >>
                 number[assign_a(y)] >> str_p(", ") >>
                 number[assign_a(z)] >> ch_p('>')));
    cout << "sX: " << x << " sY: " << y << " sZ: " << z << endl;
}

Attribute getAttribute(char *line)
{
    string attribute;
    
    parse(line, (+space_p) >> (+alpha_p)[assign_a(attribute)] >> (*anychar_p));
    
    if (!attribute.compare("scale"))
    {
        //cout << "scale" << endl;
        getScale(line);
    } 
    else if (!attribute.compare("rotate"))
    {
        //cout << "rotate" << endl;
        getRotate(line);
    }
    else if (!attribute.compare("pigment"))
    {
        getPigment(line);
    }
}

Triangle* parseTri(FILE *input, char *line)
{
    int i;
    rule<> floating = (*ch_p('-') >> (str_p("0.") | str_p("1.")) >> (*digit_p) );
    string x, y, z;
    string attr;
    for (i = 0; i < 3; i++)
    {
        line = getLine(input);
        parse(line, ((*space_p) >> ch_p('<') >> 
                    floating[assign_a(x)] >> str_p(", ") >> 
                    floating[assign_a(y)] >> str_p(", ") >> 
                    floating[assign_a(z)] >> ch_p('>')));
    }
    line = getLine(input);
    while (line[0] != '}')
    {
        getAttribute(line);
        //cout << attr << endl;
        line = getLine(input);
    }
}

vector<geomObj*> parseObjs(FILE *input, char *file)
{
    char *line;
    vector<geomObj*> objects;
    string type;
    int status;
    line = getLine(input);
    while (line != NULL)
    {
        status = parse(line, (+alpha_p)[assign_a(type)] >> " {" >> *anychar_p).full;
        if (status)
        {
            cout << type << endl;
            if (!type.compare("triangle"))
            {
                objects.push_back(parseTri(input, line));
                //cout << line << endl;
            }
            //cout << type << endl;
        }
        cout << endl;
        line = getLine(input);
    }
    //Triangle *blah = new Triangle::Triangle();
    //objects.push_back(blah);
    return objects;
}
