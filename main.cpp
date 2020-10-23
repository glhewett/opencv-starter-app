#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

const cv::String keys =
    "{ help usage h    |            | print out help message                 }"
    "{ webcam-device   |          0 | which camera device to use             }"
    "{ image           |            | load an image instead                  }";

int main(int argc, char **argv) {
  cv::CommandLineParser parser(argc, argv, keys);
  parser.about("opencv-starter-app v0.0.1");
  cv::namedWindow("WebCam", cv::WINDOW_AUTOSIZE);
  cv::Mat image;
  bool capturing = true;
  bool load_image = false;
  cv::String image_path;

  if (!parser.check()) {
    parser.printErrors();
    parser.printMessage();
    return 1;
  }

  if (parser.has("help")) {
    parser.printMessage();
    return 0;
  }
  int device_id = parser.get<int>("webcam-device");
  cv::VideoCapture cap(device_id);

  if (parser.has("image")) {
    load_image = true;
    cv::String image_path = parser.get<cv::String>("image");
  }

  // load the image and don't try an check the device if we are not going to
  // use it.
  if (load_image) {
    image = cv::imread(image_path, cv::IMREAD_COLOR);
  } else {

    // confirm that the video capture device is open.
    if (!cap.isOpened()) {
      std::cerr << "Could not open webcam." << std::endl;
      return 1;
    }
  }

  while (capturing) {

    // load the image from the camera
    if (!load_image) {

      // capture the image on the camera
      if (!cap.grab()) {
        std::cerr << "Error grabbing image." << std::endl;
        continue;
      }

      // retrieve/decode the image and copy into cv::Mat
      if (!cap.retrieve(image, cv::CAP_ANY)) {
        std::cerr << "Error retrieving image." << std::endl;
        continue;
      }
    }
    cv::imshow("WebCam", image);
    int key_code = cv::waitKey(250);

    switch (key_code) {
    case -1: // NOOP
      break;

    case 27: // ESC
      capturing = false;
      break;
    }
  }
  cap.release();
  return 0;
}

