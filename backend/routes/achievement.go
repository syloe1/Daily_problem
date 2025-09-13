package routes

import (
	"encoding/json"
	"keshe-backend/database"
	"net/http"
	"os"
	"path/filepath"
	"strconv"
	"time"

	"github.com/gin-gonic/gin"
)

type Achievement struct {
	ID             int    `json:"id"`
	Type           string `json:"type"`
	Subtype        string `json:"subtype"`
	Participants   string `json:"participants"`
	RankOrder      int    `json:"rank_order"`
	FilePath       string `json:"file_path"`
	Status         string `json:"status"`
	ReviewComments string `json:"review_comments"`
	CreatedAt      string `json:"created_at"`
	UpdatedAt      string `json:"updated_at"`
}

type ReviewRequest struct {
	Status  string `json:"status"`
	Comment string `json:"comment"`
}

func RegisterAchievementRoutes(group *gin.RouterGroup) {
	// 教职工接口
	achGroup := group.Group("/achievements", AuthMiddleware())
	{
		achGroup.GET("", GetAchievements)
		achGroup.POST("", CreateAchievement)
		achGroup.PUT("/:id", UpdateAchievement)
		achGroup.DELETE("/:id", DeleteAchievement)
	}

	// 管理员审核接口
	adminGroup := group.Group("/achievements", AuthMiddleware(), AdminMiddleware())
	{
		adminGroup.GET("/pending", GetPendingAchievements)
		adminGroup.POST("/:id/review", ReviewAchievement)
	}

	// 管理员统计接口
	statGroup := group.Group("/achievements/statistics", AuthMiddleware(), AdminMiddleware())
	{
		statGroup.GET("/type", StatisticsByType)
		statGroup.GET("/user", StatisticsByUser)
		statGroup.GET("/time", StatisticsByTime)
	}
}

// -------------------- 教职工接口 --------------------

// 查询列表
func GetAchievements(c *gin.Context) {
	rows, err := database.DB.Query(
		"SELECT id, type, subtype, participants, rank_order, file_path, status, review_comments, created_at, updated_at FROM achievements",
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "query failure"})
		return
	}
	defer rows.Close()

	var achievements []Achievement
	for rows.Next() {
		var a Achievement
		if err := rows.Scan(&a.ID, &a.Type, &a.Subtype, &a.Participants, &a.RankOrder, &a.FilePath, &a.Status, &a.ReviewComments, &a.CreatedAt, &a.UpdatedAt); err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "scan failure"})
			return
		}
		achievements = append(achievements, a)
	}

	c.JSON(http.StatusOK, achievements)
}

// 创建成果
func CreateAchievement(c *gin.Context) {
	typeReq := c.PostForm("type")
	subtypeReq := c.PostForm("subtype")
	participants := c.PostForm("participants")
	rankOrder, _ := strconv.Atoi(c.PostForm("rank_order"))

	file, err := c.FormFile("file")
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "file required"})
		return
	}

	ext := filepath.Ext(file.Filename)
	dir := "./uploads/pdf"
	if ext == ".md" {
		dir = "./uploads/md"
	}

	if err := os.MkdirAll(dir, os.ModePerm); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "cannot create dir"})
		return
	}

	filePath := filepath.Join(dir, strconv.FormatInt(time.Now().UnixNano(), 10)+ext)
	if err := c.SaveUploadedFile(file, filePath); err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "save file failure"})
		return
	}

	_, err = database.DB.Exec(
		"INSERT INTO achievements (type, subtype, participants, rank_order, file_path, status, review_comments) VALUES (?, ?, ?, ?, ?, '待审核', '')",
		typeReq, subtypeReq, participants, rankOrder, filePath,
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "insert failure"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "achievement created"})
}

