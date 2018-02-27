# Introduction to Computer Graphics
## Project I 
### Simple Ray Tracing

It is a simple ray tracer that renders a scene using ambient illumination (meaning, constant colors for objects), 
checking only for shadow and multiplying colors by 0.1 if the intersection point is in shadow. 
The eye point is assumed at the origin (0,0,0); and the center of the screen is at (0,0,100).
The screen extends from (-50,-50,100) to (50,50,100) and the resolution is to be 1000x1000 pixels. 
There is only one light source and it is at (500,500,500). 

Inputs
- number of spheres \- N
- color of sphere i (R,G,B) \- i = 1,...,N
- position sphere i (x,y,z) \- i = 1,...,N
- radius of sphere i \- i = 1,...,N

Sample output for given parameters
- Number of spheres: 2
- Color "R,G,B" of sphere  #1: 255,0,0
- Position "x, y, z" of sphere #1: 50,50,300
- Radius of sphere #1: 20
- Color "R,G,B" of sphere  #2: 0,255,0
- Position "x, y, z" of sphere #2: 100,100,600
- Radius of sphere #2: 60


![](https://github.com/ptpam/2018_Spring/blob/master/Intro_to_Computer_Graphics/project_1/fig.png)
