# Partitioned Shadow Volume

This a replica of paper  
## Dependences

This project keeps few dependencies :

```
OpenGL 4.3+
GLFW 3
GLEW 1.10+
```

## Build

Download data:
```
https://drive.google.com/open?id=1bCQYOzu4BSKNwKwzprBcFBO6A_OuHtCf
```

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
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Execute

```
./psv [option]
option can be 1: PSV and 2: IPSV, IPSV add a method for select triangle from a heuristic function
```

## Results

![alt text](https://raw.githubusercontent.com/LuisErnestoColchado/PartitionedShadowVolume/master/images/video1.gif)

![alt text](https://raw.githubusercontent.com/LuisErnestoColchado/PartitionedShadowVolume/master/images/video2.gif)
