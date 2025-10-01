# 3D Viewer v2.0

A robust C++/Qt-based 3D viewer for `.obj` files. It allows for loading, viewing, and transforming 3D models with an intuitive graphical user interface.

## Features

-   **OBJ File Parsing:** Loads and renders 3D models from `.obj` files.
-   **Advanced Rendering:** Supports rendering models with both triangular and polygonal faces, with automatic triangulation for the latter.
-   **Model Transformations:**
    -   **Translation:** Move the model along the X, Y, and Z axes.
    -   **Rotation:** Rotate the model around the X, Y, and Z axes.
    -   **Scaling:** Uniformly scale the model up or down.
-   **OpenGL Integration:** Uses a custom OpenGL widget for efficient rendering.
-   **GUI:** Built with Qt, providing a user-friendly interface for all features.
-   **GIF Recording:** Capture and save the viewport as a GIF animation.
-   **Settings Persistence:** Saves and loads user settings for a consistent experience.

## Getting Started

### Prerequisites

-   **C++ Compiler:** A C++20 compatible compiler (e.g., GCC, Clang).
-   **Qt6:** The Qt 6 development libraries.
    -   On Debian/Ubuntu: `qtbase6-dev`, `qtbase6-dev-tools`, `libqt6core6a`, `libqt6gui6`, `libqt6widgets6`
-   **OpenGL:** The OpenGL development libraries.
    -   On Debian/Ubuntu: `libgl1-mesa-dev`
-   **Make:** The GNU Make utility.
-   **(Optional) Doxygen:** For generating documentation.
-   **(Optional) lcov:** For generating test coverage reports.
-   **(Optional) valgrind:** For memory leak checking on Linux.

### Installation on Debian/Ubuntu

```bash
sudo apt-get update
sudo apt-get install build-essential qtbase6-dev qtbase6-dev-tools libgl1-mesa-dev doxygen lcov valgrind
```

### Building the Application

To build the 3D Viewer, navigate to the `src` directory and run:

```bash
make viewer
```

This will compile the source code and create the executable at `src/build/3d_viewer`.

## Usage

1.  Launch the application by running `./build/3d_viewer` from the `src` directory.
2.  Click the **"Load File"** button to select an `.obj` model file.
3.  Use the controls in the GUI to translate, rotate, and scale the model.
4.  The viewport will update in real-time to reflect the transformations.
5.  The number of vertices and edges in the model are displayed in the UI.

## Testing

The project includes a suite of unit tests to ensure the correctness of the model loading and transformation logic.

To run the tests, execute the following command from the `src` directory:

```bash
make test
```

## Available Commands

The `Makefile` provides several commands for development and maintenance:

| Command           | Description                                                                  |
| ----------------- | ---------------------------------------------------------------------------- |
| `make viewer`     | Builds the main 3D Viewer application.                                       |
| `make test`       | Compiles and runs the unit tests.                                            |
| `make gcov_report`| Generates a test coverage report using `lcov`.                               |
| `make mem_check`  | Runs the tests with `valgrind` to check for memory leaks.                    |
| `make cpp_check`  | Performs static analysis of the code using `cppcheck`.                       |
| `make clang_format`     | Formats the source code using `clang-format`.                          |
| `make clang_format_test`| Tests the source code using `clang-format`.                            |
| `make install`    | Installs the application to the `./install` directory.                       |
| `make uninstall`  | Removes the installed application.                                           |
| `make dist`       | Creates a distributable archive of the source code in the `./dist` directory.|
| `make dvi`        | Generates documentation using `Doxygen` in the `./docs` directory.           |
| `make clean`      | Removes build artifacts, and the `dist` and `docs` directories.              |
| `make total_clean`| Performs a `clean` and `uninstall`, and removes `settings.conf`.             |

## Project Structure

```
src/
├── 3rdParty/         # Third-party libraries (giflib)
├── build/            # Build artifacts (automatically created)
├── controller/       # Controller component (MVC)
├── gifimage/         # GIF image generation library
├── gui/              # Qt GUI and OpenGL widget
├── model/            # Model, parser, and transformation logic
├── tests/            # Unit tests
├── Doxyfile          # Doxygen configuration
├── Makefile          # Build script
└── readme.md         # This file
```

## Troubleshooting

-   **Build errors:** Ensure all prerequisites are installed and that you are using a C++20 compatible compiler.
-   **Application does not run:** Verify that all Qt6 libraries are correctly installed and accessible.
-   **Model not rendering correctly:** Check the `.obj` file for correctness and ensure it contains face information.
