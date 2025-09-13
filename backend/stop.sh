#!/bin/bash
# 文件名: stop_keshe.sh
# 功能: 关闭 keshe_backend 后端服务

EXEC_FILE="keshe_backend"        # 后端可执行文件名
START_PORT=8888
INSTANCE_COUNT=10

echo "正在关闭 $INSTANCE_COUNT 个后端实例..."

for i in $(seq 0 $((INSTANCE_COUNT-1))); do
    PORT=$((START_PORT + i))
    # 找到对应端口的进程并杀掉
    PID=$(lsof -ti tcp:$PORT)
    if [ -n "$PID" ]; then
        echo "关闭端口 $PORT 的进程 PID=$PID ..."
        kill -9 $PID
    else
        echo "端口 $PORT 没有运行的进程"
    fi
done

echo "所有指定端口的后端实例已关闭。"

