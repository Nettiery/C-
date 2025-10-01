#ifndef S21_OPTIONS_H
#define S21_OPTIONS_H

#include <QColor>
#include <iostream>

namespace s21 {

/**
 * @brief The ProjectionType enum defines the types of projection.
 */
enum class ProjectionType {
  Orthographic, /**< Orthographic projection */
  Perspective   /**< Perspective projection */
};

/**
 * @brief The LineType enum defines the types of lines.
 */
enum class LineType {
  Solid, /**< Solid line */
  Dashed /**< Dashed line */
};

/**
 * @brief The PointType enum defines the types of points.
 */
enum class PointType {
  None,   /**< No points */
  Circle, /**< Circular points */
  Square  /**< Square points */
};

/**
 * @brief The Options struct holds the rendering options.
 */
struct Options {
  ProjectionType projectionType = ProjectionType::Orthographic;
  LineType lineType = LineType::Solid;
  QColor color = Qt::red;
  float lineThickness = 1.0;
  PointType pointType = PointType::None;
  QColor pointColor = Qt::white;
  int pointSize = 1;
  QColor backgroundColor = Qt::black;

  /**
   * @brief Saves the options to a stream.
   * @param out The output stream.
   */
  void save(std::ostream& out) const {
    out << static_cast<int>(projectionType) << " " << static_cast<int>(lineType)
        << " " << color.rgb() << " " << lineThickness << " "
        << static_cast<int>(pointType) << " " << pointColor.rgb() << " "
        << pointSize << " " << backgroundColor.rgb() << "\n";
  }

  /**
   * @brief Loads the options from a stream.
   * @param in The input stream.
   * @return True if the options were loaded successfully, false otherwise.
   */
  bool load(std::istream& in) {
    int ptype, ltype, pntType, pntS;
    float lt;
    unsigned int c, pntC, bgC;
    if (!(in >> ptype >> ltype >> c >> lt >> pntType >> pntC >> pntS >> bgC))
      return false;
    projectionType = static_cast<ProjectionType>(ptype);
    lineType = static_cast<LineType>(ltype);
    color = QColor(c);
    lineThickness = lt;
    pointType = static_cast<PointType>(pntType);
    pointColor = QColor(pntC);
    pointSize = pntS;
    backgroundColor = QColor(bgC);
    return true;
  }
};

}  // namespace s21

#endif  // S21_OPTIONS_H
