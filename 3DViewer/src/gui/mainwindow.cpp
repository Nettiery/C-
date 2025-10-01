/**
 * @file mainwindow.cpp
 * @brief Implementation of the MainWindow class.
 */

#include "mainwindow.h"

#include <QColorDialog>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <fstream>

#include "glwidget.h"

namespace s21 {

// Helper function to populate a color combo box with standard colors
void populateColorComboBox(QComboBox* comboBox) {
  comboBox->addItem("Black", QColor(Qt::black));
  comboBox->addItem("White", QColor(Qt::white));
  comboBox->addItem("Dark gray", QColor(Qt::darkGray));
  comboBox->addItem("Red", QColor(Qt::red));
  comboBox->addItem("Green", QColor(Qt::green));
  comboBox->addItem("Blue", QColor(Qt::blue));
  comboBox->addItem("Yellow", QColor(Qt::yellow));
  comboBox->addItem("Cyan", QColor(Qt::cyan));
  comboBox->addItem("Magenta", QColor(Qt::magenta));
}

/**
 * @brief Constructs a MainWindow, sets up the UI, and loads settings.
 * @param parent The parent widget.
 */
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setupUi();

  Options options;
  std::ifstream settings_file("settings.conf");
  if (settings_file.is_open()) {
    options.load(settings_file);
  }

  // Initialize button maps for transformations
  rotateButtons_ = {
      {m_rotatePosXButton, {1, 0, 0}}, {m_rotateNegXButton, {-1, 0, 0}},
      {m_rotatePosYButton, {0, 1, 0}}, {m_rotateNegYButton, {0, -1, 0}},
      {m_rotatePosZButton, {0, 0, 1}}, {m_rotateNegZButton, {0, 0, -1}},
  };

  translateButtons_ = {
      {m_translateUpButton, {0, 1, 0}},
      {m_translateDownButton, {0, -1, 0}},
      {m_translateLeftButton, {-1, 0, 0}},
      {m_translateRightButton, {1, 0, 0}},
  };

  // Initialize UI with loaded options
  m_projectionTypeComboBox->setCurrentIndex(
      static_cast<int>(options.projectionType));
  m_edgeTypeComboBox->setCurrentIndex(static_cast<int>(options.lineType));
  m_edgeThicknessEdit->setText(QString::number(options.lineThickness));
  m_vertexMethodComboBox->setCurrentIndex(static_cast<int>(options.pointType));
  m_vertexSizeEdit->setText(QString::number(options.pointSize));

  // Set initial colors
  m_edgeColorComboBox->setCurrentIndex(
      m_edgeColorComboBox->findData(options.color));
  m_vertexColorComboBox->setCurrentIndex(
      m_vertexColorComboBox->findData(options.pointColor));
  m_backgroundColorComboBox->setCurrentIndex(
      m_backgroundColorComboBox->findData(options.backgroundColor));

  onSettingsChanged();
}

/**
 * @brief Destroys the MainWindow and saves the current settings.
 */
MainWindow::~MainWindow() {
  Options currentOptions;
  currentOptions.projectionType =
      static_cast<ProjectionType>(m_projectionTypeComboBox->currentIndex());
  currentOptions.lineType =
      static_cast<LineType>(m_edgeTypeComboBox->currentIndex());
  currentOptions.lineThickness = m_edgeThicknessEdit->text().toFloat();
  currentOptions.pointType =
      static_cast<PointType>(m_vertexMethodComboBox->currentIndex());
  currentOptions.pointSize = m_vertexSizeEdit->text().toInt();
  currentOptions.color = m_edgeColorComboBox->currentData().value<QColor>();
  currentOptions.pointColor =
      m_vertexColorComboBox->currentData().value<QColor>();
  currentOptions.backgroundColor =
      m_backgroundColorComboBox->currentData().value<QColor>();

  std::ofstream settings_file("settings.conf");
  if (settings_file.is_open()) {
    currentOptions.save(settings_file);
  }
}

/**
 * @brief Sets up the UI elements of the main window.
 */
