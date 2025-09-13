package routes

import (
	"keshe-backend/database"
	"keshe-backend/utils"
	"github.com/gin-gonic/gin"
	"github.com/golang-jwt/jwt/v5"
	"net/http"
	"time"
)

// AuthMiddleware 校验 JWT + Redis 登录态
func AuthMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		// 预检请求直接放行
		if c.Request.Method == "OPTIONS" {
			c.Next()
			return
		}

		// 获取 Authorization 头
		authHeader := c.GetHeader("Authorization")
		if len(authHeader) < 7 || authHeader[:7] != "Bearer " {
			c.AbortWithStatusJSON(http.StatusUnauthorized, gin.H{"error": "未提供有效的令牌"})
			return
		}

		tokenStr := authHeader[7:]
		claims := &utils.Claims{}

		// 解析 JWT
		token, err := jwt.ParseWithClaims(tokenStr, claims, func(token *jwt.Token) (interface{}, error) {
			return []byte("keshe_secret_key"), nil
		})
		if err != nil || !token.Valid {
			c.AbortWithStatusJSON(http.StatusUnauthorized, gin.H{"error": "未登录或令牌已失效"})
			return
		}

		// Redis 校验 token
		cachedToken, _ := database.RDB.Get(database.Ctx, "token:"+claims.StaffID).Result()
		if cachedToken != tokenStr {
			c.AbortWithStatusJSON(http.StatusUnauthorized, gin.H{"error": "token invalid"})
			return
		}

		// 延长 Redis token 过期时间（保持登录态）
		database.RDB.Expire(database.Ctx, "token:"+claims.StaffID, 24*time.Hour)

		// 将用户信息存入上下文
		c.Set("staff_id", claims.StaffID)
		c.Set("role", claims.Role)

		c.Next()
	}
}

// AdminMiddleware 检查是否为管理员
func AdminMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		role, exists := c.Get("role")
		if !exists || role != "admin" {
			c.AbortWithStatusJSON(http.StatusForbidden, gin.H{"error": "权限不足，需要管理员权限"})
			return
		}
		c.Next()
	}
}
