# Partitioned Shadow Volume

## Dependences

This project keeps few dependencies :

```
OpenGL 4.3+ [required]
GLFW 3 [required]
GLEW 1.10+ [optional]
Python 2.7+ [if not using GLEW]
```

## Build
Copy data folder in root.

```
.
├── ...
└── repository                   
│    ├── data
│    ├── common
│    ├── shaders
│    ├── ...              
└── ...
```

Enter your terminal in the project root directory and type the following :
```
$ mkdir build; cd build
$ cmake ..
$ make
```

## Execute

```
./psv [option]
option can be 1: PSV and 2: IPSV, IPSV add a method for select triangle from a heuristic function
```

## Results

![alt text](https://github.com/LuisErnestoColchado/pmvs_ba.git/images/video1.gif)

![alt text](https://github.com/LuisErnestoColchado/pmvs_ba.git/images/video2.gif)
