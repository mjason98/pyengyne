# My Engyne

This engyne, is intended to repleace old engyne based on opengl

## Build, Run and Release compilation
First clone the repository
```shell
git clone --recursive https://github.com/mjason98/pyengyne.git
```
### Build bgfx
For more build options rather than Linux, check this.
In the root repository folder:
```shell
cd bgfx
make linux-release64
```
### Build glfw
In the root repository folder:
```
cd glfw
cmake . -B build
cd build
make 
```
### Build reactphisics3d
In the root repository folder:
```
```

### Build the engyne
In the root repository folder:
```shell
make 
```
then, build the shaders:
```shell
make shaders
```

### Run the engyne
In the root repository folder:
```shell
make run
```

### Build a release version
In the root repository folder:
```shell
```