void MainWindow::setupUi() {
  setWindowTitle("3DViewer v2.0");
  resize(1452, 720);

  // --- Central Widget and Main Layout ---
  QWidget* centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

  // --- OpenGL Widget (Visualization Area) ---
  m_glWidget = new GLWidget(this);
  m_glWidget->setMinimumSize(800, 600);
  mainLayout->addWidget(m_glWidget, 1);

  // Delay update to ensure widget is fully shown before updating
  QTimer::singleShot(250, m_glWidget, SLOT(update()));

  // --- Controls Panel ---
  m_controlsLayout = new QGridLayout();
  mainLayout->addLayout(m_controlsLayout);

  // --- Column 1: File & Display ---
  QVBoxLayout* column1Layout = new QVBoxLayout();
  m_controlsLayout->addLayout(column1Layout, 0, 0);

  // --- File Loading & Info ---
  column1Layout->addWidget(new QLabel("<h3>File & Model Info</h3>", this));
  m_loadButton = new QPushButton("Select .obj File", this);
  m_fileNameLabel = new QLabel("No file selected.", this);
  m_verticesLabel = new QLabel("<b>Vertices:</b> 0", this);
  m_edgesLabel = new QLabel("<b>Edges:</b> 0", this);
  column1Layout->addWidget(m_loadButton);
  column1Layout->addWidget(m_fileNameLabel);
  column1Layout->addWidget(m_verticesLabel);
  column1Layout->addWidget(m_edgesLabel);
  column1Layout->addStretch();

  // --- Display Settings ---
  column1Layout->addWidget(new QLabel("<h3>Display Settings</h3>", this));

  // Projection Type
  QHBoxLayout* projectionLayout = new QHBoxLayout();
  projectionLayout->addWidget(new QLabel("Projection:", this));
  m_projectionTypeComboBox = new QComboBox(this);
  m_projectionTypeComboBox->addItem("Orthographic");
  m_projectionTypeComboBox->addItem("Perspective");
  projectionLayout->addWidget(m_projectionTypeComboBox);
  column1Layout->addLayout(projectionLayout);

  // Edge Settings
  QGridLayout* edgeSettingsLayout = new QGridLayout();
  edgeSettingsLayout->addWidget(new QLabel("Edge Type:", this), 0, 0);
  m_edgeTypeComboBox = new QComboBox(this);
  m_edgeTypeComboBox->addItem("Solid");
  m_edgeTypeComboBox->addItem("Dashed");
  edgeSettingsLayout->addWidget(m_edgeTypeComboBox, 0, 1);

  edgeSettingsLayout->addWidget(new QLabel("Edge Thickness:", this), 1, 0);
  m_edgeThicknessEdit = new QLineEdit("1.0", this);
  edgeSettingsLayout->addWidget(m_edgeThicknessEdit, 1, 1);

  edgeSettingsLayout->addWidget(new QLabel("Edge Color:", this), 2, 0);
  m_edgeColorComboBox = new QComboBox(this);
  populateColorComboBox(m_edgeColorComboBox);
  edgeSettingsLayout->addWidget(m_edgeColorComboBox, 2, 1);
  column1Layout->addLayout(edgeSettingsLayout);

  // Vertex Settings
  QGridLayout* vertexSettingsLayout = new QGridLayout();
  vertexSettingsLayout->addWidget(new QLabel("Vertex Method:", this), 0, 0);
  m_vertexMethodComboBox = new QComboBox(this);
  m_vertexMethodComboBox->addItem("None");
  m_vertexMethodComboBox->addItem("Circle");
  m_vertexMethodComboBox->addItem("Square");
  vertexSettingsLayout->addWidget(m_vertexMethodComboBox, 0, 1);

  vertexSettingsLayout->addWidget(new QLabel("Vertex Size:", this), 1, 0);
  m_vertexSizeEdit = new QLineEdit("1.0", this);
  vertexSettingsLayout->addWidget(m_vertexSizeEdit, 1, 1);

  vertexSettingsLayout->addWidget(new QLabel("Vertex Color:", this), 2, 0);
  m_vertexColorComboBox = new QComboBox(this);
  populateColorComboBox(m_vertexColorComboBox);
  vertexSettingsLayout->addWidget(m_vertexColorComboBox, 2, 1);
  column1Layout->addLayout(vertexSettingsLayout);

  // Background Color
  QHBoxLayout* backgroundColorLayout = new QHBoxLayout();
  backgroundColorLayout->addWidget(new QLabel("Background Color:", this));
  m_backgroundColorComboBox = new QComboBox(this);
  populateColorComboBox(m_backgroundColorComboBox);
  backgroundColorLayout->addWidget(m_backgroundColorComboBox);
  column1Layout->addLayout(backgroundColorLayout);

  // --- Separator ---
  QFrame* separator = new QFrame(this);
  separator->setFrameShape(QFrame::VLine);
  separator->setFrameShadow(QFrame::Sunken);
  m_controlsLayout->addWidget(separator, 0, 1, 2, 1);

  // --- Column 2: Transformations & View ---
  QVBoxLayout* column2Layout = new QVBoxLayout();
  m_controlsLayout->addLayout(column2Layout, 0, 2);

  // --- Translation Controls ---
  column2Layout->addWidget(new QLabel("<h3>Translate</h3>", this));
  QGridLayout* translateButtonsLayout = new QGridLayout();
  m_translateUpButton = new QPushButton("Up", this);
  m_translateDownButton = new QPushButton("Down", this);
  m_translateLeftButton = new QPushButton("Left", this);
  m_translateRightButton = new QPushButton("Right", this);
  translateButtonsLayout->addWidget(m_translateUpButton, 0, 1);
  translateButtonsLayout->addWidget(m_translateLeftButton, 1, 0);
  translateButtonsLayout->addWidget(m_translateDownButton, 1, 1);
  translateButtonsLayout->addWidget(m_translateRightButton, 1, 2);
  column2Layout->addLayout(translateButtonsLayout);

  QGridLayout* translateEditLayout = new QGridLayout();
  m_translateXEdit = new QLineEdit("0.0", this);
  m_translateYEdit = new QLineEdit("0.0", this);
  m_translateZEdit = new QLineEdit("0.0", this);
  m_applyTranslateButton = new QPushButton("Apply", this);
  translateEditLayout->addWidget(new QLabel("X:", this), 0, 0);
  translateEditLayout->addWidget(m_translateXEdit, 0, 1);
  translateEditLayout->addWidget(new QLabel("Y:", this), 1, 0);
  translateEditLayout->addWidget(m_translateYEdit, 1, 1);
  translateEditLayout->addWidget(new QLabel("Z:", this), 2, 0);
  translateEditLayout->addWidget(m_translateZEdit, 2, 1);
  column2Layout->addLayout(translateEditLayout);
  column2Layout->addWidget(m_applyTranslateButton);
  column2Layout->addStretch();

  // --- Rotation Controls ---
  column2Layout->addWidget(new QLabel("<h3>Rotate</h3>", this));
  QGridLayout* rotateEditLayout = new QGridLayout();
  m_rotateXEdit = new QLineEdit("0.0", this);
  m_rotateYEdit = new QLineEdit("0.0", this);
  m_rotateZEdit = new QLineEdit("0.0", this);
  m_applyRotateButton = new QPushButton("Apply", this);
  rotateEditLayout->addWidget(new QLabel("X:", this), 0, 0);
  rotateEditLayout->addWidget(m_rotateXEdit, 0, 1);
  rotateEditLayout->addWidget(new QLabel("Y:", this), 1, 0);
  rotateEditLayout->addWidget(m_rotateYEdit, 1, 1);
  rotateEditLayout->addWidget(new QLabel("Z:", this), 2, 0);
  rotateEditLayout->addWidget(m_rotateZEdit, 2, 1);
  rotateEditLayout->addWidget(m_applyRotateButton, 3, 0, 1, 2);
  column2Layout->addLayout(rotateEditLayout);

  // Add 6 default rotation buttons
  QGridLayout* rotateDefaultLayout = new QGridLayout();
  m_rotatePosXButton = new QPushButton("+X", this);
  m_rotateNegXButton = new QPushButton("-X", this);
  m_rotatePosYButton = new QPushButton("+Y", this);
  m_rotateNegYButton = new QPushButton("-Y", this);
  m_rotatePosZButton = new QPushButton("+Z", this);
  m_rotateNegZButton = new QPushButton("-Z", this);
  rotateDefaultLayout->addWidget(m_rotatePosXButton, 0, 0);
  rotateDefaultLayout->addWidget(m_rotateNegXButton, 0, 1);
  rotateDefaultLayout->addWidget(m_rotatePosYButton, 1, 0);
  rotateDefaultLayout->addWidget(m_rotateNegYButton, 1, 1);
  rotateDefaultLayout->addWidget(m_rotatePosZButton, 2, 0);
  rotateDefaultLayout->addWidget(m_rotateNegZButton, 2, 1);
  column2Layout->addLayout(rotateDefaultLayout);
  column2Layout->addStretch();

  // --- Scaling Controls ---
  column2Layout->addWidget(new QLabel("<h3>Scale</h3>", this));
  QHBoxLayout* scaleLayout = new QHBoxLayout();
  m_scaleDownButton = new QPushButton("-", this);
  m_scaleUpButton = new QPushButton("+", this);
  m_scaleEdit = new QLineEdit("1.0", this);
  m_applyScaleButton = new QPushButton("Apply", this);
  scaleLayout->addWidget(m_scaleDownButton);
  scaleLayout->addWidget(m_scaleUpButton);
  column2Layout->addLayout(scaleLayout);
  column2Layout->addWidget(m_scaleEdit);
  column2Layout->addWidget(m_applyScaleButton);
  column2Layout->addStretch();

  // --- View Reset Controls ---
  column2Layout->addWidget(new QLabel("<h3>View Control</h3>", this));
  m_normalizeButton = new QPushButton("Normalize", this);
  m_resetButton = new QPushButton("Reset", this);
  column2Layout->addWidget(m_normalizeButton);
  column2Layout->addWidget(m_resetButton);
  column2Layout->addStretch();

  // --- Screenshot & GIF Buttons ---
  m_screenshotButton = new QPushButton("Screenshot", this);
  m_recordButton = new QPushButton("Record GIF", this);
  m_controlsLayout->addWidget(m_screenshotButton, 1, 0);
  m_controlsLayout->addWidget(m_recordButton, 1, 2);

  // --- Connect Signals to Slots ---
  connect(m_loadButton, &QPushButton::clicked, this,
          &MainWindow::onLoadFileClicked);
  connect(m_screenshotButton, &QPushButton::clicked, this,
          &MainWindow::onScreenshotButtonClicked);
  connect(m_recordButton, &QPushButton::clicked, this,
          &MainWindow::onRecordButtonClicked);

  // Connect signals for settings
  connect(m_projectionTypeComboBox, &QComboBox::currentIndexChanged, this,
          &MainWindow::onSettingsChanged);
  connect(m_edgeTypeComboBox, &QComboBox::currentIndexChanged, this,
          &MainWindow::onSettingsChanged);
  connect(m_edgeThicknessEdit, &QLineEdit::textChanged, this,
          &MainWindow::onSettingsChanged);
  connect(m_edgeColorComboBox, &QComboBox::currentIndexChanged, this,
          &MainWindow::onSettingsChanged);
  connect(m_vertexMethodComboBox, &QComboBox::currentIndexChanged, this,
          &MainWindow::onSettingsChanged);
  connect(m_vertexSizeEdit, &QLineEdit::textChanged, this,
          &MainWindow::onSettingsChanged);
  connect(m_vertexColorComboBox, &QComboBox::currentIndexChanged, this,
          &MainWindow::onSettingsChanged);
  connect(m_backgroundColorComboBox, &QComboBox::currentIndexChanged, this,
          &MainWindow::onSettingsChanged);

  // Connect all transform buttons to the single slot
  connect(m_translateUpButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_translateDownButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_translateLeftButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_translateRightButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_scaleUpButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_scaleDownButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_applyTranslateButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_applyRotateButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_rotatePosXButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_rotateNegXButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_rotatePosYButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_rotateNegYButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_rotatePosZButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_rotateNegZButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);
  connect(m_applyScaleButton, &QPushButton::clicked, this,
          &MainWindow::onTransformButtonClicked);

  // Normalization and Reset buttons clicked
  connect(m_resetButton, &QPushButton::clicked, this,
          &MainWindow::onResetButtonClicked);
  connect(m_normalizeButton, &QPushButton::clicked, this,
          &MainWindow::onResetButtonClicked);

  // Rotation fields edit
  connect(m_rotateXEdit, &QLineEdit::textEdited, this,
          &MainWindow::onRotationInputEdited);
  connect(m_rotateYEdit, &QLineEdit::textEdited, this,
          &MainWindow::onRotationInputEdited);
  connect(m_rotateZEdit, &QLineEdit::textEdited, this,
          &MainWindow::onRotationInputEdited);
}

