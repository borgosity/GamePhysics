Thanks for downloading my Liquid Phyiscs project!
The project shows a cool approach for doing liquids in unity without being to expensive in performance.
If you need more liquid volume than the one showed here consider faking the effect with containers that doesn't hold any particles.

For more visit http://codeartist.mx/
Credit: Rodrigo Fernandez Diaz
Contact: q_layer@hotmail.com

Please! If you use this in a project somehow, let me know!
Credit is all I ask for, so please consider it!

///// Before starting:
-You need unity pro for this example. (Due to the render textures, which make the whole thing)

///// Where to start:
-Open the scene folder and press play!
-After that toggle the different examples and play around them.
-Inside every example there is particle generators, you can select the particle state and the frequency as well as the lifetime of each particle.

///// How does it work?
-The particles are simple spheres with some visual effects to look the behave like liquid particles.
-A camera only looks this particles and puts this on a texture (Aka RenderTextures)
-The texture goes into a plane which will be what our player views.
-There is a shader that overrides colors and tweaks them for the liquid effect (Metaballs).
-The shader is applied to a Material wich uses the RenderTexture.
-You can find a extended explanation on how this project works over here: http://codeartist.mx/tutorials/liquids/

///// Tweaking particle behaviours:
-In the script folder take a look on the "DynamicParticle" script, this is the definition of each particle.
-"OnCollisionEnter2D" holds the definition for Water+Lava=Water->Gas, you can tweak this as you wish.
-On the "SetState" method you can adjust each state physic properties.

///// Tweaking the colors:
-Open the shaders folder
-Create a duplicate of the "Metaball_Multiple" shader, change its name and open it.
-At the top of the shader, rename it again.
-On the "frag" method play around the 3 different definitions for each color.
-Define the color of the particle first in the finalColor variable;
-To tweak the metaball values play around the 2 float values inside the floor function.
-On the Materials folder, Select "LiquidRenderTexture"
-Change the shader to your shader.



