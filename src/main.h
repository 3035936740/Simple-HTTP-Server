#pragma once

#ifndef MAIN_H
#define MAIN_H
#include <iostream>

/*--- 以下三方库的头文件的顺序不要乱改! ---*/
#include "httplib.h"
#include "common/utils/log_system.hpp"
#include "common/utils/source_location_handle.hpp"
#include "configuration/config.hpp"
#include "common/utils/sql_util.hpp"
#include "service/sql/sql_handle.hpp"
#include "service/achieve/impl/bottle_service_impl.hpp"
#include "controller/bottle_controller.hpp"
#include "common/utils/http_util.hpp"
#define DEBUG true

//初始化
inline void init(void){
	Config::initialized();
	LogSystem::initialized();
    SQL_Util::initialized();
}

// HTTP
//#define CPPHTTPLIB_OPENSSL_SUPPORT
httplib::Server svr;

//启动HTTP服务器
inline void startServer(void) {
    HTTPUtil::inject(&svr);

    //设置资源目录
    auto ret = svr.set_mount_point("/", "./static");

    //svr.new_task_queue = [] { return new httplib::ThreadPool(8); };
    //svr.set_keep_alive_max_count(2);
    //svr.set_keep_alive_timeout(10);
    //svr.set_read_timeout(5, 0);       // 5 seconds
    //svr.set_write_timeout(5, 0);      // 5 seconds
    //svr.set_idle_interval(0, 100000); // 100 milliseconds
    //svr.set_payload_max_length(1024 * 1024); // 1MB

    //yaml配置
    const bool isCors{ Config::getConfig()["server"]["cors"].as<bool>() };
    const std::string 
        secret{ Config::getConfig()["server"]["token"]["secret"].as<std::string>()},
        issuer{ Config::getConfig()["server"]["token"]["issuer"].as<std::string>()};
    const int port{ Config::getConfig()["server"]["port"].as<int>() };

    httplib::Headers headers_set{
        {"Vary", "Origin, Access-Control-Request-Method, Access-Control-Request-Headers"}
    };
    //CORS相关
    if (isCors) {
        const std::string control_max_age{"86400"};
        headers_set.insert({
            { "Access-Control-Allow-Origin" , "*" },
            { "Access-Control-Allow-Methods", "POST, GET, PUT, OPTIONS, DELETE"},
            { "Access-Control-Max-Age", control_max_age},
            { "Access-Control-Allow-Headers", "*"}
        });
    }

    svr.set_default_headers(headers_set);

    /* start */
    /* bottle的controller */
    BottleService* bottle{ new BottleServiceImpl() };
    BottleController bottle_controller(svr, secret, issuer, bottle);
    bottle_controller.controller();
    /*  end  */

    svr.listen("0.0.0.0", port);
    
    //释放指针
    delete bottle;
    bottle = nullptr;
}

#endif //MAIN_H