# ── Stage 1: build GEOS 3.10.7 + compile the simulator ──────────────────────
FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
        cmake g++ make ca-certificates wget bzip2 \
    && rm -rf /var/lib/apt/lists/*

# Build GEOS 3.10.7 as a static library
WORKDIR /tmp/geos-src
RUN wget -q https://download.osgeo.org/geos/geos-3.10.7.tar.bz2 \
    && tar xf geos-3.10.7.tar.bz2

RUN cmake -S geos-3.10.7 -B geos-build \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DBUILD_SHARED_LIBS=OFF \
        -DBUILD_TESTING=OFF \
        -DBUILD_DOCUMENTATION=OFF \
    && cmake --build geos-build -j"$(nproc)" \
    && cmake --install geos-build

# Build the simulator
WORKDIR /simulator
COPY . .

RUN sed -i 's|^PROJ_HOME *=.*|PROJ_HOME = /simulator|' makefile.inc \
    && sed -i 's|^GEOS_HOME *=.*|GEOS_HOME = /usr/local|' makefile.inc

RUN make clean && make && make install

# ── Stage 2: minimal runtime image ───────────────────────────────────────────
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y --no-install-recommends \
        libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /data
COPY --from=build /simulator/Release/simulator /usr/local/bin/simulator
RUN chmod +x /usr/local/bin/simulator

ENTRYPOINT ["/usr/local/bin/simulator"]
