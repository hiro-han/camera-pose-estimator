# camera-pose-estimator

# Set up environment
## Clone
```
$ git clone {repository}
$ cd camera-pose-estimator
$ git submodule update -i

```

## Build docker image
```
$ docker-compose build
```

# Run container
## Run xserver
host pc:

```
$ xhost local:
```

## Run docker
```
$ docker-compose run camera-pose-estimator /bin/bash
```

# Run application
## Build app
```
$ cd app
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Run app
```
$ cd app/bin
```

### Run maker generator
Example
```
$ ./marker_generator -i ../marker-gen-config.json -o ../result --type 4
```

### Run camera calibrator
args
| param | required | description | 
|-|-|-|
| -m | x | c : CameraCalibration, p : PoseEstimation |
| -v | x | Input video |
| -o | x | Output directory |
| --oi | - | Output detected image |

Example:
```
./CameraPoseEstimator -m c -v ../test/data/checker-video.mp4 -o ../result -oi 1
```

### Run pose estimator

```
./CameraPoseEstimator -m p -v ../test/data/marker.mp4 -c ../marker-config.json -o ../result -oi 1
```

# config
## marker-gen-config.json

```
{
  "marker": {
    "marker_length": 200    # [pixel]
  },
  "board": {
    "row": 5,               # num of row
    "col": 7,               # num of col
    "marker_length": 0.04   # [m]
    "marker_interval": 0.02 # [m]
    "width": 600,           # image width [pixel]
    "height": 500,          # image height [pixel]
    "margin": 10,           # interval of marker [pixel]
  },
  "chess-board": {
    "row": 5,               # num of row
    "col": 7,               # num of col
    "square_length": 0.04,  # [m]
    "marker_length": 0.02   # [m]
    "width": 600,           # image width [pixel]
    "height": 500,          # image height [pixel]
    "margin": 10,           # interval of marker [pixel]
  },
  "diamond": {
    "square_length": 200, # [pixel]
    "marker_length": 120  # [pixel]
  }
}
```

### marker-config.json
```
{
  "marker": {
    "marker_size": 0.05      # marker size [m]
  },
  "board": {
    "row": 5,                # num of row
    "col": 7,                # num of col
    "marker_size": 0.04,     # marker size [m]
    "marker_interval": 0.02, # interval of marker [m]
    "margin": 10             # margins of the board [pixel]
  },
  "chess-board": {
    "row": 5,             # num of row
    "col": 7,             # num of col
    "square_length": 0.04, # [m]
    "marker_length": 0.02  # [m]
  },
  "diamond": {
    "square_length": 0.04, # [m]
    "marker_length": 0.02  # [m]
  }
}
```