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
$ docker-compose up -d
```

## Run xserver
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
```

## Run app
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


### Run maker generator
Example
```
$ ./marker_generator -t 1 -o ../result
```
