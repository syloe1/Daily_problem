#!/bin/bash
# 文件名: start_all.sh
# 功能: 停止旧实例 → 编译 Go → 启动 10 个后端实例 → 生成 Nginx 配置 → 提示压测

# ================= 配置 =================
EXEC_FILE="./keshe_backend"      # 可执行文件
LOG_DIR="./logs"                 # 日志目录
START_PORT=8888
INSTANCE_COUNT=10
NGINX_CONF="/etc/nginx/sites-available/keshe.conf"
SERVER_NAME="localhost"          # Nginx server_name，可改为IP或域名
API_TEST_URL="http://localhost/api/ping"
TOKEN="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9..."  # 替换为你的JWT

# ================= 函数 =================
stop_instances() {
    echo "关闭旧实例..."
    for i in $(seq 0 $((INSTANCE_COUNT-1))); do
        PORT=$((START_PORT + i))
        PID=$(lsof -ti tcp:$PORT)
        if [ -n "$PID" ]; then
            kill -9 $PID
            echo "关闭端口 $PORT 的进程 PID=$PID"
        fi
    done
}

compile_go() {
    echo "编译 Go 程序..."
    go build -o $EXEC_FILE main.go
    if [ $? -ne 0 ]; then
        echo "Go 编译失败！"
        exit 1
    fi
}

start_instances() {
    mkdir -p "$LOG_DIR"
    echo "启动 $INSTANCE_COUNT 个后端实例..."
    for i in $(seq 0 $((INSTANCE_COUNT-1))); do
        PORT=$((START_PORT + i))
        echo "启动端口 $PORT ..."
        "$EXEC_FILE" --port=$PORT > "$LOG_DIR/$PORT.log" 2>&1 &
    done
}

generate_nginx() {
    echo "生成 Nginx 配置..."
    sudo bash -c "cat > $NGINX_CONF <<EOL
upstream gin_backend {
EOL"

    for i in $(seq 0 $((INSTANCE_COUNT-1))); do
        PORT=$((START_PORT + i))
        sudo bash -c "echo '    server 127.0.0.1:$PORT;' >> $NGINX_CONF"
    done

    sudo bash -c "cat >> $NGINX_CONF <<EOL
}

server {
    listen 80;
    server_name $SERVER_NAME;

    location / {
        proxy_pass http://gin_backend;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
    }
}
EOL"

    echo "重载 Nginx 配置..."
    sudo nginx -s reload
}

pressure_test() {
    echo "开始压力测试 (10k请求, 1000并发)..."
    hey -n 10000 -c 1000 -H "Authorization: Bearer $TOKEN" $API_TEST_URL
}

# ================= 主流程 =================
stop_instances
compile_go
start_instances
generate_nginx
echo "启动完成！后端服务已运行，Nginx 已重载。"
# 压测可选，取消注释执行
# pressure_test