/**
 * @brief Handles the click event of the 'Load File' button.
 */
void MainWindow::onLoadFileClicked() {
  QString filePath = QFileDialog::getOpenFileName(
      this, "Open .obj File", QDir::homePath(), "OBJ Files (*.obj)");
  if (!filePath.isEmpty()) {
    m_fileNameLabel->setText("<b>File:</b> " + QFileInfo(filePath).fileName());
    file_path_string = filePath.toStdString();
    controller.executeCommand(
        std::make_unique<s21::OpenFileCommand>(file_path_string));
    updateUiFromModel();
  }
}

/**
 * @brief Handles the click events of all transformation buttons.
 */
void MainWindow::onTransformButtonClicked() {
  QPushButton* button = dynamic_cast<QPushButton*>(sender());
  if (!button) return;

  if (translateButtons_.count(button)) {
    controller.executeCommand(
        std::make_unique<TranslateCommand>(translateButtons_.at(button), true));
  } else if (rotateButtons_.count(button)) {
    controller.executeCommand(
        std::make_unique<RotateCommand>(rotateButtons_.at(button), true));
  } else if (button == m_applyTranslateButton) {
    float x = m_translateXEdit->text().toFloat();
    float y = m_translateYEdit->text().toFloat();
    float z = m_translateZEdit->text().toFloat();
    controller.executeCommand(
        std::make_unique<TranslateCommand>(Vertex{x, y, z}, false));
  } else if (button == m_applyRotateButton) {
    float x = m_rotateXEdit->text().toFloat();
    float y = m_rotateYEdit->text().toFloat();
    float z = m_rotateZEdit->text().toFloat();
    controller.executeCommand(
        std::make_unique<RotateCommand>(Vertex{x, y, z}, false));
  } else if (button == m_scaleUpButton) {
    controller.executeCommand(std::make_unique<ScaleCommand>(1.1f, false));
  } else if (button == m_scaleDownButton) {
    controller.executeCommand(std::make_unique<ScaleCommand>(0.9f, false));
  } else if (button == m_applyScaleButton) {
    float scale = m_scaleEdit->text().toFloat();
    if (scale > 0) {
      controller.executeCommand(std::make_unique<ScaleCommand>(scale, false));
    } else {
      QMessageBox::warning(this, "Invalid Scale",
                           "Scale value must be greater than 0.");
    }
  }
  updateUiFromModel();
}

