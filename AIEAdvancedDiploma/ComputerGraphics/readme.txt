
TEMPORARY UNTIL COMPLETING TERRAIN
q1 = s1T + t1B  ->  a = bT + cB  
q2 = s2T + t2B  ->  d = eT + fB
B = (a - bT)/c
d = eT + f((a - bT)/c)
d = eT + fa/c - fbT/c
d - fa/c = eT - (fb/c)T
d - fa/c = (e - (fb/c))T
T = (d - fa/c) / (e - (fb/c))

=================================================================
AIE Advanced Diploma - Computer Graphics Unit
=================================================================
mail@KaraJensen.com
www.KaraJensen.com
https://github.com/karajensen/course-projects.git
=================================================================

RELEASE REQUIREMENTS: Windows 7+
BUILD REQUIREMENTS: Windows 7+, Visual Studio 2013 (v120)
NOTES: The fbx loader libary requires unzipping before building

=================================================================

INPUT:
1 -> 9     Render a specific layer
0          Toggle Wireframe
P          Save procedural textures to file
O          Reload the procedural scene
WASDQE     Move the camera
LMC        Rotate the camera

=================================================================