#include "visualiser.h"
#include "glm/glm.hpp"

#include <QMouseEvent>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
#include <QFileDialog>
#include <QOpenGLShaderProgram>
#include <QDebug>
#include <stdio.h>
#include <vector>
#include <GL/glu.h>
#include <QCoreApplication>
#include <math.h>
#include <QDebug>
#include <limits>

using namespace std;

static const char *vertexShaderSource =
  "attribute vec4 vertex;\n"
  "attribute vec3 normal;\n"
  "varying vec3 vert;\n"
  "varying vec3 vertNormal;\n"
  "uniform mat4 projMatrix;\n"
  "uniform mat4 mvMatrix;\n"
  "uniform mat3 normalMatrix;\n"
  "void main() {\n"
  "   vert = vertex.xyz;\n"
  "   vertNormal = normalMatrix * normal;\n"
  "   gl_Position = projMatrix * mvMatrix * vertex;\n"
  "}\n";

static const char *fragmentShaderSource =
  "varying highp vec3 vert;\n"
  "varying highp vec3 vertNormal;\n"
  "uniform highp vec3 lightPos;\n"
  "void main() {\n"
  "   highp vec3 L = normalize(lightPos - vert);\n"
  "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
  "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
  "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
  "   gl_FragColor = vec4(col, 1.0);\n"
  "}\n";

Visualiser::Visualiser(QWidget *parent)
  : m_vertices(vector<glm::vec3>())
  , m_normals(vector<glm::vec3>())
  , m_faces(vector<glm::vec3>())
  , QOpenGLWidget(parent)
  , m_xRot(0)
  , m_yRot(0)
  , m_zRot(0)
  , m_init(false)
{
}

Visualiser::~Visualiser() {
  cleanup();
}

QSize Visualiser::minimumSizeHint() const
{
  return QSize(50, 50);
}

QSize Visualiser::sizeHint() const
{
  return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
  while (angle < 0)
    angle += 360 * 16;
  while (angle > 360 * 16)
    angle -= 360 * 16;
}

void Visualiser::setXRotation(int angle)
{
  qNormalizeAngle(angle);
  if (angle != m_xRot) {
    m_xRot = angle;
    update();
  }
}

void Visualiser::setYRotation(int angle)
{
  qNormalizeAngle(angle);
  if (angle != m_yRot) {
    m_yRot = angle;
    update();
  }
}

void Visualiser::setZRotation(int angle)
{
  qNormalizeAngle(angle);
  if (angle != m_zRot) {
    m_zRot = angle;
    update();
  }
}

void Visualiser::cleanup()
{
  m_vIndices.clear();
  m_nIndices.clear();
  m_vertices.clear();
  m_normals.clear();
  m_faces.clear();
  glDeleteBuffers(1, &m_vVbo);
  glDeleteBuffers(1, &m_iVbo);
}

void Visualiser::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0, 0, 0, 1);

  m_program = new QOpenGLShaderProgram();
  m_program->addShaderFromSourceCode(
    QOpenGLShader::Vertex,
    vertexShaderSource
  );
  m_program->addShaderFromSourceCode(
    QOpenGLShader::Fragment,
    fragmentShaderSource
  );
  m_program->bindAttributeLocation("vertex", 0);
  m_program->bindAttributeLocation("normal", 1);
  m_program->link();

  m_program->bind();
  m_projMatrixLoc = m_program->uniformLocation("projMatrix");
  m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
  m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
  m_lightPosLoc = m_program->uniformLocation("lightPos");

  //m_vao.create();
  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

  // Light position is fixed.
  m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 100, 0));

  m_program->release();
}

void Visualiser::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  m_modelMatrix.setToIdentity();

  m_modelMatrix.rotate(m_xRot / 16.0f, 1, 0, 0);
  m_modelMatrix.rotate(m_yRot / 16.0f, 0, 1, 0);
  m_modelMatrix.rotate(m_zRot / 16.0f, 0, 0, 1);

  m_modelMatrix.translate(
    -(m_maxVertex.x - m_minVertex.x)/2,
    -(m_maxVertex.y - m_minVertex.y)/2,
    -(m_maxVertex.z - m_minVertex.z)/2
  );

  QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

  qreal z_offset = m_maxVertex.z * 1.5;
  m_viewMatrix.setToIdentity();
  m_viewMatrix.lookAt(
    QVector3D(0.0, 0.0, z_offset),
    QVector3D(0.0, 0.0, 0.0),
    QVector3D(0.0, 1.0, 0.0)
  );

  m_program->bind();
  m_program->setUniformValue(m_projMatrixLoc, m_projMatrix);
  m_program->setUniformValue(m_mvMatrixLoc, m_viewMatrix * m_modelMatrix);
  QMatrix3x3 normalMatrix = m_modelMatrix.normalMatrix();
  m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);

  if (m_vertices.size() == 0) {
    m_vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    m_vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
    m_vertices.push_back(glm::vec3(1.0f,  0.0f, 0.0f));
    m_vertices.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
    m_vertices.push_back(glm::vec3(-1.0f,  0.0f, 0.0f));

    m_normals.push_back(glm::vec3(0.0f,  0.0f, 1.0f));
    m_normals.push_back(glm::vec3(0.0f,  0.0f, 1.0f));
    m_normals.push_back(glm::vec3(0.0f,  0.0f, 1.0f));
    m_normals.push_back(glm::vec3(0.0f,  0.0f, 1.0f));
    m_normals.push_back(glm::vec3(0.0f,  0.0f, 1.0f));

    m_vIndices.push_back(0);
    m_vIndices.push_back(1);
    m_vIndices.push_back(2);
    m_vIndices.push_back(0);
    m_vIndices.push_back(3);
    m_vIndices.push_back(4);
  }

  // vertices buffer
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER,
    m_vertices.size() * sizeof(glm::vec3),
    &m_vertices[0],
    GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

  glVertexAttribPointer(
    0,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0
  );

  // normals buffer
  glGenBuffers(1, &normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(
    GL_ARRAY_BUFFER,
    m_normals.size() * sizeof(glm::vec3),
    &m_normals[0],
    GL_STATIC_DRAW
  );

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glVertexAttribPointer(
    1,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0
  );

  glGenBuffers(1, &elementbuffer);
  glBindBuffer(
    GL_ELEMENT_ARRAY_BUFFER,
    elementbuffer
  );
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    m_vIndices.size() * sizeof(unsigned int),
    &m_vIndices[0],
    GL_STATIC_DRAW
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

  glPolygonMode(GL_FRONT, GL_LINE);
  glDrawElements(
    GL_TRIANGLES,
    (GLsizei) m_vIndices.size(),
    GL_UNSIGNED_INT,
    (void*)0
  );

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  m_program->release();
};

