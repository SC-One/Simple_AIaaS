#include <DarknetWrapper/DarkNetWrapper.h>
#include <QImage>
#include <QProcess>
#include <QBuffer>

QImage convertDarknetImageToQImage(const image &darknetImage) {
    int width = darknetImage.w;
    int height = darknetImage.h;
    int channels = darknetImage.c;

    // Create a QImage with the same dimensions as the Darknet image
    QImage qImage(width, height, QImage::Format_RGBA8888);

    // Convert Darknet image data to QImage pixel values
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Calculate the index in the Darknet image data
            int index = y * width * channels + x * channels;

            // Extract the color channels from the Darknet image
            int red = static_cast<int>(darknetImage.data[index + 0] * 255);
            int green = static_cast<int>(darknetImage.data[index + 1] * 255);
            int blue = static_cast<int>(darknetImage.data[index + 2] * 255);
            int alpha = static_cast<int>(darknetImage.data[index + 3] * 255);

            // Set the corresponding pixel in the QImage
            QRgb pixelValue = qRgba(red, green, blue, alpha);
            qImage.setPixel(x, y, pixelValue);
        }
    }

    return qImage;
}

static image createDarknetImageFromQImage(const QImage &qImage) {
    int width = qImage.width();
    int height = qImage.height();
    int channels = 4;  // Assuming the image has RGBA format

    // Create the Darknet image
    image darknetImage = make_image(width, height, channels);

    // Convert QImage to Darknet image format
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            QColor pixelColor = qImage.pixelColor(x, y);
            darknetImage.data[channels * width * y + channels * x + 0] =
                pixelColor.redF();
            darknetImage.data[channels * width * y + channels * x + 1] =
                pixelColor.greenF();
            darknetImage.data[channels * width * y + channels * x + 2] =
                pixelColor.blueF();
            darknetImage.data[channels * width * y + channels * x + 3] =
                pixelColor.alphaF();
        }
    }

    return darknetImage;
}

static bool validateDarknetImage(const image &darknetImage) {
    // Check image dimensions
    if (darknetImage.w <= 0 || darknetImage.h <= 0 || darknetImage.c <= 0) {
        return false;  // Invalid dimensions
    }

    // Check image data
    if (darknetImage.data == nullptr) {
        return false;  // Invalid data buffer
    }

    // Validate data size
    int expectedSize = darknetImage.w * darknetImage.h * darknetImage.c;
    if (expectedSize <= 0) {
        return false;  // Invalid size
    }

    // TODO: Add additional validation criteria if needed
    // For example, check the data range, data type, etc.

    return true;  // Image is valid
}

static char **load_labels(const char *filename) {
    char **names = nullptr;
    FILE *file = fopen(filename, "r");
    if (file != nullptr) {
        int max_labels = 90;  // Set the maximum number of labels
        int labels_read = 0;
        names = (char **)calloc(max_labels, sizeof(char *));
        char label_name[256];

        while (fgets(label_name, sizeof(label_name), file) != nullptr) {
            // Remove newline character
            int length = strlen(label_name);
            if (label_name[length - 1] == '\n') {
                label_name[length - 1] = '\0';
            }

            // Copy the label name
            names[labels_read] = strdup(label_name);
            labels_read++;

            // Check if more memory is needed for labels
            if (labels_read == max_labels) {
                max_labels *= 2;
                names = (char **)realloc(names, max_labels * sizeof(char *));
            }
        }

        fclose(file);
    }
    return names;
}

DarknetWrapper::DarknetWrapper(
    const std::string &datacfg, const std::string &cfgFile,
    const std::string &weightsFile,
    const std::string &namesChar) {  // /data/coco.names

    //    net = load_network(const_cast<char *>(cfgFile.c_str()),
    //                       const_cast<char *>(weightsFile.c_str()), 0);
    //    names = get_labels(const_cast<char *>(namesChar.c_str()));
    //    //    names = load_labels(namesChar.c_str());
    //    set_batch_network(net, 1);
    //    srand(2222222);
}

DarknetWrapper::~DarknetWrapper() {
    // Release Darknet resources
    free_network(net);
    free_ptrs((void **)names, net->layers[net->n - 1].classes);
}

void saveDarknetImageToFile(const image &darknetImage,
                            const std::string &filePath) {
    // Convert Darknet image to QImage
    QImage qImage = convertDarknetImageToQImage(darknetImage);

    // Save QImage to file using the same format as QImage::save
    qImage.save(QString::fromStdString(filePath), "png");
}

