/**
 * @file glwidget.cpp
 * @brief Implementation of the GLWidget class.
 */

#include "glwidget.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <cmath>
#include <set>
#include <utility>

namespace s21 {

/**
 * @brief Constructs a GLWidget.
 * @param parent The parent widget.
 */
GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_width(0),
      m_height(0),
      m_vertexCount(0),
      m_indexCount(0) {}

/**
 * @brief Destroys the GLWidget and cleans up OpenGL resources.
 */
GLWidget::~GLWidget() {
  makeCurrent();  // Ensure the context is current for cleanup
  m_vertexBuffer.destroy();
  m_indexBuffer.destroy();
  doneCurrent();
}

/**
 * @brief Sets the vertex data for the model.
 * @param vertices A vector of floats representing the vertex coordinates (x, y,
 * z).
 */
void GLWidget::setVertexData(const std::vector<float>& vertices) {
  makeCurrent();
  m_vertexBuffer.bind();
  m_vertexBuffer.allocate(vertices.data(), vertices.size() * sizeof(float));
  m_vertexData = vertices;  // Store a copy for CPU access
  m_vertexCount = vertices.size();
  doneCurrent();
}

/**
 * @brief Sets the index data for the model.
 * @param indices A vector of unsigned integers representing the indices of the
 * vertices that form the faces of the model.
 */
void GLWidget::setIndexData(const std::vector<unsigned int>& indices) {
  makeCurrent();
  m_indexBuffer.bind();
  m_indexBuffer.allocate(indices.data(), indices.size() * sizeof(unsigned int));
  m_indexCount = indices.size();
  m_originalIndices = indices;  // Keep a copy on the CPU
  doneCurrent();
  update();
}

/**
 * @brief Sets the projection type.
 * @param type The projection type to use.
 */
void GLWidget::setProjectionType(s21::ProjectionType type) {
  m_options.projectionType = type;
  update();
}

/**
 * @brief Gets the current projection type.
 * @return The current projection type.
 */
s21::ProjectionType GLWidget::getProjectionType() {
  return m_options.projectionType;
}

/**
 * @brief Sets the rendering options.
 * @param options The rendering options to use.
 */
void GLWidget::setOptions(const Options& options) {
  m_options = options;
  update();
}

/**
 * @brief Initializes the OpenGL context and resources.
 */
void GLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // Create Vertex and Index Buffer Objects
  m_vertexBuffer.create();
  m_indexBuffer.create();
}

/**
 * @brief Handles resizing of the widget.
 * @param w The new width of the widget.
 * @param h The new height of the widget.
 */
void GLWidget::resizeGL(int w, int h) {
  m_width = w;
  m_height = h;
  glViewport(0, 0, w, h);
}

/**
 * @brief Renders the 3D model.
 */
void GLWidget::paintGL() {
  // Set the background color
  glClearColor(m_options.backgroundColor.redF(),
               m_options.backgroundColor.greenF(),
               m_options.backgroundColor.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float aspect = static_cast<float>(m_width) /
                 static_cast<float>(m_height > 0 ? m_height : 1);
  if (m_options.projectionType == s21::ProjectionType::Orthographic) {
    float ortho_size = 1.0f;
    glOrtho(-ortho_size * aspect, ortho_size * aspect, -ortho_size, ortho_size,
            -100.0, 100.0);
  } else {
    constexpr double near_plane = 1.0, far_plane = 100.0;
    constexpr double fovY = 45.0;
    double top = tan(fovY * M_PI / 360.0) * near_plane;
    double right = top * aspect;
    glFrustum(-right, right, -top, top, near_plane, far_plane);
  }

  // Set up the model-view matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -5.0);  // Move the camera back

  if (m_indexCount == 0) return;

  glEnableClientState(GL_VERTEX_ARRAY);
  m_vertexBuffer.bind();
  glVertexPointer(3, GL_FLOAT, 0, nullptr);
  m_indexBuffer.bind();

  // Draw edges
  if (m_options.lineThickness > 0) {
    if (m_options.lineThickness == 1 && m_options.lineType == LineType::Solid) {
      glDisable(GL_CULL_FACE);
      glColor3f(m_options.color.redF(), m_options.color.greenF(),
                m_options.color.blueF());
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
      glEnable(GL_CULL_FACE);
    } else {
      drawThickLines();
    }
  }

  // Draw vertices
  if (m_options.pointType != s21::PointType::None) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(m_options.pointColor.redF(), m_options.pointColor.greenF(),
              m_options.pointColor.blueF());
    glPointSize(m_options.pointSize);

    if (m_options.pointType == s21::PointType::Circle) {
      float r = m_options.pointSize * 0.0025f;
      for (int i = 0; i < m_vertexCount / 3; ++i) {
        float x = m_vertexData[i * 3 + 0];
        float y = m_vertexData[i * 3 + 1];
        float z = m_vertexData[i * 3 + 2];
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(m_options.pointColor.redF(), m_options.pointColor.greenF(),
                  m_options.pointColor.blueF());
        glVertex3f(x, y, z);
        for (int j = 0; j <= 20; ++j) {
          float angle = j * 2.0f * M_PI / 20;
          glVertex3f(x + cos(angle) * r, y + sin(angle) * r, z);
        }
        glEnd();
      }
    } else {  // Square points
      glDrawElements(GL_POINTS, m_indexCount, GL_UNSIGNED_INT, nullptr);
    }
  }

  glDisableClientState(GL_VERTEX_ARRAY);
}

