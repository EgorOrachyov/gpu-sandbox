# gpu sandbox

[![Build](https://github.com/EgorOrachyov/gpu-sandbox/actions/workflows/build.yml/badge.svg)](https://github.com/EgorOrachyov/gpu-sandbox/actions/workflows/build.yml)
[![Clang Format](https://github.com/EgorOrachyov/gpu-sandbox/actions/workflows/clang-format.yml/badge.svg)](https://github.com/EgorOrachyov/gpu-sandbox/actions/workflows/clang-format.yml)
[![Docs C/C++](https://github.com/EgorOrachyov/gpu-sandbox/actions/workflows/docs-cpp.yml/badge.svg)](https://github.com/EgorOrachyov/gpu-sandbox/actions/workflows/docs-cpp.yml)

Sandbox to test various GPU algorithms, image filters, etc. implemented using OpenCL or Cuda.

## Image filters (Naive CPU / OpenCL)

| Flip X                                                                                         | Flip Y                                                                                         |
|:-----------------------------------------------------------------------------------------------|:-----------------------------------------------------------------------------------------------|
| ![iamge](https://raw.githubusercontent.com/EgorOrachyov/gpu-sandbox/main/resources/flip_x.bmp) | ![iamge](https://raw.githubusercontent.com/EgorOrachyov/gpu-sandbox/main/resources/flip_y.bmp) |

| Blur 3/5/7                                                                                   | Motion 9                                                                                       |
|:---------------------------------------------------------------------------------------------|:-----------------------------------------------------------------------------------------------|
| ![iamge](https://raw.githubusercontent.com/EgorOrachyov/gpu-sandbox/main/resources/blur.bmp) | ![iamge](https://raw.githubusercontent.com/EgorOrachyov/gpu-sandbox/main/resources/motion.bmp) |

| Sobel                                                                                          | Grayscale                                                                                    |
|:-----------------------------------------------------------------------------------------------|:---------------------------------------------------------------------------------------------|
| ![iamge](https://raw.githubusercontent.com/EgorOrachyov/gpu-sandbox/main/resources/sobel.bmp)  | ![iamge](https://raw.githubusercontent.com/EgorOrachyov/gpu-sandbox/main/resources/gray.bmp) |

## Building from sources

### Prerequisites

- **Common**:
    - Git (to get source code)
    - CMake (the latest version)
    - Ninja (as build files generator)
    - Python 3.7+
- **Windows 10**:
    - Microsoft Visual C++ Compiler (MSVC) with C++ 17 support
    - x64 Native Tools Command Prompt for VS
- **Ubuntu 20.04**:
    - GNU C++ Compiler with C++ 17 support
- **MaсOS Catalina 10.15**:
    - Clang Compiler with C++ 17 support

### Get source code

The following code snippet downloads project source code repository, and enters project root folder. Must be executed
from the folder where you want to locate project.

```shell
$ git clone https://github.com/EgorOrachyov/gpu-sandbox.git
$ cd gpu-sandbox
```

### Configure and run build

> **Attention!** On Windows platform building commands must be executed in `x64 Native Tools Command Prompt for VS`.

The following code snippet runs `build.py` script, which allows configuring cmake and running of actual build with
selected options. You can specify build directory, build type, number of system threads for build.
Must be executed from project root folder.

```shell
$ python ./build.py --build-dir=build --build-type=Release --nt=4
```

On macOS, you can optionally specify target binaries architecture to build. Pass option `--arch`
with `x86_64` or `arm64` respectively. By default, build falls back to `CMAKE_SYSTEM_PROCESSOR` specified architecture.
See example bellow, replace `<arch>` with desired architecture for your build. Must be executed from project root
folder.

```shell
$ python ./build.py --build-dir=build --build-type=Release --nt=4 --arch=<arch>
```

### Run executable

The following code snippet allows to run executable application of the project to test different algorithms.
Must be executed from the directory used for a build in the previous step.

Print help message to get all possible options.

```shell
$ ./gpusandbox -h
```

List all available filters.

```shell
$ ./gpusandbox -l
```

Apply filter to an image and save result.

```shell
$ ./gpusandbox --filter=<filter name> --input=<image_src>.png --output=<image_dst>.bmp
```

## Contributors

- Egor Orachyov (Github: [@EgorOrachyov](https://github.com/EgorOrachyov))

## Citation

```ignorelang
@online{spla,
  author = {Orachyov, Egor},
  title = {Gpu Sandbox: Sandbox to test various GPU algorithms, image filters, etc. implemented using OpenCL or Cuda},
  year = 2023,
  url = {https://github.com/EgorOrachyov/gpu-sandbox},
  note = {Version 1.0.0}
}
```

## Project structure

| Entry               | Description                                               |
|:--------------------|:----------------------------------------------------------|
| `📁 .github`        | CI/CD scripts and GitHub related files                    |
| `📁 deps`           | Third-party project dependencies, stored as submodules    |
| `📁 resource`       | Example resource images for testing and playing with      |
| `📁 src`            | Private compiled source directory                         |
| `📄 CMakeLists.txt` | CMake configuration, add as sub directory to your project |
| `📄 build.py`       | Script to build project sources to executable             |

## License

This project licensed under MIT License. License text can be found in the
[license file](https://github.com/EgorOrachyov/gpu-sandbox/blob/master/LICENSE.md).
