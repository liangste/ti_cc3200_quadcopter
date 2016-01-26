#ifndef VISUALISER_H
#define VISUALISER_H

#include "glm/glm.hpp"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QString>

using namespace std;

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class Visualiser : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  Visualiser(QWidget *parent = 0);
  ~Visualiser();

  QSize minimumSizeHint() const Q_DECL_OVERRIDE;
  QSize sizeHint() const Q_DECL_OVERRIDE;
  bool LoadModel(QString);

public slots:
  void cleanup();
  void setXRotation(int angle);
  void setYRotation(int angle);
  void setZRotation(int angle);
signals:
  void LogUartConsole(QString);
protected:
  void initializeGL() Q_DECL_OVERRIDE;
  void resizeGL(int w, int h) Q_DECL_OVERRIDE;
  void paintGL() Q_DECL_OVERRIDE;
  void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
  void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
private:

  void updateMaxMinVertices(glm::vec3);

  vector<unsigned int> m_vIndices, m_nIndices;
  vector<glm::vec3> m_vertices;
  vector<glm::vec3> m_normals;
  vector<glm::vec3> m_faces;

  glm::vec3 m_maxVertex, m_minVertex;

  QOpenGLShaderProgram *m_program;
  int m_projMatrixLoc;
  int m_mvMatrixLoc;
  int m_normalMatrixLoc;
  int m_lightPosLoc;
  QMatrix4x4 m_projMatrix;
  QMatrix4x4 m_viewMatrix;
  QMatrix4x4 m_modelMatrix;
  int m_xRot;
  int m_yRot;
  int m_zRot;
  GLuint m_vVbo;
  GLuint m_iVbo;
  bool m_init;
  GLuint VertexArrayID;

  // for opengl-tutorials
  QOpenGLVertexArrayObject m_vao;
  GLuint vertexbuffer;
  GLuint normalbuffer;
  GLuint elementbuffer;

  QPoint m_lastPos;
};

#endif // VISUALISER_H
