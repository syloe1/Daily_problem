package database

import (
	"context"
	"database/sql"
	"log"
	"time"

	"github.com/go-redis/redis/v8"
	_ "github.com/go-sql-driver/mysql"
)

var DB *sql.DB
var Ctx = context.Background() // 导出 ctx
var RDB *redis.Client

func InitDB() {
	var err error
	DB, err = sql.Open("mysql", "wk:qaz123@tcp(127.0.0.1:3306)/keshe?parseTime=true")
	if err != nil {
		log.Fatal("database connect failure:", err)
	}
	DB.SetMaxOpenConns(50)
	DB.SetMaxIdleConns(25)
	DB.SetConnMaxLifetime(time.Hour)
	if err = DB.Ping(); err != nil {
		log.Fatal("database ping failure:", err)
	}
	log.Println("database connect success")
}

func InitRedis() {
	RDB = redis.NewClient(&redis.Options{
		Addr:     "localhost:6379",
		Password: "",
		DB:       0,
	})

	_, err := RDB.Ping(Ctx).Result()
	if err != nil {
		log.Fatalf("Redis connect failure: %v", err)
	}
	log.Println("Redis connect success")
}

func InitAll() {
	InitDB()
	InitRedis()
}

/*
go run main.go --port=8888 &
go run main.go --port=8889 &
go run main.go --port=8890 &

*/
