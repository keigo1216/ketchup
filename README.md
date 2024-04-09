# ketchup

## Description
raspberrypi 3A+で動く自作OSです.
基本は`quem`エミュレータ上で開発しながら, たまに実機でテストします.

## How to run
```
make
make run
```

## Run on Docker
dockerで作った仮想環境も配布していす（ただし, qemuのバージョンが古いので正常に動作しない可能性があります）
```
cd docker
docker-compose up -d --build
```

## requirement (WSL2)
setup enviroment
```
sudo apt update
sudo apt install -y clang llvm lld qemu-system-aarch64 curl
sudo apt install make

export CLANG_PATH=/usr/bin/clang
export OBJDUMP_PATH=/usr/bin/llvm-objdump
export OBJCOPY_PATH=/usr/bin/llvm-objcopy
export NM_PATH=/usr/bin/llvm-nm
```

## requirement (Macbook)
- qemu : 8.1.3
- clang : Apple clang version 15.0.0 (clang-1500.1.0.2.5)
- llvm