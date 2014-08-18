opengl-blit-test
================

Code to test glBlitBuffers on various OpenGL implementations.

## Prerequisites

* OpenGL
* QT 4.8
* CMake
* A C/C++ compiler (e.g. gcc)

## Building

```
cd build
cmake ..
make
```


## Sample Results

### AMD Radeon R9 280x on Ubuntu Linux 14.04.1 (passes)

```
$ ./blit-test 
GL Context is valid:  true 
Multisample AA is enabled

OpenGL information: 

 Driver Version String: "3.2.12874 Core Profile Context 14.10.1006.1001" 
 Current Context: QGLFormat(options QFlags(0x1|0x2|0x4|0x8|0x20|0x80|0x200|0x400) , plane  0 , depthBufferSize  24 , accumBufferSize  -1 , stencilBufferSize  8 , redBufferSize  8 , greenBufferSize  8 , blueBufferSize  8 , alphaBufferSize  8 , samples  4 , swapInterval  0 , majorVersion  3 , minorVersion  2 , profile  1 )  

Widget information: 
 Size:  QSize(128, 128) 

Is buffer valid:  true 
Is bound:  false 
BLIT successful.
```

### Macbook Pro with Intel Integrated Graphics

```
$ ./blit-test 

GL Context is valid:  true 
Multisample AA is enabled

OpenGL information: 

 Driver Version String: "2.1 INTEL-8.28.30" 
 Current Context: QGLFormat(options QFlags(0x1|0x2|0x4|0x8|0x20|0x80|0x200|0x400) , plane  0 , depthBufferSize  24 , accumBufferSize  -1 , stencilBufferSize  8 , redBufferSize  -1 , greenBufferSize  -1 , blueBufferSize  -1 , alphaBufferSize  8 , samples  4 , swapInterval  1 , majorVersion  1 , minorVersion  0 , profile  0 )  

Widget information: 
 Size:  QSize(128, 128) 

Is buffer valid:  true 
Is bound:  false 
BLIT successful.
```

### Intel Core i7-4770k on Ubuntu Linux 14.04.1 (Intel HD Graphics 4000 using Mesa 10.1.3) (fails)

Confirm that the driver is loaded and direct rendering is enabled:

```
$ lspci | grep VGA
00:02.0 VGA compatible controller: Intel Corporation Xeon E3-1200 v3/4th Gen Core Processor Integrated Graphics Controller (rev 06)
```

```
$ cat /proc/cpuinfo | grep "model name"
model name	: Intel(R) Core(TM) i7-4770K CPU @ 3.50GHz
```

```
$ glxgears
Running synchronized to the vertical refresh.  The framerate should be
approximately the same as the monitor refresh rate.
305 frames in 5.0 seconds = 60.975 FPS
300 frames in 5.0 seconds = 59.998 FPS
...
```

```
$ glxinfo | grep rendering
direct rendering: Yes
```

Check that OpenGL exists and supports the necessary extensions:

```
$ glxinfo | grep version
server glx version string: 1.4
client glx version string: 1.4
GLX version: 1.4
OpenGL core profile version string: 3.3 (Core Profile) Mesa 10.1.3
OpenGL core profile shading language version string: 3.30
OpenGL version string: 3.0 Mesa 10.1.3
OpenGL shading language version string: 1.30

$ glxinfo | grep texture_float
    GL_ARB_texture_float, GL_ARB_texture_gather, 
    GL_ATI_texture_float, GL_EXT_abgr, GL_EXT_blend_equation_separate, 
    GL_ARB_texture_env_dot3, GL_ARB_texture_float, GL_ARB_texture_gather, 
    GL_ATI_texture_env_combine3, GL_ATI_texture_float, GL_EXT_abgr,

```

Test program fails when a new Framebuffer is created. This happens regardless of the requested
format (e.g. `GL_RGBA`, `GL_RGBA8`, `GL_RGBA32f` or even with the default constructor)

```
$ ./blit-test 
GL Context is valid:  true 
Multisample AA is enabled

OpenGL information: 

 Driver Version String: "3.3 (Core Profile) Mesa 10.1.3" 
 Current Context: QGLFormat(options QFlags(0x1|0x2|0x4|0x8|0x20|0x80|0x200|0x400) , plane  0 , depthBufferSize  24 , accumBufferSize  -1 , stencilBufferSize  8 , redBufferSize  8 , greenBufferSize  8 , blueBufferSize  8 , alphaBufferSize  8 , samples  4 , swapInterval  0 , majorVersion  3 , minorVersion  2 , profile  1 )  

Widget information: 
 Size:  QSize(128, 128) 

Is buffer valid:  true 
Is bound:  false 

Error: Failed to create buffer
OpenGL error code: 1282
OpenGL error description: invalid operation
Location : /home/bkloppenborg/workspace/opengl-blit-test/src/main.cpp:335
terminate called after throwing an instance of 'std::runtime_error'
  what():  OpenGL error detected.
Aborted (core dumped)
```
