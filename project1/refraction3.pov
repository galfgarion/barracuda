camera {
  location  <0, 0, 13> 
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}


light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}


plane { <0,1,0>, -5
  pigment { color rgb <1, 0.1, 1>}
  finish {
    ambient 0.2 
    diffuse 0.3
  }
}

sphere { <0, 0, 7>, 2
  pigment { color rgbf <1.0, 1.0, 1.0, 0.6>}
  finish {
    reflection 0
    refraction 1
    ior 1.33
  }
}

sphere { <2, -2, -1>, 2
  pigment { color rgb <1, 0, 0>}
  finish {
    ambient 0.1 
    diffuse 0.4
  }
}

sphere { <-2, -2, -1>, 2
  pigment { color rgb <0, 1, 0>}
  finish {
    ambient 0.1
    diffuse 0.4
  }
}

sphere { <0, 3, -1>, 2
  pigment { color rgb <0, 0, 1>}
  finish {
    ambient 0.1 
    diffuse 0.4
  }
}