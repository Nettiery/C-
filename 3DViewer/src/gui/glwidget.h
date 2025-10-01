#ifndef S21_GLWIDGET_H
#define S21_GLWIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <vector>

#include "options.h"

namespace s21 {

/**
 * @brief The GLWidget class provides a widget for rendering 3D models using
 * OpenGL.
 *
 * It handles the loading of vertex and index data, rendering of the model with
 * various options (projection type, colors, line styles, etc.), and provides
 * methods for manipulating the view.
 */
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  /**
   * @brief Constructs a GLWidget.
   * @param parent The parent widget.
   */
  explicit GLWidget(QWidget* parent = nullptr);

  /**
   * @brief Destroys the GLWidget and cleans up OpenGL resources.
   */
  ~GLWidget() override;

  /**
   * @brief Sets the vertex data for the model.
   * @param vertices A vector of floats representing the vertex coordinates (x,
   * y, z).
   */
  void setVertexData(const std::vector<float>& vertices);

  /**
   * @brief Sets the index data for the model.
   * @param indices A vector of unsigned integers representing the indices of
   * the vertices that form the faces of the model.
   */
  void setIndexData(const std::vector<unsigned int>& indices);

  /**
   * @brief Sets the projection type (orthographic or perspective).
   * @param type The projection type to use.
   */
  void setProjectionType(s21::ProjectionType type);

  /**
   * @brief Gets the current projection type.
   * @return The current projection type.
   */
  s21::ProjectionType getProjectionType();

  /**
   * @brief Sets the rendering options.
   * @param options The rendering options to use.
   */
  void setOptions(const Options& options);

 protected:
  /**
   * @brief Initializes the OpenGL context and resources.
   */
  void initializeGL() override;

  /**
   * @brief Handles resizing of the widget.
   * @param w The new width of the widget.
   * @param h The new height of the widget.
   */
  void resizeGL(int w, int h) override;

  /**
   * @brief Renders the 3D model.
   */
  void paintGL() override;

 private:
  /**
   * @brief Draws thick lines for the edges of the model.
   */
  void drawThickLines();

  // Vertex Buffer Object for storing vertex data on the GPU.
  QOpenGLBuffer m_vertexBuffer;
  // Index Buffer Object for storing index data on the GPU.
  QOpenGLBuffer m_indexBuffer;

  int m_width;        // Width of the widget.
  int m_height;       // Height of the widget.
  int m_vertexCount;  // Number of vertices in the model.
  int m_indexCount;   // Number of indices in the model.

  // Rendering options.
  Options m_options;
  // A copy of the vertex data stored on the CPU.
  std::vector<float> m_vertexData;
  // A copy of the index data stored on the CPU.
  std::vector<unsigned int> m_originalIndices;
};

}  // namespace s21

#endif  // S21_GLWIDGET_H