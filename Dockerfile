# -----------------------------------------------
# R-Visor Artifact Dockerfile
# -----------------------------------------------

    FROM ubuntu:22.04

    # 1. Install system dependencies
    RUN apt-get update && \
        DEBIAN_FRONTEND=noninteractive apt-get install -y \
        build-essential \
        ninja-build \
        cmake \
        git \
        python3 \
        python3-pip \
        wget \
        curl \
        make \
        bison \
        flex \
        gawk \
        texinfo \
        libexpat1-dev \
        libglib2.0-dev \
        libpixman-1-dev \
        libgmp-dev \
        libmpfr-dev \
        libmpc-dev \
        zlib1g-dev \
        qemu-user \
        && apt-get clean
    
    # 2. Install QEMU v7.0.0 from source
    WORKDIR /tmp
    RUN git clone https://github.com/qemu/qemu.git && \
        cd qemu && \
        git checkout v7.0.0 && \
        ./configure --target-list=riscv64-softmmu && \
        make -j$(nproc --ignore=1) && \
        make install && \
        cd / && rm -rf /tmp/qemu
    
    # 3. Install RISC-V GCC Toolchain (riscv64-unknown-linux-gnu-*)
    WORKDIR /tmp
    RUN git clone --recursive https://github.com/riscv/riscv-gnu-toolchain.git && \
        cd riscv-gnu-toolchain && \
        git checkout rvv-0.8 && \
        git submodule update && \
        mkdir -p /opt/riscv && \
        ./configure --prefix=/opt/riscv --with-arch=rv64imafd --with-abi=lp64d && \
        make linux -j$(nproc --ignore=1) && \
        echo 'export PATH=/opt/riscv/bin:$PATH' >> ~/.bashrc && \
        cd / && rm -rf /tmp/riscv-gnu-toolchain
    
    # Add RISC-V tools to path
    ENV PATH="/opt/riscv/bin:$PATH"
    
    # 4. Set working directory for artifact
    WORKDIR /rvisor
    
    # 5. Copy R-Visor source files into the container
    COPY ./comparison ./comparison
    COPY ./embench ./embench
    COPY ./eval ./eval
    COPY ./helpers ./helpers
    COPY ./routines ./routines
    COPY ./src ./src
    COPY ./CMakeLists.txt ./CMakeLists.txt
    
    # 6. Build R-Visor binaries
    RUN cmake . && make -j$(nproc)
    
    # 7. Default entry
    # ENTRYPOINT ["/bin/bash"]
    CMD ["/bin/bash"]