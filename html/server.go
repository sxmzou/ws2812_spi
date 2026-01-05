package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"os"
)

func main() {
	// 命令行参数
	port := flag.String("p", "8030", "HTTP 服务端口")
	dir := flag.String("d", ".", "静态文件目录")
	flag.Parse()

	// 获取当前目录
	if *dir == "." {
		abs, err := os.Getwd()
		if err == nil {
			*dir = abs
		}
	}

	// 根路径重定向到 HTML 编辑器
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		if r.URL.Path == "/" {
			http.Redirect(w, r, "/pixel_editor.html", http.StatusFound)
			return
		}
		// 其他路径交给静态文件服务器
		fs := http.FileServer(http.Dir(*dir))
		fs.ServeHTTP(w, r)
	})

	// 启动服务器
	addr := ":" + *port
	fmt.Printf("========================================\n")
	fmt.Printf("  LED 表情编辑器服务器\n")
	fmt.Printf("========================================\n")
	fmt.Printf("  访问地址: http://localhost%s\n", addr)
	fmt.Printf("  文件目录: %s\n", *dir)
	fmt.Printf("========================================\n")

	if err := http.ListenAndServe(addr, nil); err != nil {
		log.Fatal(err)
	}
}
