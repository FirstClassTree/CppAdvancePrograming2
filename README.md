# C++AdvancePrograming 2

This project is a C++ application (Tank Game) built using CMake.

## Prerequisites

*   A C++ compiler supporting C++20 (e.g., GCC, Clang, MSVC)
*   CMake (version 3.10 or higher recommended)
*   Make (or your chosen CMake generator like Ninja, Visual Studio, etc.)
*   Git (for cloning, if applicable)

## Building the Project

1.  **Clone the repository (if you haven't already):**
    ```bash
    git clone <your-repository-url>
    cd CppAdvancePrograming2
    ```

2.  **Create a build directory and configure CMake:**
    It's recommended to perform an out-of-source build.
    ```bash
    mkdir build
    cd build
    cmake ..
    ```
    This command configures the project and generates the build files (e.g., Makefiles) in the `build` directory.

3.  **Compile the project:**
    After CMake configuration is successful, run your build tool (e.g., Make).
    ```bash
    make 
    ```
    Or, for parallel compilation (faster):
    ```bash
    make -jN # Replace N with the number of cores you want to use, e.g., make -j4
    ```
    This will compile the main application and the tests. The main executable might be located in `build/src/` or `build/` depending on your `CMakeLists.txt` setup (e.g., `tanks_game_lib` or an executable target).

## Running the Application

(Example - Please update with your actual executable details)
```bash
./build/src/YourGameExecutable # Or path/to/your/executable
```
*Please update this section with the actual name and path of your main executable and any command-line arguments it might require.*

## Running Tests

Tests are managed with CTest and Google Test. After building the project:

1.  **Navigate to the build directory (if not already there):**
    ```bash
    cd /path/to/your/project/build
    ```

2.  **Run the tests using CTest:**
    ```bash
    ctest
    ```
    Or, for more verbose output:
    ```bash
    ctest -V
    ```
    This will execute all discovered tests (e.g., `runTests` which includes `MapTest`, etc.). Test results will be displayed in the terminal.

## Cleaning the Build

To remove generated build files, object files, and executables:

1.  **If you are in the `build` directory:**
    ```bash
    make clean
    ```

2.  **To remove the entire `build` directory (for a complete fresh start):**
    ```bash
    cd .. # Go up to the project root from the build directory
    rm -rf build
    ```

---
*This README assumes a standard CMake workflow with Make. If you use a different generator (e.g., Ninja, Visual Studio), the build commands (`make`, `make clean`) will differ from those shown above.*
