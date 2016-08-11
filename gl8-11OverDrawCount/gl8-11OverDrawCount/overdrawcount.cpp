#include "overdrawcount.h"
#include <iostream>

#define MAX_FRAMEBUFFER_WIDTH 1024
#define MAX_FRAMEBUFFER_HEIGHT 1024
#define FRUSTUM_DEPTH 2000.0f

using namespace vmath;
OverDrawCount::OverDrawCount(QGLWidget *parent)
	: QGLWidget(parent)
{
	ui.setupUi(this);
	startTimer(100);
}

OverDrawCount::~OverDrawCount()
{
	glUseProgram(0);
    glDeleteProgram(render_scene_prog);
    glDeleteBuffers(1, &quad_vbo);
    glDeleteVertexArrays(1, &quad_vao);
}

void OverDrawCount::initializeGL()
{
	if(glewInit())
	{
		std::cout<<"Unable to Init GLEW"<<std::endl;
		exit(EXIT_FAILURE);
	}
	render_scene_prog = -1;
	  ShaderInfo scene_shaders[] =
    {
        { GL_VERTEX_SHADER, "overdraw_count.vs.glsl" },
        { GL_FRAGMENT_SHADER, "overdraw_count.fs.glsl" },
        { GL_NONE }
    };

    if (render_scene_prog != -1)
        glDeleteProgram(render_scene_prog);

    render_scene_prog = LoadShaders(scene_shaders);
	glLinkProgram(render_scene_prog);
	glUseProgram(render_scene_prog);
    render_scene_uniforms.model_matrix = glGetUniformLocation(render_scene_prog, "model_matrix");
    render_scene_uniforms.view_matrix = glGetUniformLocation(render_scene_prog, "view_matrix");
    render_scene_uniforms.projection_matrix = glGetUniformLocation(render_scene_prog, "projection_matrix");
    render_scene_uniforms.aspect = glGetUniformLocation(render_scene_prog, "aspect");
    render_scene_uniforms.time = glGetUniformLocation(render_scene_prog, "time");

    ShaderInfo resolve_shaders[] =
    {
        { GL_VERTEX_SHADER, "blit.vs.glsl" },
        { GL_FRAGMENT_SHADER, "blit.fs.glsl" },
        { GL_NONE }
    };

    resolve_program = LoadShaders(resolve_shaders);
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

    // Create overdraw counter texture
    glGenTextures(1, &overdraw_count_buffer);
    glBindTexture(GL_TEXTURE_2D, overdraw_count_buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, MAX_FRAMEBUFFER_WIDTH, MAX_FRAMEBUFFER_HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create buffer for clearing the head pointer texture
    glGenBuffers(1, &overdraw_count_clear_buffer);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, overdraw_count_clear_buffer);
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

    object.LoadFromVBM("C:/Vermilion-Book/trunk/Code/media/torus.vbm", 0, 1, 2);
	//Draw the object
    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void OverDrawCount::paintGL()
{
	float t = (float)((unsigned int )GetTickCount() & 0xFFFFF) / (float)0x3FFF;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Clear output image
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, overdraw_count_clear_buffer);
    glBindTexture(GL_TEXTURE_2D, overdraw_count_buffer);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, current_width, current_height, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Bind output image for read-write
    glBindImageTexture(0, overdraw_count_buffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

    // Render
    glUseProgram(render_scene_prog);
	mat4 model_matrix			=  translate(0.0f, 0.0f, -20.0f) *
													rotate(t * 360.0f, 0.0f, 0.0f, 1.0f) *
													rotate(t * 435.0f, 0.0f, 1.0f, 0.0f) *
													rotate(t * 275.0f, 1.0f, 0.0f, 0.0f);
	mat4 view_matrix				= mat4::identity();
    mat4 projection_matrix		= frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 40.f);

    glUniformMatrix4fv(render_scene_uniforms.model_matrix, 1, GL_FALSE, model_matrix);
    glUniformMatrix4fv(render_scene_uniforms.view_matrix, 1, GL_FALSE, view_matrix);
    glUniformMatrix4fv(render_scene_uniforms.projection_matrix, 1, GL_FALSE, projection_matrix);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    object.Render(0, 8 * 8 * 8);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glBindVertexArray(quad_vao);
    glUseProgram(resolve_program);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void OverDrawCount::resizeGL(int w,int h)
{
	if(0 == h)
		h =1;
	current_width = w;
    current_height = h;
	aspect = float(h) /w;
    glViewport(0, 0, current_width, current_height);
}

void OverDrawCount::timerEvent(QTimerEvent*)
{
	updateGL();
}