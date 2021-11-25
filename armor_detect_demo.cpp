#include "MVCameraInput.h"
#include "VideoInput.h"
#include "ArmorDetect.h"

int main(int argc, char *argv[]) {

    std::string filename;

    std::shared_ptr<ImageInput> video;

    if (argc > 1) {
        filename = std::string(argv[1]);
        std::cout << "Opening file: " << filename << std::endl;
        video = std::make_shared<VideoInput>(filename);
    } else {
        video = std::make_shared<MVCameraInput>();
    }

    ImageInput & cap = *video;

    if (!cap) return EXIT_FAILURE;

    cv::namedWindow("frame");
    cv::namedWindow("show");

    cv::Mat frame;
    cap >> frame;
    std::cout << frame.size();
    
    cv::waitKey(0);

    ArmorDetect inst(ArmorDetect::Mode::FIND_BLUE);

    while (true) {
        cap >> frame;

        // frame: original video source
        cv::imshow("frame", frame);

        cv::Mat show;
        frame.copyTo(show);

        // get target
        auto aimArea = inst.process(frame);

        // draw all light bars in frame
        for (const auto &bar : inst.getLightBars()) {
            cv::Point2f pts[4];
            bar.box.points(pts);
            // Draw with green lines
            drawTetragon(show, pts, cv::Scalar(0, 255, 0));
        }

        if (aimArea.empty()) {
            std::cout << "No target found.\n";
        } else {
            drawTetragon(show, aimArea.data(), cv::Scalar(255, 255, 255));
        }

        // draw all armors
        for (const auto& armor : inst.getResults()) {
            cv::Point2f pts[4];
            armor.points(pts);
            drawTetragon(show, pts, cv::Scalar(0,255,0));
        }

        cv::imshow("show", show);

        // view frame by frame
        if (cv::waitKey(0) == 27) break;
    }
}
