#include <DarknetWrapper/DarkNetWrapper.h>

DarknetWrapper::DarknetWrapper(
    const std::string &cfgFile, const std::string &weightsFile,
    const std::string &namesChar) {  // /data/coco.names
    net = load_network(const_cast<char *>(cfgFile.c_str()),
                       const_cast<char *>(weightsFile.c_str()), 0);
    names = get_labels(const_cast<char *>(namesChar.c_str()));
}

DarknetWrapper::~DarknetWrapper() {
    // Release Darknet resources
    free_network(net);
    free_ptrs((void **)names, net->layers[net->n - 1].classes);
}

std::vector<DarknetWrapper::SimpleDetectionInfo> DarknetWrapper::detect(
    ImageInfo info, const std::string &imageData) {
    // Run object detection on the specified image data
    image im = makeImageFromRawData(info, imageData);
    float *predictions = network_predict(net, im.data);
    int nboxes = 0;
    detection *dets =
        get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);
    do_nms_sort(dets, nboxes, net->layers[net->n - 1].classes, nms);
    std::vector<SimpleDetectionInfo> result(nboxes, SimpleDetectionInfo{});
    for (int i = 0; i < nboxes; ++i) {
        int cls = max_index(dets[i].prob, net->layers[net->n - 1].classes);
        result[i].label = names[cls];
        result[i].possibility = dets[i].prob[cls];
        {
            result[i].area.x = dets[i].bbox.x;
            result[i].area.y = dets[i].bbox.y;
            result[i].area.w = dets[i].bbox.w;
            result[i].area.h = dets[i].bbox.h;
        }
    }
    free_detections(dets, nboxes);
    free_image(im);
    return result;
}

image DarknetWrapper::makeImageFromRawData(ImageInfo info,
                                           const char *&imageData) {
    image result = make_image(info.w, info.h, info.c);
    memcpy(result.data, imageData, info.w * info.h * info.c * sizeof(float));
    return result;
}

image DarknetWrapper::makeImageFromRawData(ImageInfo info,
                                           const std::string &imageData) {
    return makeImageFromRawData(info, imageData.data());
}
