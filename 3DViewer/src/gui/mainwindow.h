#ifndef S21_MAINWINDOW_H
#define S21_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <vector>

#include "../controller/controller.h"
#include "../gifimage/qgifimage.h"
#include "../model/common.h"
#include "glwidget.h"
#include "options.h"

class QPushButton;
class QLabel;
class QGridLayout;
class QLineEdit;
class QComboBox;
class QFrame;
class QVBoxLayout;

namespace s21 {

/**
 * @brief The MainWindow class provides the main application window.
 *
 * It contains the 3D viewer, control panels for transformations and display
 * settings, and handles user interactions.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Constructs a MainWindow.
   * @param parent The parent widget.
   */
  explicit MainWindow(QWidget* parent = nullptr);

  /**
   * @brief Destroys the MainWindow and saves the current settings.
   */
  ~MainWindow() override;

 private slots:
  // Slots for handling button clicks
  void onLoadFileClicked();
  void onTransformButtonClicked();
  void onResetButtonClicked();
  void onRotationInputEdited();
  void onSettingsChanged();
  void onScreenshotButtonClicked();
  void onRecordButtonClicked();
  void recordFrame();

 private:
  // --- UI Setup Method ---
  void setupUi();

  // --- UI Update Method ---
  void updateUiFromModel();

  // --- Member Variables ---
  Controller controller;         // The controller for managing the 3D model
  std::string file_path_string;  // The path to the currently loaded .obj file

  // Maps for transform buttons
  std::unordered_map<QPushButton*, s21::Vertex> rotateButtons_;
  std::unordered_map<QPushButton*, s21::Vertex> translateButtons_;

  // GIF recording members
  QTimer* record_timer_ = nullptr;
  QGifImage* gif_ = nullptr;
  QString gif_file_name_;
  int frame_count_ = 0;

  // --- UI Elements ---
  GLWidget* m_glWidget;

  // Layouts
  QGridLayout* m_controlsLayout;

  // File loading
  QPushButton* m_loadButton;
  QLabel* m_fileNameLabel;

  // Info display
  QLabel* m_verticesLabel;
  QLabel* m_edgesLabel;

  // Transformation Control Buttons
  QPushButton* m_translateUpButton;
  QPushButton* m_translateDownButton;
  QPushButton* m_translateLeftButton;
  QPushButton* m_translateRightButton;
  QPushButton* m_scaleUpButton;
  QPushButton* m_scaleDownButton;

  // Numerical Input Fields & Apply Buttons
  QLineEdit* m_translateXEdit;
  QLineEdit* m_translateYEdit;
  QLineEdit* m_translateZEdit;
  QPushButton* m_applyTranslateButton;

  QLineEdit* m_rotateXEdit;
  QLineEdit* m_rotateYEdit;
  QLineEdit* m_rotateZEdit;
  QPushButton* m_applyRotateButton;

  QPushButton* m_rotatePosXButton;
  QPushButton* m_rotateNegXButton;
  QPushButton* m_rotatePosYButton;
  QPushButton* m_rotateNegYButton;
  QPushButton* m_rotatePosZButton;
  QPushButton* m_rotateNegZButton;

  QLineEdit* m_scaleEdit;
  QPushButton* m_applyScaleButton;

  // Resetting and Normalize
  QPushButton* m_resetButton;
  QPushButton* m_normalizeButton;

  // Display Settings UI Elements
  QComboBox* m_projectionTypeComboBox;
  QComboBox* m_edgeTypeComboBox;
  QLineEdit* m_edgeThicknessEdit;
  QComboBox* m_edgeColorComboBox;
  QComboBox* m_vertexMethodComboBox;
  QLineEdit* m_vertexSizeEdit;
  QComboBox* m_vertexColorComboBox;
  QComboBox* m_backgroundColorComboBox;

  // Screenshot and GIF record Buttons
  QPushButton* m_screenshotButton;
  QPushButton* m_recordButton;
};

}  // namespace s21

#endif  // S21_MAINWINDOW_H