// 修改成果（不改文件）
func UpdateAchievement(c *gin.Context) {
	id, _ := strconv.Atoi(c.Param("id"))
	typeReq := c.PostForm("type")
	subtypeReq := c.PostForm("subtype")
	participants := c.PostForm("participants")
	rankOrder, _ := strconv.Atoi(c.PostForm("rank_order"))

	_, err := database.DB.Exec(
		"UPDATE achievements SET type=?, subtype=?, participants=?, rank_order=? WHERE id=?",
		typeReq, subtypeReq, participants, rankOrder, id,
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "update failure"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "achievement updated"})
}

// 删除成果
func DeleteAchievement(c *gin.Context) {
	id, _ := strconv.Atoi(c.Param("id"))

	var filePath string
	row := database.DB.QueryRow("SELECT file_path FROM achievements WHERE id=?", id)
	row.Scan(&filePath)
	os.Remove(filePath)

	_, err := database.DB.Exec("DELETE FROM achievements WHERE id=?", id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "delete failure"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "achievement deleted"})
}

// -------------------- 管理员审核 --------------------

func GetPendingAchievements(c *gin.Context) {
	rows, err := database.DB.Query(
		"SELECT id, type, subtype, participants, rank_order, file_path, status, review_comments, created_at, updated_at FROM achievements WHERE status='待审核'",
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "query failure"})
		return
	}
	defer rows.Close()

	var achievements []Achievement
	for rows.Next() {
		var a Achievement
		if err := rows.Scan(&a.ID, &a.Type, &a.Subtype, &a.Participants, &a.RankOrder, &a.FilePath, &a.Status, &a.ReviewComments, &a.CreatedAt, &a.UpdatedAt); err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "scan failure"})
			return
		}
		achievements = append(achievements, a)
	}

	c.JSON(http.StatusOK, achievements)
}

func ReviewAchievement(c *gin.Context) {
	id, _ := strconv.Atoi(c.Param("id"))
	var req ReviewRequest
	if err := c.ShouldBindJSON(&req); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid parameter"})
		return
	}

	if req.Status != "通过" && req.Status != "未通过" {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid status"})
		return
	}

	_, err := database.DB.Exec(
		"UPDATE achievements SET status=?, review_comments=? WHERE id=?",
		req.Status, req.Comment, id,
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "update failure"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "review updated"})
}

// -------------------- 管理员统计 --------------------

func StatisticsByType(c *gin.Context) {
	// 先尝试从 Redis 缓存读取
	val, err := database.RDB.Get(database.Ctx, "stat_by_type").Result()
	if err == nil {
		c.Data(http.StatusOK, "application/json", []byte(val))
		return
	}

	rows, err := database.DB.Query(
		"SELECT type, COUNT(*) as count FROM achievements WHERE status='通过' GROUP BY type",
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "query failure"})
		return
	}
	defer rows.Close()

	result := []map[string]interface{}{}
	for rows.Next() {
		var t string
		var count int
		rows.Scan(&t, &count)
		result = append(result, map[string]interface{}{"type": t, "count": count})
	}

	jsonBytes, _ := json.Marshal(result)
	database.RDB.Set(database.Ctx, "stat_by_type", jsonBytes, 10*time.Minute)

	c.JSON(http.StatusOK, result)
}

func StatisticsByUser(c *gin.Context) {
	rows, _ := database.DB.Query(
		"SELECT participants, COUNT(*) as count FROM achievements WHERE status='通过' GROUP BY participants",
	)
	defer rows.Close()

	result := []map[string]interface{}{}
	for rows.Next() {
		var participants string
		var count int
		rows.Scan(&participants, &count)
		result = append(result, map[string]interface{}{"participants": participants, "count": count})
	}

	c.JSON(http.StatusOK, result)
}

func StatisticsByTime(c *gin.Context) {
	rows, _ := database.DB.Query(
		"SELECT DATE_FORMAT(created_at, '%Y-%m') as month, COUNT(*) as count FROM achievements WHERE status='通过' GROUP BY month",
	)
	defer rows.Close()

	result := []map[string]interface{}{}
	for rows.Next() {
		var month string
		var count int
		rows.Scan(&month, &count)
		result = append(result, map[string]interface{}{"month": month, "count": count})
	}

	c.JSON(http.StatusOK, result)
}
