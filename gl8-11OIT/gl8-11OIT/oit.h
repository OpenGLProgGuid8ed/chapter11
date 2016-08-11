#ifndef OIT_H
#define OIT_H

#include <QtWidgets/QWidget>
#include "ui_oit.h"
#include "LoadShaders.h"
#include "vbm.h"
#include "vmath.h"
#include <qgl.h>
#include <QKeyEvent>
class OIT : public QGLWidget
{
	Q_OBJECT

public:
	OIT(QGLWidget *parent = 0);
	~OIT();
	void initializeGL();
	void paintGL();
	void resizeGL(int w,int h);
	void timerEvent(QTimerEvent*);
	void keyPressEvent(QKeyEvent *e);

private:
	Ui::OITClass ui;
	// Member variables
    float aspect;

    // Program to construct the linked list (renders the transparent objects)
    GLuint  list_build_program;

    // Head pointer image and PBO for clearing it
    GLuint  head_pointer_texture;
    GLuint  head_pointer_clear_buffer;
    // Atomic counter buffer
    GLuint  atomic_counter_buffer;
    // Linked list buffer
    GLuint  linked_list_buffer;
    GLuint  linked_list_texture;

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

#endif // OIT_H