/**
 * @brief Draws thick or dashed lines for the edges of the model.
 *
 * This function manually projects, clips, and renders each edge as a quad to
 * control its thickness and style.
 */
void GLWidget::drawThickLines() {
  if (m_vertexData.empty() || m_originalIndices.empty()) return;

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  GLfloat modelview[16];
  GLfloat projection[16];
  GLint viewport[4];
  glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
  glGetFloatv(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);

  // Lambda to project a 3D object point to 4D clip coordinates
  auto projectToClip = [&](const QVector3D& obj, QVector4D& clip) {
    QVector4D in(obj.x(), obj.y(), obj.z(), 1.0f);
    auto multiply = [](const GLfloat m[16], const QVector4D& v) -> QVector4D {
      return QVector4D(
          m[0] * v.x() + m[4] * v.y() + m[8] * v.z() + m[12] * v.w(),
          m[1] * v.x() + m[5] * v.y() + m[9] * v.z() + m[13] * v.w(),
          m[2] * v.x() + m[6] * v.y() + m[10] * v.z() + m[14] * v.w(),
          m[3] * v.x() + m[7] * v.y() + m[11] * v.z() + m[15] * v.w());
    };
    QVector4D eye = multiply(modelview, in);
    clip = multiply(projection, eye);
  };

  // Sutherland-Hodgman-like clipping for a line in 4D homogeneous coordinates
  auto clipLine = [](QVector4D& p1, QVector4D& p2) -> bool {
    auto compute_outcode = [](const QVector4D& p) {
      int code = 0;
      constexpr double epsilon = 1e-6;
      if (p.x() < -p.w() - epsilon) code |= 1;   // Left
      if (p.x() > p.w() + epsilon) code |= 2;    // Right
      if (p.y() < -p.w() - epsilon) code |= 4;   // Bottom
      if (p.y() > p.w() + epsilon) code |= 8;    // Top
      if (p.z() < -p.w() - epsilon) code |= 16;  // Near
      if (p.z() > p.w() + epsilon) code |= 32;   // Far
      return code;
    };

    int outcode1 = compute_outcode(p1);
    int outcode2 = compute_outcode(p2);

    for (int i = 0; i < 10;
         ++i) {  // Max 10 iterations to prevent infinite loops
      if (!(outcode1 | outcode2)) return true;  // Both points inside
      if (outcode1 & outcode2) return false;  // Both points outside same plane

      int outcode_out = outcode1 ? outcode1 : outcode2;
      QVector4D p_intersect;
      double t = 0.0;

      auto calculate_t = [&](double val1, double w1, double val2, double w2) {
        double num = w1 - val1;
        double den = num - (w2 - val2);
        return std::abs(den) < 1e-9 ? -1.0 : num / den;
      };

      if (outcode_out & 1)
        t = calculate_t(-p1.x(), p1.w(), -p2.x(), p2.w());  // Left
      else if (outcode_out & 2)
        t = calculate_t(p1.x(), p1.w(), p2.x(), p2.w());  // Right
      else if (outcode_out & 4)
        t = calculate_t(-p1.y(), p1.w(), -p2.y(), p2.w());  // Bottom
      else if (outcode_out & 8)
        t = calculate_t(p1.y(), p1.w(), p2.y(), p2.w());  // Top
      else if (outcode_out & 16)
        t = calculate_t(-p1.z(), p1.w(), -p2.z(), p2.w());  // Near
      else if (outcode_out & 32)
        t = calculate_t(p1.z(), p1.w(), p2.z(), p2.w());  // Far

      if (t < 0)
        return false;  // Avoid division by zero or invalid intersection
      t = std::max(0.0, std::min(1.0, t));
      p_intersect = p1 + t * (p2 - p1);

      if (outcode_out == outcode1) {
        p1 = p_intersect;
        outcode1 = compute_outcode(p1);
      } else {
        p2 = p_intersect;
        outcode2 = compute_outcode(p2);
      }
    }
    return false;  // Should not be reached if clipping is successful
  };

  // Set up 2D orthographic projection for drawing lines
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, m_width, 0, m_height, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);

  glColor3f(m_options.color.redF(), m_options.color.greenF(),
            m_options.color.blueF());
  float thickness = m_options.lineThickness;

  glBegin(GL_QUADS);

  std::set<std::pair<unsigned int, unsigned int>> drawn_edges;

  for (size_t i = 0; i < m_originalIndices.size(); i += 3) {
    unsigned int indices[3] = {m_originalIndices[i], m_originalIndices[i + 1],
                               m_originalIndices[i + 2]};

    for (int j = 0; j < 3; ++j) {
      unsigned int i1 = indices[j];
      unsigned int i2 = indices[(j + 1) % 3];

      if (i1 > i2) std::swap(i1, i2);
      if (drawn_edges.count({i1, i2})) continue;
      drawn_edges.insert({i1, i2});

      QVector3D p1_world(m_vertexData[i1 * 3], m_vertexData[i1 * 3 + 1],
                         m_vertexData[i1 * 3 + 2]);
      QVector3D p2_world(m_vertexData[i2 * 3], m_vertexData[i2 * 3 + 1],
                         m_vertexData[i2 * 3 + 2]);

      QVector4D p1_clip, p2_clip;
      projectToClip(p1_world, p1_clip);
      projectToClip(p2_world, p2_clip);

      if (!clipLine(p1_clip, p2_clip)) continue;

      // Convert clipped clip-space coordinates to screen coordinates
      auto toScreen = [&](const QVector4D& clip_p, QVector3D& screen_p) {
        if (std::abs(clip_p.w()) < 1e-9) return false;
        QVector3D ndc(clip_p.x() / clip_p.w(), clip_p.y() / clip_p.w(),
                      clip_p.z() / clip_p.w());
        screen_p.setX((ndc.x() + 1.0f) / 2.0f * viewport[2] + viewport[0]);
        screen_p.setY((ndc.y() + 1.0f) / 2.0f * viewport[3] + viewport[1]);
        screen_p.setZ((ndc.z() + 1.0f) / 2.0f);
        return true;
      };

      QVector3D s1, s2;
      if (!toScreen(p1_clip, s1) || !toScreen(p2_clip, s2)) continue;

      QVector2D dir = QVector2D(s2.x() - s1.x(), s2.y() - s1.y());
      float line_length = dir.length();
      if (line_length < 1e-6) continue;
      dir.normalize();

      QVector2D perp(-dir.y(), dir.x());
      perp *= thickness / 2.0f;

      if (m_options.lineType == s21::LineType::Dashed) {
        constexpr float dash_length = 10.0f;
        constexpr float gap_length = 5.0f;
        float current_pos = 0.0f;
        bool is_dash = true;

        while (current_pos < line_length) {
          float segment_length = is_dash ? dash_length : gap_length;
          float end_pos = std::min(current_pos + segment_length, line_length);

          if (is_dash) {
            QVector2D start_point =
                QVector2D(s1.x(), s1.y()) + dir * current_pos;
            QVector2D end_point = QVector2D(s1.x(), s1.y()) + dir * end_pos;

            glVertex2f(start_point.x() + perp.x(), start_point.y() + perp.y());
            glVertex2f(start_point.x() - perp.x(), start_point.y() - perp.y());
            glVertex2f(end_point.x() - perp.x(), end_point.y() - perp.y());
            glVertex2f(end_point.x() + perp.x(), end_point.y() + perp.y());
          }

          current_pos = end_pos;
          is_dash = !is_dash;
        }
      } else {  // Solid thick line
        glVertex2f(s1.x() + perp.x(), s1.y() + perp.y());
        glVertex2f(s1.x() - perp.x(), s1.y() - perp.y());
        glVertex2f(s2.x() - perp.x(), s2.y() - perp.y());
        glVertex2f(s2.x() + perp.x(), s2.y() + perp.y());
      }
    }
  }
  glEnd();

  // Restore OpenGL state
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

}  // namespace s21