void Visualiser::resizeGL(int w, int h) {
  m_projMatrix.setToIdentity();
  m_projMatrix.perspective(45.0f, GLfloat(w) / h, 0.01f, 300.0f);
}

void Visualiser::updateMaxMinVertices(glm::vec3 v) {
  if (v.x > m_maxVertex.x)
    m_maxVertex.x = v.x;
  else if (v.x < m_minVertex.x)
    m_minVertex.x = v.x;

  if (v.y > m_maxVertex.y)
    m_maxVertex.y = v.y;
  else if (v.y < m_minVertex.y)
    m_minVertex.y = v.y;

  if (v.z > m_maxVertex.z)
    m_maxVertex.z = v.z;
  else if (v.z < m_minVertex.z)
    m_minVertex.z = v.z;
}

bool Visualiser::LoadModel(QString modelFile) {
  emit LogUartConsole("Loading " + modelFile);
  initializeOpenGLFunctions();

  if (m_vIndices.size() > 0) {
    cleanup();
  }

  m_maxVertex = glm::vec3(
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::min(),
    std::numeric_limits<float>::min()
  );
  m_minVertex = glm::vec3(
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::max(),
    std::numeric_limits<float>::max()
  );

  QFile file(modelFile);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(
      0,
      "error, unale to open file",
      file.errorString()
    );
    return false;
  }
  //QMessageBox::information(0, "Found file", modelFile);
  FILE *fh = fopen(modelFile.toStdString().c_str(), "r");
  if (fh == NULL) {
    QMessageBox::information(
      0,
      "error, unable to open file for reading",
      file.errorString()
    );
    return false;
  }

  /* Here we load obj file generated by Meshlab, this file should at least
   * contain vertices (v), normals (vn) and faces (f)
   */
  while (1) {
    char lineHeader[128];
    int res = fscanf(fh, "%s", lineHeader);
    if (res == EOF)
      break;

    if (strcmp(lineHeader, "v") == 0) {
      glm::vec3 vertex;
      fscanf(fh, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      updateMaxMinVertices(vertex);
      m_vertices.push_back(vertex);
    } else if (strcmp(lineHeader, "vn") == 0) {
      glm::vec3 normal;
      fscanf(fh, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      m_normals.push_back(normal);
    } else if (strcmp(lineHeader, "f") == 0) {
      unsigned int vIndex[3], nIndex[3];
      int matches = fscanf(
        fh,
        "%d//%d %d//%d %d//%d\n",
        &vIndex[0], &nIndex[0],
        &vIndex[1], &nIndex[1],
        &vIndex[2], &nIndex[2]
      );
      if (matches != 6)
        return false;
      m_vIndices.push_back(vIndex[0] - 1);
      m_vIndices.push_back(vIndex[1] - 1);
      m_vIndices.push_back(vIndex[2] - 1);
      m_nIndices.push_back(nIndex[0] - 1);
      m_nIndices.push_back(nIndex[1]);
      m_nIndices.push_back(nIndex[2]);
      m_nIndices.push_back(nIndex[0]);
    }
  }

  emit LogUartConsole("Loaded " + QString::number(m_vIndices.size()) + " vertex indices");
  emit LogUartConsole("Loaded " + QString::number(m_nIndices.size()) + " normal indices");
  emit LogUartConsole("Loaded " + QString::number(m_vertices.size()) + " vertices");
  emit LogUartConsole("Loaded " + QString::number(m_normals.size()) + " normals");

  m_xRot = m_yRot = m_zRot = 0;

  update();
  m_init = true;

  return true;
}

void Visualiser::mousePressEvent(QMouseEvent *event) {
  m_lastPos = event->pos();
}

void Visualiser::mouseMoveEvent(QMouseEvent *event) {
  int dx = event->x() - m_lastPos.x();
  int dy = event->y() - m_lastPos.y();

  if (event->buttons() & Qt::LeftButton) {
    setXRotation(m_xRot + 8 * dy);
    setYRotation(m_yRot + 8 * dx);
  } else if (event->buttons() & Qt::RightButton) {
    setXRotation(m_xRot + 8 * dy);
    setZRotation(m_zRot + 8 * dx);
  }
  m_lastPos = event->pos();
}
