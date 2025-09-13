package main

import (
	"flag"
	"keshe-backend/database"
	"keshe-backend/routes"
	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
	"log"
	"time"
)

func main() {
	// 1. 命令行参数：端口
	port := flag.String("port", "8888", "服务端口")
	flag.Parse()

	// 2. 初始化 MySQL + Redis
	database.InitAll() // 假设你已经把 InitDB 和 InitRedis 封装在 InitAll()

	// 3. 初始化Gin引擎
	gin.SetMode(gin.ReleaseMode)
	r := gin.Default()

	// 4. CORS 中间件
	r.Use(cors.New(cors.Config{
		AllowOrigins:     []string{"http://localhost:5173"},
		AllowMethods:     []string{"GET", "POST", "PUT", "DELETE", "OPTIONS"},
		AllowHeaders:     []string{"Origin", "Content-Type", "Authorization"},
		ExposeHeaders:    []string{"Content-Length"},
		AllowCredentials: true,
		MaxAge:           12 * time.Hour,
	}))

	// 5. API 路由分组
	apiGroup := r.Group("/api")
	routes.RegisterUserRoutes(apiGroup)
	routes.RegisterDepartmentRoutes(apiGroup)
	routes.RegisterAchievementRoutes(apiGroup)

	apiGroup.GET("/ping", func(c *gin.Context) {
		c.JSON(200, gin.H{"message": "pong"})
	})

	// 6. 前端静态资源
	r.Static("/static", "./frontend/build/static")
	r.StaticFile("/", "./frontend/build/index.html")
	r.NoRoute(func(c *gin.Context) {
		c.File("./frontend/build/index.html")
	})

	// 7. 启动服务，使用命令行端口
	log.Printf("service start: http://localhost:%s\n", *port)
	if err := r.Run(":" + *port); err != nil {
		log.Fatalf("service start failure :%v", err)
	}
}
