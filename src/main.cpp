 #include <iostream>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QApplication>
#include <QGLFormat>
#include <QGLFramebufferObjectFormat>
#include <QString>
#include <QDebug>
#include <string>
#include <stdexcept>

// Standard OpenGL Headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#endif

using namespace std;

#define OPENGL_SUCCESS 0
#define OPENGL_FAILURE 1

static void printGLErrorDetail(GLenum error_code, std::string message, bool critical_error);

/// Checks for the status of an OpenGL error and generates an error message is one is
/// detected.
static int checkGLError(GLenum input, GLenum reference, std::string message, bool critical_error = true)
{
	if(input != reference)
	{
		// Look up the OpenGL error message
		printGLErrorDetail(input, message, critical_error);
		return OPENGL_FAILURE;
	}

	return OPENGL_SUCCESS;
}

// Quickly clear errors, don't care what they state.
void clearGLErrors()
{
    while(glGetError() != GL_NO_ERROR)
    {
        continue;
    }
}

static void printGLErrorDetail(GLenum error_code, std::string message, bool critical_error)
{
	const GLubyte * error_string = gluErrorString(error_code);

	if(critical_error)
		std::cout << std::endl << "Error: ";
	else
		std::cout << std::endl << "Warning: ";

	std::cout << message << std::endl;
	std::cout << "OpenGL error code: " << error_code << std::endl;;
	std::cout << "OpenGL error description: " << error_string << std::endl;
}

/// Macro which checks and marks the source of an OpenGL error and throws a runtime_error if
#define CHECK_OPENGL_STATUS_ERROR(actual, msg) \
    if(checkGLError(actual, GL_NO_ERROR, msg)) \
    { \
        std::cout << "Location : " << __FILE__ << ":" << __LINE__<< std::endl; \
        throw runtime_error("OpenGL error detected."); \
    }

void CreateGLMultisampleRenderBuffer(unsigned int width, unsigned int height, unsigned int samples,
		GLuint & FBO, GLuint & FBO_texture, GLuint & FBO_depth)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenRenderbuffers(1, &FBO_texture);
	glBindRenderbuffer(GL_RENDERBUFFER, FBO_texture);
	// Create a 2D multisample texture
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA32F, GLsizei(width), GLsizei(height));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, FBO_texture);

	glGenRenderbuffers(1, &FBO_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, FBO_depth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, GLsizei(width), GLsizei(height));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, FBO_depth);

	CHECK_OPENGL_STATUS_ERROR(glGetError(), "Failed to create renderbuffer storage");

    // Get the status of the OpenGL framebuffer
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    // Check the status. If it is not GL_FRAMEBUFFER_COMPLETE generate an error message
    // and throw a runtime error.
    if(checkGLError(status, GL_FRAMEBUFFER_COMPLETE, "Could not create multisample render buffer."))
    {
        cout << "Location : " << __FILE__ << ":" << __LINE__<< std::endl; \
        throw runtime_error("OpenGL error detected."); \
    }

    // All done, bind back to the default framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    CHECK_OPENGL_STATUS_ERROR(glGetError(), "Failed to bind back to default buffer");
}

// Dump details on a renderbuffer
void printRenderbufferInformation()
{
    // OpenGL Renderbuffer. See function call information here:
    // http://www.opengl.org/wiki/GLAPI/glGetRenderbufferParameter
    GLint param = 0;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &param);
    cout << "Width: " << param << endl;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &param);
    cout << "Height: " << param << endl;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_SAMPLES, &param);
    cout << "Number of samples: " << param << endl;

    // Ideally we would convert this over to a string, but that's just too much
    // work for this question.
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &param);
    cout << "Internal format: " << "0x" << std::noshowbase << std::hex << param << std::dec << endl;

    cout << "True size of color components: " << endl;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_RED_SIZE, &param);
    cout << " GL_RENDERBUFFER_RED_SIZE: " << param << endl;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_GREEN_SIZE, &param);
    cout << " GL_RENDERBUFFER_GREEN_SIZE: " << param << endl;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_BLUE_SIZE, &param);
    cout << " GL_RENDERBUFFER_BLUE_SIZE: " << param << endl;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_ALPHA_SIZE, &param);
    cout << " GL_RENDERBUFFER_ALPHA_SIZE: " << param << endl;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &param);
    cout << " GL_RENDERBUFFER_DEPTH_SIZE: " << param << endl;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE, &param);
    cout << " GL_RENDERBUFFER_STENCIL_SIZE: " << param << endl;
}

