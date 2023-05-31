#ifndef DARKNETWRAPPER_H
#define DARKNETWRAPPER_H

#include <darknet.h>
#include <QImage>
#include <QDir>
#include <QDebug>

#include <darknet.h>
#include <cstring>

class DarknetWrapper {
   public:
    static inline std::string const app_dir = QDir::currentPath().toStdString();
    struct ImageInfo {
        int w;
        int h;
        int c;
    };

    struct DetectionArea {
        int x, y, w, h;
    };
    struct SimpleDetectionInfo {
        DetectionArea area;
        std::string label;
        int possibility;
    };

    // TODO: we should make this automatic , but for now ,we should copt these
    // files to binary dir(In docker or cmake)
    DarknetWrapper(
        const std::string &datacfg = app_dir + "/coco.data",
        const std::string &cfgFile = app_dir +
                                     "/yolov3.cfg",  // /cfg/yolov3.cfg
        const std::string &weightsFile =
            app_dir + "/yolov3.weights",  // smaller: yolov3-tiny.weights
        const std::string &namesChar = app_dir + "/coco.names");

    ~DarknetWrapper();

    std::vector<SimpleDetectionInfo> detect(ImageInfo info,
                                            const std::string &imageData);
    QImage drawonImage(const std::string &imageData);

   private:
    image makeImageFromRawData(ImageInfo info, const std::string &imageData);

    // helpers
    std::vector<DarknetWrapper::SimpleDetectionInfo> cpp_run_detector(
        ImageInfo info, const std::string &imageData);
    std::vector<DarknetWrapper::SimpleDetectionInfo> test_detector(
        char *datacfg, char *cfgfile, char *weightfile, image im, float thresh,
        float hier_thresh, int fullscreen);

    std::vector<DarknetWrapper::SimpleDetectionInfo> extr_detections(
        image im, detection *dets, int num, float thresh, char **names,
        image **alphabet, int classes);

    // this is not good solution , just bypassing deadline of demo
    void lastTrick() {}

   private:
    network *net;
    inline static constexpr float thresh = 0.5;
    inline static constexpr float hier_thresh = 0.5;
    inline static constexpr float nms = 0.45;
    char **names;
};

#endif  // DARKNETWRAPPER_H
