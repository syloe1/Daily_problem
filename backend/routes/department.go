package routes

import (
	"keshe-backend/database"
	"github.com/gin-gonic/gin"
	"net/http"
	"strconv"
)

// Department 结构体（不变）
type Department struct {
	ID       int    `json:"id"`
	DeptCode string `json:"dept_code"`
	DeptName string `json:"dept_name"`
	Manager  string `json:"manager"`
}

// 【关键修改1】参数从*gin.Engine改为*gin.RouterGroup
func RegisterDepartmentRoutes(parentGroup *gin.RouterGroup) {
	// 【关键修改2】基于传入的父分组（/api）创建部门子分组，保留原权限中间件
	// 最终路由前缀：/api/departments
	deptGroup := parentGroup.Group("/departments", AuthMiddleware(), AdminMiddleware())
	{
		// 路由：GET /api/departments（列表/搜索）
		deptGroup.GET("", GetDepartments)
		// 路由：POST /api/departments（创建）
		deptGroup.POST("", CreateDepartment)
		// 路由：PUT /api/departments/:id（更新）
		deptGroup.PUT("/:id", UpdateDepartment)
		// 路由：DELETE /api/departments/:id（删除）
		deptGroup.DELETE("/:id", DeleteDepartment)
	}
}

// 以下函数逻辑完全不变（无需修改）
func GetDepartments(c *gin.Context) {
	code := c.Query("dept_code")
	name := c.Query("dept_name")

	query := "SELECT id, dept_code, dept_name, manager FROM departments WHERE 1=1"
	args := []interface{}{}

	if code != "" {
		query += " AND dept_code LIKE ?"
		args = append(args, "%"+code+"%")
	}
	if name != "" {
		query += " AND dept_name LIKE ?"
		args = append(args, "%"+name+"%")
	}

	rows, err := database.DB.Query(query, args...)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "query failure"})
		return
	}
	defer rows.Close()

	var depts []Department
	for rows.Next() {
		var d Department
		if err := rows.Scan(&d.ID, &d.DeptCode, &d.DeptName, &d.Manager); err != nil {
			c.JSON(http.StatusInternalServerError, gin.H{"error": "scan failure"})
			return
		}
		depts = append(depts, d)
	}

	c.JSON(http.StatusOK, depts)
}

func CreateDepartment(c *gin.Context) {
	var d Department
	if err := c.ShouldBindJSON(&d); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid parameter"})
		return
	}

	_, err := database.DB.Exec(
		"INSERT INTO departments (dept_code, dept_name, manager) VALUES (?, ?, ?)",
		d.DeptCode, d.DeptName, d.Manager,
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "insert failure"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "department created"})
}

func UpdateDepartment(c *gin.Context) {
	idStr := c.Param("id")
	id, err := strconv.Atoi(idStr)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid id"})
		return
	}

	var d Department
	if err := c.ShouldBindJSON(&d); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid parameter"})
		return
	}

	_, err = database.DB.Exec(
		"UPDATE departments SET dept_code = ?, dept_name = ?, manager = ? WHERE id = ?",
		d.DeptCode, d.DeptName, d.Manager, id,
	)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "update failure"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "department updated"})
}

func DeleteDepartment(c *gin.Context) {
	idStr := c.Param("id")
	id, err := strconv.Atoi(idStr)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "invalid id"})
		return
	}

	_, err = database.DB.Exec("DELETE FROM departments WHERE id = ?", id)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "delete failure"})
		return
	}

	c.JSON(http.StatusOK, gin.H{"message": "department deleted"})
}