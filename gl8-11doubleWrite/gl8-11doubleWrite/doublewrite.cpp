#include "doublewrite.h"
#include <iostream>

#define MAX_FRAMEBUFFER_WIDTH 2048
#define MAX_FRAMEBUFFER_HEIGHT 2048
#define FRUSTUM_DEPTH 2000.0f

using namespace vmath;

doubleWrite::doubleWrite(QGLWidget *parent)
	: QGLWidget(parent)
{
	ui.setupUi(this);
	startTimer(100);
}

doubleWrite::~doubleWrite()
{
	glUseProgram(0);
    glDeleteProgram(render_scene_prog);
    glDeleteBuffers(1, &quad_vbo);
    glDeleteVertexArrays(1, &quad_vao);
}

void doubleWrite::initializeGL()
{
	render_scene_prog = -1;
	if(glewInit())
	{
		std::cout<<"Unable to Init GLEW"<<std::endl;
		exit(EXIT_FAILURE);
	}
	ShaderInfo scene_shaders[] =
    {
        { GL_VERTEX_SHADER,		"double_write.vs.glsl" },
        { GL_FRAGMENT_SHADER, "double_write.fs.glsl" },
        { GL_NONE ,						  NULL}
    };
	if (render_scene_prog != -1)
        glDeleteProgram(render_scene_prog);
    render_scene_prog = LoadShaders(scene_shaders);
	glLinkProgram(render_scene_prog);
	glUseProgram(render_scene_prog);
	render_scene_uniforms.model_matrix		= glGetUniformLocation(render_scene_prog, "model_matrix");
    render_scene_uniforms.view_matrix			= glGetUniformLocation(render_scene_prog, "view_matrix");
    render_scene_uniforms.projection_matrix = glGetUniformLocation(render_scene_prog, "projection_matrix");
    render_scene_uniforms.aspect					= glGetUniformLocation(render_scene_prog, "aspect");
    render_scene_uniforms.time						= glGetUniformLocation(render_scene_prog, "time");

    ShaderInfo resolve_shaders[] =
    {
        { GL_VERTEX_SHADER,			"blit.vs.glsl" },
        { GL_FRAGMENT_SHADER,		"blit.fs.glsl" },
        { GL_NONE,							 NULL}
    };
    resolve_program = LoadShaders(resolve_shaders);
	glLinkProgram(resolve_program);
	glUseProgram(resolve_program);
	 // Create palette texture
    glGenBuffers(1, &image_palette_buffer);
    glBindBuffer(GL_TEXTURE_BUFFER, image_palette_buffer);
    glBufferData(GL_TEXTURE_BUFFER, 256 * 4 * sizeof(float), NULL, GL_STATIC_DRAW);
    glGenTextures(1, &image_palette_texture);
    glBindTexture(GL_TEXTURE_BUFFER, image_palette_texture);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, image_palette_buffer);

    vmath::vec4 * data = (vmath::vec4 *)glMapBuffer(GL_TEXTURE_BUFFER, GL_WRITE_ONLY);
    for (int i = 0; i < 256; i++)
    {
        data[i] = vmath::vec4((float)i);
    }
    glUnmapBuffer(GL_TEXTURE_BUFFER);

    // Create head pointer texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &output_texture);
    glBindTexture(GL_TEXTURE_2D, output_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindImageTexture(0, output_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_RGBA32F);

    // Create buffer for clearing the head pointer texture
    glGenBuffers(1, &output_texture_clear_buffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, output_texture_clear_buffer);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint), NULL, GL_STATIC_DRAW);

    data = (vmath::vec4 *)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    memset(data, 0x00, MAX_FRAMEBUFFER_WIDTH * MAX_FRAMEBUFFER_HEIGHT * sizeof(GLuint));
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    // Create VAO containing quad for the final blit
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);

    static const GLfloat quad_verts[] =
    {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
    };

    glGenBuffers(1, &quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_verts), quad_verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glClearDepth(1.0f);

    object.LoadFromVBM("C:/Vermilion-Book/trunk/Code/media/unit_pipe.vbm", 0, 1, 2);

	glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void doubleWrite::paintGL()
{
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
	
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	float t = (float)((unsigned int)GetTickCount() & 0xFFFFF) / (float)0x3FFF;

    // Bind palette buffer
    glBindImageTexture(0, image_palette_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    // Clear output image
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, output_texture_clear_buffer);
    glBindTexture(GL_TEXTURE_2D, output_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, current_width, current_height, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Bind output image for read-write
    glBindImageTexture(1, output_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    glUseProgram(render_scene_prog);
    mat4 model_matrix = translate(0.0f, 0.0f, -15.0f) *
										rotate(t * 360.0f, 0.0f, 0.0f, 1.0f) *
										rotate(t * 435.0f, 0.0f, 1.0f, 0.0f) *
										rotate(t * 275.0f, 1.0f, 0.0f, 0.0f);
    mat4 view_matrix = mat4::identity();
    mat4 projection_matrix = frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 40.f);
    glUniformMatrix4fv(render_scene_uniforms.model_matrix, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(render_scene_uniforms.view_matrix, 1, GL_FALSE, view_matrix);
    glUniformMatrix4fv(render_scene_uniforms.projection_matrix, 1, GL_FALSE, projection_matrix);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    object.Render(0, 4*4*4);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glBindImageTexture(0, output_texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindVertexArray(quad_vao);
    glUseProgram(resolve_program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void doubleWrite::resizeGL(int w,int h)
{
	if(0 == h)
		h = 1;
    aspect = float(h) / float(w);
    glViewport(0, 0, w, h);
}

void doubleWrite::timerEvent(QTimerEvent*)
{
	updateGL();
}

void doubleWrite::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case 'r': initializeGL();
            break;
	default:
		break;
	}
}