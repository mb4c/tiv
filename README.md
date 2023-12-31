# tiv

tiv is an image viewer that allows you to view gigapixel images
![img.png](img.png)
## Getting Started

To get tiv you need to build it yourself

### Dependencies
- glfw3 
- glm
- OpenImageIO

### How to build

#### Clone
```shell
git clone https://github.com/mb4c/tiv
```
#### Build
```shell
cmake -S . -B build
cd build
make -j$(nproc)
```
#### Install
```shell
cmake --install .
```

## Usage

You can view images by dropping them on the window or by passing them as argument
```shell
  tiv example.png
```

## Example images
[Curiosity's 1.8-Billion-Pixel Panorama](https://photojournal.jpl.nasa.gov/catalog/PIA23623)
## License
This project is licensed under the GPLv3 License. See the LICENSE file for details.