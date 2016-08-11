#ifndef OVERDRAWCOUNT_H
#define OVERDRAWCOUNT_H

#include <QtWidgets/QWidget>
#include "ui_overdrawcount.h"
#include "LoadShaders.h"
#include "vbm.h"
#include "vmath.h"
#include <qgl.h>
#include <QKeyEvent>
class OverDrawCount : public QGLWidget
{
	Q_OBJECT

public:
	OverDrawCount(QGLWidget *parent = 0);
	~OverDrawCount();
	void initializeGL();
	void paintGL();
	void resizeGL(int w,int h);
	void timerEvent(QTimerEvent*);

private:
	Ui::OverDrawCountClass ui;
	// Member variables
    float aspect;

    // Program to construct the linked list (renders the transparent objects)
    GLuint  list_build_program;

    // Color palette buffer texture
    GLuint  image_palette_buffer;
    GLuint  image_palette_texture;

    // Output image and PBO for clearing it
    GLuint  overdraw_count_buffer;
    GLuint  overdraw_count_clear_buffer;

    // Program to render the scene
    GLuint render_scene_prog;
    struct
    {
        GLint aspect;
        GLint time;
        GLint model_matrix;
        GLint view_matrix;
        GLint projection_matrix;
    } render_scene_uniforms;

    // Program to resolve 
    GLuint resolve_program;

    // Full Screen Quad
    GLuint  quad_vbo;
    GLuint  quad_vao;

    GLint current_width;
    GLint current_height;

    VBObject object;
};

#endif // OVERDRAWCOUNT_H
