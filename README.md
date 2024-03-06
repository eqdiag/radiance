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
- [ ] color gradient
- [ ] rays from cam
- [ ] sphere interesection
- [ ] camera control
- [ ] add many spheres
- [ ] lambertian lighting
- [ ] antialiasing
- [ ] mirrors
- [ ] parallel
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

```

```


## Screenshots 