#include <iostream>
#include <cstdlib>
#include <string>
#include <memory>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "Utils.h"

// Command-line parsing demo with 'cv::CommandLineParser'
// Ref: https://docs.opencv.org/4.x/d0/d2e/classcv_1_1CommandLineParser.html

// OpenCV Source code on GitHub: 
// Header: https://github.com/opencv/opencv/blob/4.x/modules/core/include/opencv2/core/utility.hpp#L817
// Src:    https://github.com/opencv/opencv/blob/4.x/modules/core/src/command_line_parser.cpp
// Test:   https://github.com/opencv/opencv/blob/4.x/modules/core/test/test_utils.cpp

int main(int argc, char* argv[]) {
        
    auto keys = 
        "{ help h ?     |      | Print this message.                                }"
        "{ mode         |<none>| Must set the working mode.                         }"  // fecth value, must presents
        "{ frame-width  | 640  | Set the frame width to resize to when processing.  }"  // fetch value (has default value)
        "{ fps          | 60   | Set the frame per second for output videos.        }"  // fetch value (has default value)
        "{ write-result |      | Write out processed video (with filename prefix).  }"  // check if presents
        "{ write-input  |      | Write out captured video (with filename prefix).   }"  // check if presents
        "{ from-file    |      | Run from local video file.                         }"  // check if presents,
        "{ file-path    |<none>| If run from file, must give video file's path.     }"  //    then check value
        "{ from-usbcam  |      | Run from USB camera (cv::VideoCapture).            }"  // check if presents,
        "{ cam-id       | 0    | If run from USB camera, may give camera index.     }"  //    then check value
        "{ visual       |      | Use cv::imshow to visualize result (ESC for exit). }"  // check if presents
        "{ stepping     |      | Pause between frames. (ESC for exit; cv::waitkey)  }"  // check if presents
    ;

    // Initialize parser: parsing the options with key definitions
    cv::CommandLineParser parser(argc, argv, keys);

    // Set the about message:
    parser.about("RM-Sensitive OpenCV Command-Line Parser Demo v1.0.0");

    // If the user wants to see HELP message (if 'help' option exists):
    
    if (parser.has("help")) {
        parser.printMessage();  // Print HELP message 
        return 0;               // Then end the program, DO NOTHING
    }

    // STEP 1: Check some OPTIONAL configurations:

    auto with_video = parser.has("from-file");     // Run with video?
    auto with_usbcam = parser.has("from-usbcam");   // Or, run with USB camera? Or run with somthing else?

    auto visual = parser.has("visual");
    auto stepping = parser.has("stepping");
    auto write_result = parser.has("write-result");
    auto write_input = parser.has("write-input");

    std::cout << "[Launch Configurations]:\n";
    std::cout << std::boolalpha;
    std::cout << "- Visualization:  " << visual         << "\n";
    std::cout << "- Stepping:       " << stepping       << "\n";
    std::cout << "- Write result:   " << write_result   << "\n";
    std::cout << "- Write input:    " << write_input    << "\n";
    std::cout << std::noboolalpha;

    // STEP 2: Fetch working mode (must present)

    //    Key 'mode' is annotated with '<node>'.
    //    If its value not given (this call of method returned 
    //    an empty string), an ERROR flag will be set.
    
    auto mode = parser.get<std::string>("mode");

    std::cout << "[Mode]: " << mode << "\n";  // Print out the working mode


    // STEP 3: Fetch some process configuration (with default value)

    //    These calls of method do type conversions. 
    //    An ERROR flag is set if any exception encountered.

    auto frame_width = parser.get<int>("frame-width");
    auto fps = parser.get<double>("fps");

    std::cout << "[Process Frame Width]: " << frame_width << "\n";
    std::cout << "[Frames Per Second]: " << fps << "\n";


    // STEP 4: Fetch output file related configuration (only when presents):

    std::string input_backup_path = "backup.mp4";
    std::string result_video_path = "result.mp4";

    if (write_input) {
        input_backup_path = AppendTimeToFileName(input_backup_path).string();
    }

    if (write_result) {
        result_video_path = AppendTimeToFileName(result_video_path).string();
    }


    std::cout << "[Video Output]\n";

    if (write_input) {
        std::cout << "- Input stream backup path: " << input_backup_path << "\n";
    }
    if (write_result) {
        std::cout << "- Result video output path: " << result_video_path << "\n";
    }
    if (!write_input && !write_result) {
        std::cout << "No video output tasks.\n";
    }


    // STEP 5: Fetch input source related configuration (only when presents, must assign value):
    
    std::string     video_path;     // Input video's path
    int             cam_no = 0;     // Camera index number


    if (with_video) {  // This option overrides other options
        video_path = parser.get<std::string>("file-path");
    }
    else if (with_usbcam) {
        cam_no = parser.get<int>("cam-id");
    }
    else {
        // Going to initial video source from somewhere...
    }


    std::cout << "[Input Source]: ";

    if (with_video) {
        std::cout << "from video '" << video_path << "'\n";    
    } else if (with_usbcam) {
        std::cout << "from USB camera, id = " << cam_no << "\n";
    } else {
        std::cout << "from some other source...? \n";
    }


    // STEP 6: Finish parsing process

    if (!parser.check()) {
        parser.printMessage();
        parser.printErrors();
        return 0;
    }

    // Main Program

    // PART 1: Initialize Video Source

    // One can write a wrapper class of all their possible video sources
    // so that they can access them via a pointer of the base class.

    std::shared_ptr<cv::VideoCapture> p_image_src;

    if (with_video) {
        p_image_src = std::make_shared<cv::VideoCapture>(video_path);
    }
    else if (with_usbcam) {
        p_image_src = std::make_shared<cv::VideoCapture>(cam_no);
    } else {
        std::cout << "Please do some video source initialization here...\n";
        return EXIT_FAILURE;
    }
    
    cv::VideoCapture & src = *p_image_src;
    
    if (!src.isOpened()) {
        std::cerr << "Failed to initialize the video source.\n";
        return EXIT_FAILURE;
    }


    // PART 2: Initialize Video Writer

    std::shared_ptr<cv::VideoWriter> input_writer;
    std::shared_ptr<cv::VideoWriter> result_writer;

    cv::Mat frame;
    
    src >> frame;  // read a sample frame from source

    cv::Size frame_size { frame.cols, frame.rows };

    if (write_input) {
        input_writer = std::make_shared<cv::VideoWriter>(
            input_backup_path,
            cv::VideoWriter::fourcc('A','V','C','1'),
            fps,
            frame_size
        );
    }

    if (write_result) {
        result_writer = std::make_shared<cv::VideoWriter>(
            result_video_path,
            cv::VideoWriter::fourcc('A','V','C','1'),
            fps,
            frame_size
        );
    }


    // PART 3: Initialize Fuction Object

    auto func = cv::cvtColor;

    cv::Mat result;

    // PART 4: Initialize Windows (for visualization)

    auto src_window_name = "src";
    auto result_window_name = "result";

    if (visual) {
        cv::namedWindow(src_window_name, cv::WINDOW_NORMAL);
        cv::namedWindow(result_window_name, cv::WINDOW_NORMAL);
    }

    // PART 5: Process Loop

    while (src.isOpened()) {  // Or while (true) { ... }
        
        // * Read from video stream

        src >> frame;

        if (frame.empty()) break;

        // * Backup input stream

        if (write_input) { (*input_writer) << frame; }
        
        // * Processing

        func(frame, result, cv::COLOR_BGR2GRAY, 1);

        // * Visualization

        if (visual) {
            cv::imshow(src_window_name, frame);
            cv::imshow(result_window_name, result);

            // wait for some time for the image to be shown
            // (if ESC is pressed, break the loop)
            if (cv::waitKey(10) == 27) break; 
        }

        if (stepping) {
            // wait infinitely until key press
            // (if ESC is pressed, break the loop)
            if (cv::waitKey() == 27) break;
        }

        // Write out result

        if (write_result) { (*result_writer) << frame; }
    }
}
