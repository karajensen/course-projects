This project is based on the Metaballs demonstration made by Rodrigo Diaz.


Where to start:
-Open the scene folder and load the example scene provided.

How does it work:
-The particles are simple spheres with a shader applied to create metaballs. These allow us to turn our spheres into a fluid.
-A camera looks at the particles and puts them on a texture (RenderTextures)
-This texture is then rendered to a plane which the player views.
-The metaball shader then overrides the colors and tweaks them for the liquid effect.
-The shader is applied to a Material wich uses the RenderTexture.

Tweaking particle behaviours:
-In the script folder you will find the "LiquidParticle" script. This is where the particles are defined.

Tweaking the colors:
-Open the shaders folder
-Create a duplicate of the "Metaball_Multiple" shader, change its name and open it.
-At the top of the shader, rename it again.
-On the "frag" method play around the 3 different definitions for each color.
-Define the color of the particle first in the finalColor variable;
-To tweak the metaball values play around the 2 float values inside the floor function.
-On the Materials folder, Select "LiquidRenderTexture"
-Change the shader to your shader.



