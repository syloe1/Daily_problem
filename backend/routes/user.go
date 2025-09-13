package routes 
import (
	"keshe-backend/database"
	"keshe-backend/utils"
	"github.com/gin-gonic/gin"
	"golang.org/x/crypto/bcrypt"
	"net/http"
	"time"
)
type RegisterRequest struct {
	StaffID      string `json:"staff_id"`
	Name         string `json:"name"`
	Gender       string `json:"gender"`
	Department   string `json:"department"`
	JoinDate        string `json:"join_date"`
	Password     string `json:"password"`
}
func RegisterUserRoutes(group *gin.RouterGroup) {
	// 所有用户接口都挂载到分组下（最终路径为 /api/user/info、/api/register 等）
	group.GET("/user/info", AuthMiddleware(), GetUserInfo)
	group.POST("/register", Register)       // 最终路径：/api/register
	group.POST("/login", Login)             // 最终路径：/api/login
	group.POST("/user/password", AuthMiddleware(), ChangePassword)
}
func Register(c *gin.Context) {
	var req RegisterRequest 
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "parameter failure"})
		return 
	}
	hash, _ := bcrypt.GenerateFromPassword([]byte(req.Password),bcrypt.DefaultCost)
	_, err := database.DB.Exec(
		"INSERT INTO users (staff_id, name, gender, department, join_date, password_hash) VALUES(?, ?, ?, ?, ?, ?)",
		req.StaffID, req.Name, req.Gender,req.Department, req.JoinDate, string(hash),
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "register failure"})
		return 
	}
	c.JSON(http.StatusOK, gin.H{"message": "register success"})
}

type LoginRequest struct {
	StaffID string `json:"staff_id"`
	Password string `json:"password"`
}
func Login(c *gin.Context) {
	var req LoginRequest 
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "parameter failure"})
		return 
	}
	row := database.DB.QueryRow("SELECT password_hash, role FROM users WHERE staff_id = ?", req.StaffID)
	var passwordHash, role string 
	if err := row.Scan(&passwordHash, &role); err != nil {
		c.JSON(http.StatusUnauthorized, gin.H{"error": "invalid staff_id"})
		return 
	}
	if err := bcrypt.CompareHashAndPassword([]byte(passwordHash), []byte(req.Password)); err != nil {
		c.JSON(http.StatusUnauthorized, gin.H{"error": "invalid password"})
		return 
	}
	token, _ := utils.GenerateToken(req.StaffID, role)
	// 存到 Redis，24小时过期
	database.RDB.Set(database.Ctx, "token:"+req.StaffID, token, 24*time.Hour)
	c.JSON(http.StatusOK, gin.H{"token": token})

}

type PasswordRequest struct {
	OldPassword string `json:"old_password"`
	NewPassword string `json:"new_password"`
}
func ChangePassword(c *gin.Context) {
	staffID := c.GetString("staff_id")
	var req PasswordRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "parameter failure"})
		return 
	}
	row := database.DB.QueryRow("SELECT password_hash FROM users WHERE staff_id = ?", staffID)
	var passwordHash string
	row.Scan(&passwordHash)
	if err := bcrypt.CompareHashAndPassword([]byte(passwordHash), []byte(req.OldPassword)); err != nil {
		c.JSON(http.StatusUnauthorized, gin.H{"error": "invalid old password"})
		return 
	}
	newHash, _ := bcrypt.GenerateFromPassword([]byte(req.NewPassword), bcrypt.DefaultCost)
	database.DB.Exec("UPDATE users SET password_hash = ? WHERE staff_id = ?", string(newHash), staffID)
	c.JSON(http.StatusOK, gin.H{"message": "password changed successfully"})
}
func GetUserInfo(c *gin.Context) {
    staffID := c.GetString("staff_id")
    row := database.DB.QueryRow("SELECT staff_id, name, role FROM users WHERE staff_id = ?", staffID)
    var sid, name, role string
    if err := row.Scan(&sid, &name, &role); err != nil {
        c.JSON(http.StatusInternalServerError, gin.H{"error": "acquire user info failure"})
        return
    }
    c.JSON(http.StatusOK, gin.H{
        "staff_id": sid,
        "name":     name,
        "role":     role,
    })
}