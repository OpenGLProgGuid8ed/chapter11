#ifndef DOUBLEWRITE_H
#define DOUBLEWRITE_H

#include <QtWidgets/QWidget>
#include "ui_doublewrite.h"
#include "LoadShaders.h"
#include "vermilion.h"
#include "vbm.h"
#include "vmath.h"
#include <qgl.h>
#include <QKeyEvent>
class doubleWrite : public QGLWidget
{
	Q_OBJECT

public:
	doubleWrite(QGLWidget *parent = 0);
	~doubleWrite();
	void initializeGL();
	void paintGL();
	void resizeGL(int w,int h);
	void timerEvent(QTimerEvent*);
	void keyPressEvent(QKeyEvent *e);

private:
	Ui::doubleWriteClass ui;
	//Member variables
	float aspect;
	GLuint  list_build_program;
    // Color palette buffer texture
    GLuint  image_palette_buffer;
    GLuint  image_palette_texture;

    GLuint  output_texture;
    GLuint  output_texture_clear_buffer;

    GLuint render_scene_prog;
    struct{
        GLint aspect;
        GLint time;
        GLint model_matrix;
        GLint view_matrix;
        GLint projection_matrix;
    } render_scene_uniforms;

    GLuint resolve_program;

    GLuint  quad_vbo;
    GLuint  quad_vao;

    GLint current_width;
    GLint current_height;

    VBObject object;
};

#endif // DOUBLEWRITE_H
