FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    qemu \
    clang \
    llvm \
    make \
    qemu-system-aarch64

ENV CLANG_PATH=clang
ENV OBJDUMP_PATH=llvm-objdump
ENV OBJCOPY_PATH=llvm-objcopy
ENV NM_PATH=llvm-nm

CMD ["/bin/bash"]