// Dump as many details about a framebuffer's attachment as possible.
void printGLFramebufferBufferInfo(GLenum target, GLenum attachment)
{
    string temp;
    string attachment_name;
    
    // Get a friendly string for the attachment
    if(attachment == GL_FRONT_LEFT) attachment_name = "GL_FRONT_LEFT";
    else if(attachment == GL_FRONT_RIGHT) attachment_name = "GL_FRONT_RIGHT";
    else if(attachment == GL_BACK_LEFT) attachment_name = "GL_BACK_LEFT";
    else if(attachment == GL_BACK_RIGHT) attachment_name = "GL_BACK_RIGHT";
    else if(attachment == GL_DEPTH) attachment_name = "GL_DEPTH";
    else if(attachment == GL_STENCIL) attachment_name = "GL_STENCIL";
    else if(attachment == GL_COLOR_ATTACHMENT0) attachment_name = "GL_COLOR_ATTACHMENT0";
    else if(attachment == GL_DEPTH_ATTACHMENT) attachment_name = "GL_DEPTH_ATTACHMENT";
    else if(attachment == GL_STENCIL_ATTACHMENT) attachment_name = "GL_STENCIL_ATTACHMENT";
    else if(attachment == GL_DEPTH_STENCIL_ATTACHMENT) attachment_name = "GL_DEPTH_STENCIL_ATTACHMENT";  
    else attachment_name = "Unknown type of attachment or attachment type not implemented.";    
    
    // Determine the attachment type
	GLint fbo_attachment_object_type;
	glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &fbo_attachment_object_type);
	if(fbo_attachment_object_type == GL_FRAMEBUFFER_DEFAULT) temp = "GL_FRAMEBUFFER_DEFAULT";
	else if(fbo_attachment_object_type == GL_TEXTURE) temp = "GL_TEXTURE";
	else if(fbo_attachment_object_type == GL_RENDERBUFFER) temp = "GL_RENDERBUFFER";
	else if(fbo_attachment_object_type == GL_NONE) temp = "GL_NONE";
	else if(glGetError() != GL_NO_ERROR) temp = "OpenGL Error while querying GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE";
	else temp = "Unknown type";
	cout << "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE: " << temp  << endl;

    // Determine the type of the components
    GLint component_type;
    glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE, &component_type);
    if(component_type == GL_FLOAT) temp = "GL_FLOAT";
    else if(component_type == GL_INT) temp = "GL_INT";
    else if(component_type == GL_UNSIGNED_INT) temp = "GL_UNSIGNED_INT";
    else if(component_type == GL_SIGNED_NORMALIZED) temp = "GL_SIGNED_NORMALIZED";
    else if(component_type == GL_UNSIGNED_NORMALIZED) temp = "GL_UNSIGNED_NORMALIZED";
    else if(glGetError() != GL_NO_ERROR) temp = "OpenGL Error while querying GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE";
    else temp = "Unknown type";
    cout << "GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE: " << temp << endl;

	// Determine the color encoding
	GLint color_encoding;
	glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &color_encoding);
	if(color_encoding == GL_LINEAR) temp = "GL_LINEAR";
	else if(color_encoding == GL_SRGB) temp = "GL_SRGB";
	else temp = "Unknown encoding format";
	cout << "GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING: " << temp << endl;

	// Get the size, in bits, for the attachment's components
	GLint red_size, green_size, blue_size, alpha_size, depth_size, stencil_size;
	glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &red_size);
	glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &green_size);
	glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &blue_size);
	glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alpha_size);
	glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_size);
	glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_size);
	cout << "Number of bits for attachment components: " << endl;
	cout << " GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE: " << red_size << endl;
	cout << " GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE: " << green_size << endl;
	cout << " GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE: " << blue_size << endl;
	cout << " GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE: " << alpha_size << endl;
	cout << " GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE: " << depth_size << endl;
	cout << " GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE: " << stencil_size << endl;

    // now print details that are specific to render/texture buffers    
	if(fbo_attachment_object_type == GL_FRAMEBUFFER_DEFAULT)
    {
        cout << "Details about this GL_FRAMEBUFFER_DEFAULT" << endl;
		GLint name;
		glBindRenderbuffer(GL_RENDERBUFFER, attachment);
		printRenderbufferInformation();
	}
    else if(fbo_attachment_object_type == GL_RENDERBUFFER)
    {
        cout << "Details on this GL_RENDERBUFFER buffer. See the following page" << endl;
        cout << "to look up hex enumeration values:" << endl;
        cout << "http://www.opengl.org/registry/oldspecs/enum.spec" << endl;
      
        // determine which attachment holds the data and bind to it
		GLint name;
		glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &name);
		glBindRenderbuffer(GL_RENDERBUFFER, name);

        printRenderbufferInformation();
    }
    else if(fbo_attachment_object_type == GL_TEXTURE)
    {
        // OpenGL Texturebuffer. See details about these functions here:
        cout << "Details on this GL_TEXTURE buffer. See the following page" << endl;
        cout << "to look up hex enumeration values:" << endl;
        cout << "http://www.opengl.org/registry/oldspecs/enum.spec" << endl;

		GLint name;
		GLint mipmap_level;
		GLint layer;
		GLint is_layered;
		glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &name);
		cout << "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME: " << name << endl;
		glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL, &mipmap_level);
		cout << "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL: " << mipmap_level << endl;
		// Not implementing GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE query... just too much work
		glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER, &layer);
		cout << "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER: " << layer << endl;
		glGetFramebufferAttachmentParameteriv(target, attachment, GL_FRAMEBUFFER_ATTACHMENT_LAYERED, &is_layered);
		if(is_layered)
			cout << "Texture is layered" << endl;
		else
			cout << "Texure is NOT layered" << endl;

	    GLint param;
	    glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_WIDTH, &param);
	    cout << "Width: " << param << endl;
	    glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_HEIGHT, &param);
	    cout << "Height: " << param << endl;
	    glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_DEPTH, &param);
	    cout << "Depth: " << param << endl;
	    glGetTexLevelParameteriv(target, mipmap_level, GL_TEXTURE_INTERNAL_FORMAT, &param);
	    cout << "Internal Format: " << "0x" << std::noshowbase << std::hex << param << endl;
	
	}

    clearGLErrors();
}


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Select an OpenGL 3.3 core profile context
    QGLFormat gl_core_format;
    gl_core_format.setVersion(3, 3);
    gl_core_format.setProfile(QGLFormat::CoreProfile);
    gl_core_format.setAlpha(true);
    gl_core_format.setDoubleBuffer(true);
    gl_core_format.setSampleBuffers(false);
    gl_core_format.setSamples(4);
    QGLFormat::setDefaultFormat(gl_core_format);

    QGLWidget gl;

    // after construction, you should have a valid context
    // however, it is NOT made current unless show() or
    // similar functions are called
    if(!gl.isValid ())
    {
        std::cout << "ERROR: No GL context!" << std::endl;
        return -1;
    }

    unsigned int width = 128;
    unsigned int height = 128;
    unsigned int depth = 1;
    unsigned int samples = 4;
    GLuint FBO; 
    GLuint FBO_texture;
    GLuint FBO_depth;

    // do some off-screen rendering, the widget has never been made visible
    gl.makeCurrent (); // ABSOLUTELY CRUCIAL!
    
    QString versionString(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    qDebug() << endl << "OpenGL information:" << endl;
    qDebug() << " Driver Version String:" << versionString;
    qDebug() << " Current Context:" << gl.format();
    
    // resize the default buffer to match
    gl.resize(width, height);
    qDebug() << endl << "Widget information:";
    qDebug() << " Size: " << gl.size() << endl;

    // create the off-screen storage buffer
    GLuint FBO_storage;
    GLuint FBO_storage_texture;
    CreateGLMultisampleRenderBuffer(width, height, samples, FBO, FBO_texture, FBO_depth);
    
    // Print out some information on the two framebuffers.
    // First the default buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    qDebug() << endl << "GL_DRAW_FRAMEBUFFER details:" << endl;
    printGLFramebufferBufferInfo(GL_FRAMEBUFFER, GL_BACK_LEFT);
    // now the custom FBO we created above.
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    qDebug() << endl << "GL_READ_FRAMEBUFFER details:" << endl;
    printGLFramebufferBufferInfo(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0);

    // Set the draw buffer to be the default buffer:
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    //glDrawBuffer(GL_BACK); // is this necessary?
    // Set the read buffer to be the custom buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
    //glReadBuffer(GL_COLOR_ATTACHMENT0); // is this necessary?
    
    // Check that the binds worked
    CHECK_OPENGL_STATUS_ERROR(glGetError(), "Failed to bind input/output buffers");
    // execute the blit
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    CHECK_OPENGL_STATUS_ERROR(glGetError(), "Failed to blit buffers");
 
    return 0;
}
