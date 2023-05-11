# https://hub.docker.com/r/societycleaner/ubuntu_qt5.15_grpc1.54.1/tags
FROM societycleaner/ubuntu_qt5.15_grpc1.54.1:latest
RUN sudo apt-get update
RUN sudo apt-get install build-essential autoconf libtool pkg-config git cmake wget -y

RUN mkdir -p APP
WORKDIR APP
RUN mkdir -p my_build 
RUN git clone -b Server_right_rule_on_github_hooks https://github.com/SC-One/Simple_AIaaS.git
WORKDIR Simple_AIaaS
RUN git submodule update --init --recursive
RUN mkdir -p Server/include/proto_out
WORKDIR protos
RUN export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARYPATH
RUN LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARYPATH protoc --cpp_out=./../Server/include/proto_out --grpc_out=./../Server/include/proto_out --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` image.proto
WORKDIR ../../my_build
RUN cmake ../Simple_AIaaS/Server/
RUN export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARYPATH
RUN make -j`nproc`
RUN wget https://pjreddie.com/media/files/yolov3.weights
RUN export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARYPATH
RUN sudo ldconfig
CMD ./Server
# please use -p 50777:50777 when running docker image
