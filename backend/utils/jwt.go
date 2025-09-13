package utils

import (
	"keshe-backend/database"
	"github.com/golang-jwt/jwt/v5"
	"time"
)

var jwtkey = []byte("keshe_secret_key")

type Claims struct {
	StaffID string `json:"staff_id"`
	Role    string `json:"role"`
	jwt.RegisteredClaims
}

// GenerateToken 生成 JWT 并写入 Redis，保持登录态
func GenerateToken(staffID, role string) (string, error) {
	claims := &Claims{
		StaffID: staffID,
		Role:    role,
		RegisteredClaims: jwt.RegisteredClaims{
			ExpiresAt: jwt.NewNumericDate(time.Now().Add(24 * time.Hour)),
			IssuedAt:  jwt.NewNumericDate(time.Now()),
		},
	}

	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	tokenStr, err := token.SignedString(jwtkey)
	if err != nil {
		return "", err
	}

	// 写入 Redis，过期时间 24 小时
	// 写入 Redis，过期时间 24 小时
	database.RDB.Set(database.Ctx, "token:"+staffID, tokenStr, 24*time.Hour)


	return tokenStr, nil
}
