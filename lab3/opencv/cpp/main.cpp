#include <ctime>
#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

int waitKeyTime = 1;
size_t frames_to_analyse = 300;

struct time_range {
    timespec start, end;
};

double get_all(const std::vector<time_range> &tmp) {
    double rez = 0;
    for (auto range: tmp) {
        rez += (range.end.tv_sec - range.start.tv_sec) + 1e-9 * (range.end.tv_nsec - range.start.tv_nsec);
    }
    return rez;
}

int firstVideoTest(const std::string &FileOutName) {
    cv::VideoCapture video(0);

    if (!video.isOpened()) {
        std::cerr << "Ошибка при открытии видео!" << std::endl;
        return -1;
    }

    cv::Mat img;

    std::vector<time_range> reading_range;
    std::vector<time_range> showing_range;
    std::vector<time_range> frame_range;
    frame_range.resize(frames_to_analyse);
    showing_range.resize(frames_to_analyse);
    reading_range.resize(frames_to_analyse);
    time_range total_time_range{0, 0};

    clock_gettime(CLOCK_MONOTONIC_RAW, &total_time_range.start);
    for (int frame_num = 0; frame_num < frames_to_analyse; frame_num++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &frame_range[frame_num].start);

        clock_gettime(CLOCK_MONOTONIC_RAW, &reading_range[frame_num].start);
        video.read(img);
        clock_gettime(CLOCK_MONOTONIC_RAW, &reading_range[frame_num].end);

        clock_gettime(CLOCK_MONOTONIC_RAW, &showing_range[frame_num].start);
        imshow("Hello from Russia", img);
        clock_gettime(CLOCK_MONOTONIC_RAW, &showing_range[frame_num].end);


        if (cv::waitKey(waitKeyTime) == 27) {// Нажмите Esc, чтобы выйти из программы
            break;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &frame_range[frame_num].end);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &total_time_range.end);


    double frame_avg = get_all(frame_range) / frames_to_analyse;
    double total_time = (total_time_range.end.tv_sec - total_time_range.start.tv_sec)
                        + 1e-9 * (total_time_range.end.tv_nsec - total_time_range.start.tv_nsec);
    double showing_avg = get_all(showing_range) / frames_to_analyse;
    double reading_avg = get_all(reading_range) / frames_to_analyse;

    video.release();
    cv::destroyAllWindows();
    std::ofstream outStream{FileOutName};
    outStream << "total time       : " << total_time << "\n";
    outStream << "frames per second: " << frames_to_analyse / total_time << "\n";
    outStream << "frame time       : " << frame_avg << "\n";
    outStream << "showing time     : " << showing_avg << "\n";
    outStream << "reading time     : " << reading_avg << "\n";

    return 0;
}


int secondVideoTest(const std::string &FileOutName) {
    cv::VideoCapture video(0);

    if (!video.isOpened()) {
        std::cerr << "Ошибка при открытии видео!" << std::endl;
        return -1;
    }
    cv::Mat frame;
    cv::CascadeClassifier eyeCascade;
    if (!eyeCascade.load("haarcascade_eye.xml")) {
        std::cerr << "Не удалось загрузить каскад для глаз!" << std::endl;
        return -1;
    }

    std::vector<time_range> frame_range;
    std::vector<time_range> reading_range;
    std::vector<time_range> filter_range;
    std::vector<time_range> showing_range;
    time_range total_time_range{0, 0};

    frame_range.resize(frames_to_analyse);
    reading_range.resize(frames_to_analyse);
    filter_range.resize(frames_to_analyse);
    showing_range.resize(frames_to_analyse);


    clock_gettime(CLOCK_MONOTONIC_RAW, &total_time_range.start);
    for (int frame_num = 0; frame_num < frames_to_analyse; frame_num++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &frame_range[frame_num].start);

        clock_gettime(CLOCK_MONOTONIC_RAW, &reading_range[frame_num].start);
        video.read(frame);
        clock_gettime(CLOCK_MONOTONIC_RAW, &reading_range[frame_num].end);


        std::vector<cv::Rect> eyes;

        clock_gettime(CLOCK_MONOTONIC_RAW, &filter_range[frame_num].start);
        // Заменяем области глаз на белый цвет
        eyeCascade.detectMultiScale(frame, eyes);
        for (const cv::Rect &eye: eyes) {
            // Вычисляем координаты центра глаза и радиус
            cv::Point center(eye.x + eye.width / 2, eye.y + eye.height / 2);
            int radius = eye.width / 2;

            cv::circle(frame, center, radius, cv::Scalar(255, 255, 255), cv::FILLED);
            cv::circle(frame, center, radius, cv::Scalar(255, 0, 0), 3);
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &filter_range[frame_num].end);


        clock_gettime(CLOCK_MONOTONIC_RAW, &showing_range[frame_num].start);
        cv::imshow("Circles on Eyes", frame);
        clock_gettime(CLOCK_MONOTONIC_RAW, &showing_range[frame_num].end);

        if (cv::waitKey(waitKeyTime) == 27) {// Нажмите Esc, чтобы выйти из программы
            break;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &frame_range[frame_num].end);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &total_time_range.end);

    double frame_avg = get_all(frame_range) / frames_to_analyse;
    double filter_avg = get_all(filter_range) / frames_to_analyse;
    double showing_avg = get_all(showing_range) / frames_to_analyse;
    double reading_avg = get_all(reading_range) / frames_to_analyse;
    double total_time = (total_time_range.end.tv_sec - total_time_range.start.tv_sec)
                        + 1e-9 * (total_time_range.end.tv_nsec - total_time_range.start.tv_nsec);

    video.release();
    cv::destroyAllWindows();

    std::ofstream outStream{FileOutName};
    outStream << "total time       : " << total_time << "\n";
    outStream << "frames per second: " << frames_to_analyse / total_time << "\n";
    outStream << "frame time       : " << frame_avg << "\n";
    outStream << "filter time      : " << filter_avg << "\n";
    outStream << "showing time     : " << showing_avg << "\n";
    outStream << "reading time     : " << reading_avg << "\n";
    return 0;
}

int main() {
//    firstVideoTest("out_1.txt");
    secondVideoTest("out_2.txt");
    return 0;
}
