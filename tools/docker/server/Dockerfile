FROM ubuntu:noble AS baseimg
RUN apt-get update && apt-get -y install software-properties-common wget
RUN add-apt-repository "deb http://apt.llvm.org/noble/ llvm-toolchain-noble-20 main"
RUN wget https://apache.jfrog.io/artifactory/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb && apt install -y -V ./apache-arrow-apt-source-latest-$(lsb_release --codename  --short).deb && rm *.deb
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && add-apt-repository "deb http://apt.llvm.org/noble/ llvm-toolchain-noble-20 main"
RUN apt-get update && apt-get -y install python3 python3-venv python3-pip git g++ cmake ninja-build wget unzip ccache curl lsb-release wget zlib1g-dev lcov clang-20 llvm-20 libclang-20-dev llvm-20-dev libmlir-20-dev mlir-20-tools clang-tidy-20 libarrow-dev=19.*  libboost-context1.83-dev
RUN pip3 install --break-system-packages lit
RUN apt install -y -V libarrow-flight-dev=19.*
RUN apt install -y -V libarrow-flight-sql-dev=19.*
RUN git clone https://github.com/moba15/lingo-db.git /lingo-db && cd lingo-db && git checkout 9e76ac9
RUN cd lingo-db && make build-release 
RUN mkdir /database
EXPOSE 8083
VOLUME /database
ENTRYPOINT ["/bin/sh", "-c", "./lingo-db/build/lingodb-release/server 8083 tpch:/database"]
