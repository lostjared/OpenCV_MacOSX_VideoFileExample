
#include<opencv2/opencv.hpp>
#include<iostream>
#include<cstdlib>

void procFrame(cv::Mat &frame);

int main(int argc, char **argv) {
    
    if(argc != 3) {
        std::cerr << "Error invalid arguments..\n";
        exit(EXIT_FAILURE);
    }
    
    cv::VideoCapture cap(argv[1]);
    if(!cap.isOpened()) {
        std::cerr << "Error requires file to be able to be opened.\n";
        exit(EXIT_FAILURE);
    }
    
    cv::VideoWriter writer;
    int aw = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
    int ah = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CV_CAP_PROP_FPS);
    writer.open(argv[2], CV_FOURCC('m', 'p', '4', 'v'), fps, cv::Size(aw, ah), true);
    if(!writer.isOpened()) {
        std::cerr << "Error could not open output video: " << argv[2] << "\n";
    }
    bool active = true;
    cv::namedWindow("CVTest");
    
    while(active) {
        cv::Mat frame;
        if(cap.read(frame) == false) {
            active = false;
            break;
        }
        procFrame(frame);
        cv::imshow("CVTest", frame);
        writer.write(frame);
        unsigned int key = cv::waitKey(25);
        if(key == 'q') {
            active = false;
            break;
        }
    }
    std::cout << "Wrote to file: " << argv[2] << "\n";
    return 0;
}


void procFrame(cv::Mat &frame) {
    unsigned int w = frame.cols;
    unsigned int h = frame.rows;
    static double pos = 1.0;
    for(unsigned int x = 0; x < w; ++x) {
        for(unsigned int y = 0; y < h; ++y) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
            for(unsigned int q = 0; q < 3; ++q) {
                pixel[q] += static_cast<unsigned char>(pixel[q]*pos);
            }
        }
    }
    static int direction = 1;
    if(direction == 1) {
        pos += 0.01;
        if(pos > 7.0) {
            direction = 0;
        }
    } else {
        pos -= 0.01;
        if(pos <= 1) {
            direction = 1;
        }
    }
}