std::vector<DarknetWrapper::SimpleDetectionInfo> DarknetWrapper::detect(
    ImageInfo info, const std::string &imageData) {
    // Run object detection on the specified image data
    return cpp_run_detector(info, imageData);

    image im = makeImageFromRawData(info, imageData);
    image sized = letterbox_image(im, net->w, net->h);
    layer l = net->layers[net->n - 1];

    float *X = sized.data;
    network_predict(net, X);

    int nboxes = 0;
    detection *dets =
        get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);

    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);

    std::vector<SimpleDetectionInfo> result;

    for (int i = 0; i < nboxes; ++i) {
        char labelstr[4096] = {0};
        int cls = -1;
        for (int j = 0; j < l.classes; ++j) {
            if (dets[i].prob[j] > thresh) {
                if (cls < 0) {
                    strcat(labelstr, names[j]);
                    cls = j;
                } else {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j] * 100);
            }
        }
    }

    // Iterate over the detections and convert them to BoundingBox objects
    for (int i = 0; i < nboxes; ++i) {
        int cls = max_index(dets[i].prob, l.classes);
        SimpleDetectionInfo bbox;
        bbox.label = names[cls];
        bbox.possibility = dets[i].prob[cls] * 100;
        bbox.area.x = dets[i].bbox.x;
        bbox.area.y = dets[i].bbox.y;
        bbox.area.w = dets[i].bbox.w;
        bbox.area.h = dets[i].bbox.h;
        result.push_back(bbox);
    }

    free_detections(dets, nboxes);
    free_image(im);
    free_image(sized);

    return result;
}

QImage DarknetWrapper::drawonImage(const std::string &imageData) {
    // so bad idea , but it's faster way to achieve a demo before deadline
    QImage tmp;
    static const QString DIR_DARKNET =
        QString::fromStdString(DarknetWrapper::app_dir + "/darknet");
    static const QString tmpInName = DIR_DARKNET + "/tmp_to_process.jpg";
    static const QString tmpOutName = DIR_DARKNET + "/predictions.jpg";
    tmp.loadFromData(reinterpret_cast<const uchar *>(imageData.data()),
                     imageData.size(), "JPG");
    tmp.save(tmpInName, "JPG");
    QProcess process;
    QString program = "darknet";
    QStringList arguments;
    arguments << "detector"
              << "test"
              << "cfg/coco.data"
              << "cfg/yolov3.cfg"
              << "yolov3.weights" << tmpInName;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.setWorkingDirectory(DIR_DARKNET);
    process.start(program, arguments);
    process.waitForFinished(10e8);
    if (!process.waitForFinished())
        qDebug() << "prediction failed:" << process.errorString();
    else
        qDebug() << "prediction output:" << process.readAll();

    QImage result(tmpOutName);
    return result;
}

image DarknetWrapper::makeImageFromRawData(ImageInfo info,
                                           const std::string &imageData) {
    QImage tmp;
    tmp.loadFromData(reinterpret_cast<const uchar *>(imageData.data()),
                     imageData.size(), "PNG");
    auto result = createDarknetImageFromQImage(tmp);
    return result;
}

std::vector<DarknetWrapper::SimpleDetectionInfo>
DarknetWrapper::cpp_run_detector(ImageInfo info, const std::string &imageData) {
    image im = makeImageFromRawData(info, imageData);
    return test_detector(const_cast<char *>("cfg/coco.data"),
                         const_cast<char *>("cfg/yolov3.cfg"),
                         const_cast<char *>("yolov3.weights"), im, thresh,
                         hier_thresh, 0);
}

std::vector<DarknetWrapper::SimpleDetectionInfo> DarknetWrapper::test_detector(
    char *datacfg, char *cfgfile, char *weightfile, image im, float thresh,
    float hier_thresh, int fullscreen) {
    list *options = read_data_cfg(datacfg);
    char *name_list = option_find_str(options, "names", "data/names.list");
    char **names = get_labels(name_list);

    image **alphabet = load_alphabet();
    network *tmp_net = load_network(cfgfile, weightfile, 0);
    set_batch_network(tmp_net, 1);
    srand(2222222);

    image sized = letterbox_image(im, tmp_net->w, tmp_net->h);
    layer l = tmp_net->layers[tmp_net->n - 1];
    float *X = sized.data;
    network_predict(tmp_net, X);
    int nboxes = 0;
    detection *dets = get_network_boxes(tmp_net, im.w, im.h, thresh,
                                        hier_thresh, 0, 1, &nboxes);
    if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
    return extr_detections(im, dets, nboxes, thresh, names, alphabet,
                           l.classes);
}

std::vector<DarknetWrapper::SimpleDetectionInfo>
DarknetWrapper::extr_detections(image im, detection *dets, int num,
                                float thresh, char **names, image **alphabet,
                                int classes) {
    std::vector<DarknetWrapper::SimpleDetectionInfo> result;
    for (int i = 0; i < num; ++i) {
        //        char labelstr[4096] = {0};
        int cls = -1;
        for (int j = 0; j < classes; ++j) {
            auto prob = dets[i].prob[j];
            if (prob > thresh) {
                if (cls < 0) {
                    //                    strcat(labelstr, names[j]);
                    cls = j;
                } else {
                    //                    strcat(labelstr, ", ");
                    //                    strcat(labelstr, names[j]);
                }
                //                printf("%s: %.0f%%\n", names[j],
                //                dets[i].prob[j] * 100);
            }
        }

        if (cls >= 0) {
            box b = dets[i].bbox;
            DarknetWrapper::SimpleDetectionInfo tmp;
            tmp.area.x = b.x;
            tmp.area.y = b.y;
            tmp.area.w = b.w;
            tmp.area.h = b.h;
            tmp.label = names[cls];
            tmp.label = dets[i].prob[cls] * 100;
        }
    }
    return result;
}
