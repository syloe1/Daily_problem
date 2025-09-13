#!/bin/bash
# 文件名: q.sh
# 功能: 编译 Go 程序、启动 10 个后端实例、生成 Nginx 配置

# 配置
EXEC_FILE="./keshe_backend"        # 可执行文件路径
LOG_DIR="./logs"                   # 日志目录
START_PORT=8888
INSTANCE_COUNT=10
NGINX_CONF="/etc/nginx/sites-available/keshe.conf"
SERVER_NAME="your_server_ip"       # 修改为服务器 IP 或域名
GO_BIN=$(which go)                 # 获取 Go 可执行路径

# 1. 创建日志目录
mkdir -p "$LOG_DIR"

# 2. 编译 Go 程序
echo "编译 Go 程序..."
if ! $GO_BIN build -o $EXEC_FILE main.go; then
    echo "Go 编译失败！请确认 Go 已安装且环境变量正确"
    exit 1
fi

# 3. 启动实例
echo "启动 $INSTANCE_COUNT 个后端实例..."
for i in $(seq 0 $((INSTANCE_COUNT-1))); do
    PORT=$((START_PORT + i))
    echo "启动端口 $PORT ..."
    "$EXEC_FILE" --port=$PORT > "$LOG_DIR/$PORT.log" 2>&1 &
done

# 4. 生成 Nginx 配置（需要 sudo）
echo "生成 Nginx 配置..."
sudo bash -c "cat > $NGINX_CONF <<EOL
upstream gin_backend {
EOL
"

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
EOL
"

# 5. 提示用户重载 Nginx
echo "启动完成！请执行：sudo nginx -s reload 重载 Nginx 配置"

