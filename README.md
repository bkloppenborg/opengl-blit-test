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

### NVidia 8600m GT (passed)

```
$ ./blit-test 

OpenGL information: 

 Driver Version String: "3.3.0 NVIDIA 331.38" 
 Current Context: QGLFormat(options QFlags(0x1|0x2|0x4|0x8|0x10|0x20|0x80|0x200|0x400) , plane  0 , depthBufferSize  24 , accumBufferSize  16 , stencilBufferSize  8 , redBufferSize  8 , greenBufferSize  8 , blueBufferSize  8 , alphaBufferSize  8 , samples  4 , swapInterval  0 , majorVersion  3 , minorVersion  3 , profile  1 )  

Widget information: 
 Size:  QSize(128, 128) 


GL_DRAW_FRAMEBUFFER details: 

GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE: GL_FRAMEBUFFER_DEFAULT
GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE: GL_UNSIGNED_NORMALIZED
GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING: GL_LINEAR
Number of bits for attachment components: 
 GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE: 8
 GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE: 8
 GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE: 8
 GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE: 8
 GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE: 0
 GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE: 0
Details about this GL_FRAMEBUFFER_DEFAULT
Width: 128
Height: 128
Number of samples: 4
Internal format: 0x1902
True size of color components: 
 GL_RENDERBUFFER_RED_SIZE: 0
 GL_RENDERBUFFER_GREEN_SIZE: 0
 GL_RENDERBUFFER_BLUE_SIZE: 0
 GL_RENDERBUFFER_ALPHA_SIZE: 0
 GL_RENDERBUFFER_DEPTH_SIZE: 24
 GL_RENDERBUFFER_STENCIL_SIZE: 0

GL_READ_FRAMEBUFFER details: 

GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE: GL_RENDERBUFFER
GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE: GL_FLOAT
GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING: GL_LINEAR
Number of bits for attachment components: 
 GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE: 32
 GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE: 32
 GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE: 32
 GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE: 32
 GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE: 0
 GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE: 0
Details on this GL_RENDERBUFFER buffer. See the following page
to look up hex enumeration values:
http://www.opengl.org/registry/oldspecs/enum.spec
Width: 128
Height: 128
Number of samples: 4
Internal format: 0x8814
True size of color components: 
 GL_RENDERBUFFER_RED_SIZE: 32
 GL_RENDERBUFFER_GREEN_SIZE: 32
 GL_RENDERBUFFER_BLUE_SIZE: 32
 GL_RENDERBUFFER_ALPHA_SIZE: 32
 GL_RENDERBUFFER_DEPTH_SIZE: 0
 GL_RENDERBUFFER_STENCIL_SIZE: 0

If you made it here, the blit test passed!

```