/**
 * @brief Handles the click events of the 'Reset' and 'Normalize' buttons.
 */
void MainWindow::onResetButtonClicked() {
  QPushButton* button = dynamic_cast<QPushButton*>(sender());
  if (!button) return;

  if (button == m_normalizeButton) {
    controller.executeCommand(std::make_unique<NormalizeCommand>());
  } else if (button == m_resetButton) {
    controller.executeCommand(
        std::make_unique<s21::OpenFileCommand>(file_path_string));
  }
  updateUiFromModel();
}

/**
 * @brief Handles the text edited events of the rotation input fields.
 */
void MainWindow::onRotationInputEdited() {
  QLineEdit* active_field = dynamic_cast<QLineEdit*>(sender());
  if (!active_field) return;

  bool is_active =
      !active_field->text().isEmpty() && active_field->text().toFloat() != 0.0f;

  m_rotateXEdit->setEnabled(!is_active || active_field == m_rotateXEdit);
  m_rotateYEdit->setEnabled(!is_active || active_field == m_rotateYEdit);
  m_rotateZEdit->setEnabled(!is_active || active_field == m_rotateZEdit);

  if (is_active) {
    if (active_field != m_rotateXEdit) m_rotateXEdit->clear();
    if (active_field != m_rotateYEdit) m_rotateYEdit->clear();
    if (active_field != m_rotateZEdit) m_rotateZEdit->clear();
  }
}

