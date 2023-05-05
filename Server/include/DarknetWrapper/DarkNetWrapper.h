#ifndef DARKNETWRAPPER_H
#define DARKNETWRAPPER_H

#include <darknet.h>
#include <QImage>
#include <QDir>
#include <QDebug>

#include <darknet.h>
#include <cstring>
std::string const app_dir = QDir::currentPath().toStdString();

class DarknetWrapper {
    struct ImageInfo {
        int w;
        int h;
        int c;
    };

    image makeImageFromRawData(ImageInfo info, const char *&imageData) {
        image result = make_image(info.w, info.h, info.c);
        memcpy(result.data, imageData,
               info.w * info.h * info.c * sizeof(float));
        return result;
    }

    image makeImageFromRawData(ImageInfo info, const std::string &imageData) {
        return makeImageFromRawData(info, imageData.data());
    }

   public:
    // TODO: we should make this automatic , but for now ,we should copt these
    // files to binary dir(In docker or cmake)
    DarknetWrapper(const std::string &cfgFile = app_dir + "/cfg/yolov3.cfg",
                   const std::string &weightsFile = app_dir + "/yolov3.weights",
                   const std::string &namesChar = app_dir +
                                                  "/data/coco.names") {
        net = load_network(const_cast<char *>(cfgFile.c_str()),
                           const_cast<char *>(weightsFile.c_str()), 0);
        names = get_labels(const_cast<char *>(namesChar.c_str()));
    }

    ~DarknetWrapper() {
        // Release Darknet resources
        free_network(net);
        free_ptrs((void **)names, net->layers[net->n - 1].classes);
    }

    std::vector<std::string> detect(ImageInfo info,
                                    const std::string &imageData) {
        // Run object detection on the specified image data
        image im = makeImageFromRawData(info, imageData);
        float *predictions = network_predict(net, im.data);
        int nboxes = 0;
        detection *dets = get_network_boxes(net, im.w, im.h, thresh,
                                            hier_thresh, 0, 1, &nboxes);
        do_nms_sort(dets, nboxes, net->layers[net->n - 1].classes, nms);
        std::vector<std::string> result;
        for (int i = 0; i < nboxes; ++i) {
            char label[256];
            int cls = max_index(dets[i].prob, net->layers[net->n - 1].classes);
            sprintf(label, "%s: %.2f", names[cls], dets[i].prob[cls]);
            result.push_back(label);
        }
        free_detections(dets, nboxes);
        free_image(im);
        return result;
    }

   private:
    network *net;
    inline static constexpr float thresh = 0.5;
    inline static constexpr float hier_thresh = 0.5;
    inline static constexpr float nms = 0.45;
    char **names;
};

#endif  // DARKNETWRAPPER_H
