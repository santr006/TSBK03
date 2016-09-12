# TSBK03
Real-time underwater caustics. 
Uses vertex, geometry and fragment shaders to compute different render passes. Projection is used to find where the sunrays hit the water surface and then hit the ocean floor.
![result](https://github.com/santr006/santr006.github.io/blob/master/convexSurfaceCaustics.png)
The blue is the water surface, the green is a plane below the water and the 
yellow dots are the positions where the photons emitted from the light source landed.
The light source is straight above the water and the camera is in front of the scene looking down at the plane.
