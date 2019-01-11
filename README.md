# LargeVolumeRendering
This project was mainly to experiment and learn more about volume rendering in OpenGL.

## Goal
To research how to do volume rendering and to make an OpenGL/GLEW framework that allows me to work faster.

## How it works
Firstly, the volume data is represented using signed distance fields(SDF's).  SDF's are great for volume representation for several reasons.  They are easy to perform constructive solid geometry(CSG) which allows for easy to understand tools.  SDF's are also very easy to render using ray marching which can be relatively fast if optimized.  SDF's also are useful for many other shading techniques as well as useful for physics simulations to determine collisions.  

In earlier versions of this project the SDF was represented by a single 256^3 RGBA32F(~268MB) texture.  This texture was driven by a compute shader and seeded on start with random noise.  This allowed for infinite edits to the same area however was very memory intensive and costly to edit.  While it does perform rather well it is limited to a fixed size.  3D clipmaps were also considered because they could allow for much larger play areas.  However, they would need complex data structures like octrees or N^3 trees to be memory efficient.

This version is currently investigating what I believe to be a better option.  The scene will be broken up into uniform chunks of a given size which contain all of the relevant SDF's that make up that chunk.  The chunks will be stored into a simple 3D volume that contains the location of the local chunk data in memory.  The volume is then a circular array centered around the player allowing for the CPU to give the GPU the necessary chunks to make the SDF.  

