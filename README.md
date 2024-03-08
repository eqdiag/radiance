# radiance





All dependencies are self-contained in this project using git's submodule system.

## Dependencies
- tiny_obj: Loading 3d model obj files
- tiny_ply: Loading 3d model ply files
- stb_image: Loading image files
- tiny_exr: Loading HDR image files
- pugixml: Parsing xml files for scene format


## Features
### Assignment 1
- [x] color gradient
- [x] rays from cam
- [x] sphere interesection
- [x] command line args
- [x] camera control
- [x] add many spheres
- [x] examples
- [x] lambertian lighting
- [x] antialiasing
- [x] add materials
- [x] scene reorg for recursion
- [x] mirrors
- [ ] glass
- [ ] defocus blur
### Assignment 2
- [ ] triangle
- [ ] tri mesh
- [ ] parsing scene files
- [ ] intersect axis-aligned bounding box
- [ ] bounding volume hierarchy
- [ ] implement timer
- [ ] bvh optimizations
- [ ] motion blur
- [ ] object instancing

### Assignment 3
- [ ] textures
- [ ] shading normals
- [ ] fresnel reflection
- [ ] area lights
- [ ] perlin noise
- [ ] volumes
- [ ] schlick fresnel
- [ ] stratified sampling
- [ ] sample 1 light at a time
- [ ] better spherical light sampling
- [ ] better triangle sampling

### Assignment 4
- [ ] diffuse interreflection
- [ ] non-diffuse 
- [ ] russian roulette
- [ ] GGX/Trowbridge-Reitz microfacet
- [ ] rough dielecetric bsdgf
- [ ] determnistic MIS

### Bonus
- [ ] replace random gen with PCG
- [ ] parallel (openmp, not working on my mac :( )


## Installation

```
git clone https://github.com/eqdiag/radiance
cd radiance
git submodule update --init

mkdir build
cd build
cmake ..
make
```

## Usage
The CLI version of *radiance* contains many options than be passed to control the rendering behavior.
They are organized by parts of the scene that will be affected.

### Camera params
```
[-w,--width] = 256: Width of the image 
[-h,--height] = 256: Height of the image
[-vfov] = 90: Vertical field of view for the camera 
[-lookfrom] = [0,0,0]: Eye/Origin of the camera 
[-lookAt] = [0,0,-1]: Forward view direction of the camera 
```
### File params
```
[-o,--out] = out.png: Output file name of image, stored in ./output directory
```

## Screenshots 