/**
 * @brief Updates the UI with the current model data.
 */
void MainWindow::updateUiFromModel() {
  // Convert model data for OpenGL
  const auto& vertices = controller.getVertices();
  std::vector<float> floated_vertex;
  floated_vertex.reserve(vertices.size() * 3);
  for (const auto& v : vertices) {
    floated_vertex.push_back(v.x);
    floated_vertex.push_back(v.y);
    floated_vertex.push_back(v.z);
  }

  const auto& polygons = controller.getPolygons();
  std::vector<unsigned int> indices;
  indices.reserve(polygons.size() * 3);
  for (const auto& tri : polygons) {
    indices.push_back(tri.v1);
    indices.push_back(tri.v2);
    indices.push_back(tri.v3);
  }

  // Update OpenGL widget
  m_glWidget->setVertexData(floated_vertex);
  m_glWidget->setIndexData(indices);

  // Update info labels
  m_verticesLabel->setText(QString("<b>Vertices:</b> %1").arg(vertices.size()));
  m_edgesLabel->setText(QString("<b>Edges:</b> %1").arg(indices.size() / 2));
}

/**
 * @brief Handles changes in the display settings.
 */
void MainWindow::onSettingsChanged() {
  Options currentOptions;
  currentOptions.projectionType =
      static_cast<ProjectionType>(m_projectionTypeComboBox->currentIndex());
  currentOptions.lineType =
      static_cast<LineType>(m_edgeTypeComboBox->currentIndex());
  currentOptions.lineThickness = m_edgeThicknessEdit->text().toFloat();
  currentOptions.pointType =
      static_cast<PointType>(m_vertexMethodComboBox->currentIndex());
  currentOptions.pointSize = m_vertexSizeEdit->text().toFloat();

  // For colors, get the selected color from the combo box
  currentOptions.color = m_edgeColorComboBox->currentData().value<QColor>();
  currentOptions.pointColor =
      m_vertexColorComboBox->currentData().value<QColor>();
  currentOptions.backgroundColor =
      m_backgroundColorComboBox->currentData().value<QColor>();

  m_glWidget->setOptions(currentOptions);
}

