# radiance





All dependencies are self-contained in this project using git's submodule system.

## Dependencies
- tiny_obj: Loading 3d model obj files
- happly: Loading 3d model ply files
- stb_image: Loading image files
- tiny_exr: Loading HDR image files
- pugixml: Parsing xml files for scene format
- argh: Command line parsing


## Features
### Feature set 1
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
- [x] glass
- [x] defocus blur

### Feature set 2
- [x] triangle
- [x] tri mesh
- [x] reading ply files

- [x] implement timer
- [x] parsing scene files
    - [x] inline materials
    - [x] optional parse printing (cli option)
    - [x] add obj loading
    - [x] add ply loading
    - [ ] add rectangle loading
- [x] print render time

- [x] intersect axis-aligned bounding box
- [x] bounding volume hierarchy
- [x] object instancing (hittable + transform)
- [x] aabb for transformed objects

- [ ] get rectangles working
- [ ] make sure transformations are working w tri meshes (eg bunny examples)

### Feature set 3
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

### Feature set 4
- [ ] diffuse interreflection
- [ ] non-diffuse 
- [ ] russian roulette
- [ ] GGX/Trowbridge-Reitz microfacet
- [ ] rough dielecetric bsdgf
- [ ] determnistic MIS

## Minor cleanup
- [ ] Tri mesh triangle intersection speed up
    - [ ] creating too many tris + shared ptr copies
    - [ ] determinant based intersection
- [ ] Delete copy operators if ya don't need em
- [ ] Make sure persp camera is always ortho
- [ ] put everythning into radiance namespace
- [ ] make radiance a lib so exmaples compile faster
- [ ] make sure dependencies all in readme
- [ ] change scene function from shade to radiance

### Bonus
- [ ] replace random gen with PCG
- [ ] parallel (openmp, not working on my mac :( )
- [ ] motion blur
- [ ] add parsing for shape type "serialized"
- [ ] bvh optimizations
- [ ] add ply files (currently loader is broken :/)
- [ ] caching file names so i don't load them multiple times


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
