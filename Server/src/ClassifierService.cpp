#include <Server/ClassifierService.h>
#include <QBuffer>

namespace DarkNetServer {
using Detections = std::vector<DarknetWrapper::SimpleDetectionInfo>;

grpc::Status ClassifierServiceImpl::classifyshot(
    grpc::ServerContext *context, const sc::image::Image *request,
    sc::image::Detections *response) {
    {
        Detections tmpResult = _network.detect(
            {request->width(), request->height(), request->channels()},
            request->data());
        for (auto const &item : tmpResult) {
            auto detected = response->add_detections();
            detected->set_label(item.label);
            detected->set_possibility(item.possibility);

            auto tmp = new sc::image::Area();
            {
                tmp->set_x(item.area.x);
                tmp->set_y(item.area.y);
                tmp->set_w(item.area.w);
                tmp->set_h(item.area.h);
                detected->set_allocated_area(tmp);
            }
        }
    }
    return grpc::Status::OK;
}

grpc::Status ClassifierServiceImpl::drawClassifiedImage(
    grpc::ServerContext *context, const sc::image::Image *request,
    sc::image::Image *response) {
    auto drawed = _network.drawonImage(request->data());
    {
        QByteArray imageDataArray;
        QBuffer buffer(&imageDataArray);
        buffer.open(QIODevice::WriteOnly);
        drawed.save(&buffer, "JPG");

        response->set_data(imageDataArray.constData(), imageDataArray.size());
    }
    return grpc::Status::OK;
}

}  // namespace DarkNetServer