/**
 * @brief Handles the click event of the 'Screenshot' button.
 */
void MainWindow::onScreenshotButtonClicked() {
  QImage screenshot = m_glWidget->grabFramebuffer();
  QString selectedFilter;
  QString filePath = QFileDialog::getSaveFileName(
      this, "Save Screenshot", QDir::homePath(),
      "BMP Files (*.bmp);;JPEG Files (*.jpeg)", &selectedFilter);

  if (!filePath.isEmpty()) {
    QFileInfo fi(filePath);
    if (fi.suffix().isEmpty()) {
      filePath += selectedFilter.contains("*.jpeg") ? ".jpeg" : ".bmp";
    }
    qDebug() << "Screenshot was saved at:" << filePath;
    screenshot.save(filePath);
  }
}

/**
 * @brief Handles the click event of the 'Record GIF' button.
 */
void MainWindow::onRecordButtonClicked() {
  gif_file_name_ = QFileDialog::getSaveFileName(this, "Save a gif animation",
                                                "", "GIF image (*.gif)");
  if (!gif_file_name_.isEmpty()) {
    m_recordButton->setDisabled(true);
    QFileInfo fi(gif_file_name_);
    if (fi.suffix().isEmpty()) {
      gif_file_name_ += ".gif";
    }
    gif_ = new QGifImage(QSize(640, 480));
    record_timer_ = new QTimer(this);
    gif_->setDefaultDelay(100);
    connect(record_timer_, &QTimer::timeout, this, &MainWindow::recordFrame);
    record_timer_->start(100);
  }
}

/**
 * @brief Records a single frame for the GIF animation.
 */
void MainWindow::recordFrame() {
  if (frame_count_ < 50) {
    QImage frame = m_glWidget->grabFramebuffer();
    gif_->addFrame(frame.scaled(640, 480));
    frame_count_++;
  } else {
    record_timer_->stop();
    gif_->save(gif_file_name_);
    delete gif_;
    gif_ = nullptr;
    delete record_timer_;
    record_timer_ = nullptr;
    frame_count_ = 0;
    m_recordButton->setDisabled(false);
    QMessageBox::information(
        this, "Success",
        QString("Gif animation saved: %1").arg(gif_file_name_));
  }
}

}  // namespace s21
