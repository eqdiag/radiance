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
- [ ]



### P3
- [ ] stratified sampling
- [ ] multithreaded


### Bonus
- [ ] Non-uniform diffuse
- [ ] Types of texture samplers
- [ ] stratification
- [ ] sample lights based on surface area frac
- [ ] efficient sphere sampling (via "cone sampling")
- [ ] better triangle sampling
- [ ] different bvh splitting methods
- [ ] instancing (test with spheres, meshes)
    - [ ] hit list -> bvh node
- [ ] remove ply dependency


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

## Screenshots