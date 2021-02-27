FROM ubuntu:20.04 as dependencies

ENV DEBIAN_FRONTEND=noninteractive

ENV LDFLAGS=

RUN apt-get update && apt-get install -y \
  bison \
  build-essential \
  flex \
  libgraphviz-dev \
  && rm -rf /var/lib/apt/lists/*

FROM dependencies as builder

WORKDIR /etc/mjavac

COPY . .

RUN make

FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
  graphviz \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /etc/mjavac

COPY --from=builder /etc/mjavac/ .

RUN ln -s /etc/mjavac/mjavac/build/production/mjavac /usr/local/bin/mjavac

ENTRYPOINT ["mjavac"]
