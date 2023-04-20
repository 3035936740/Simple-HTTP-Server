#pragma once

#ifndef TEST_PROJECT_HPP
#define TEST_PROJECT_HPP
#include <iostream>
#include <fstream>
#include <filesystem>
#include <service/sql/sql_handle.hpp>
#include <span>
#include <jwt-cpp/jwt.h>
#include <bcrypt.h>

class TestProject final{
public:
	//检测文件是否存在
	static void testMethod1(void) {
		bool b1{ std::filesystem::exists("config.yaml") };
		bool b2{ std::filesystem::exists("config.yml") };
		std::cout << "ymal -> " << b1 << std::endl;
		std::cout << "yml  -> " << b2 << std::endl;
	}

	// SQL查询测试
	static void testMethod2(void) {
		std::cout << "More:\n";
		TestProject::testMoreSQLQuery();

		std::cout << "Simple:\n";
		TestProject::testSimpleSQLQuery();
	}

	// jwt测试
	static void testMethod3(void) {
		std::string token = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJpc3MiOiJhdXRoMCJ9.AbIJTDMFc7yUa5MhvcP03nJPyCPzZtQcGEp-zWfOkEE";
		auto decoded = jwt::decode(token);

		for (auto& e : decoded.get_payload_json())
			std::cout << e.first << " = " << e.second << std::endl;
	}

	// bcrypt测试
	static void testMethod4(void) {
		std::string password = "250kaijie";

		//生成
		std::string hash = bcrypt::generateHash(password,10);

		std::cout << "250kaijie: " << hash << std::endl;

		//效验
		std::cout << "\"" << password << "\" : " << bcrypt::validatePassword(password, hash) << std::endl;
		std::cout << "\"wrong\" : " << bcrypt::validatePassword("wrong", hash) << std::endl;
		std::cout << "\"database\" : " << bcrypt::validatePassword(password, "$2a$10$2MKm.McjEdc/O.PXArxEeOh4dBROZ1BoLswMP8lG8bwRBDZdqhRoe") << std::endl;
	}

	static void testMoreSQLQuery(void) {
		StartWatch();
		SQL_Handle handle;
		auto contents{ handle.moreQuery("SELECT * from bottle") };
		printMaps(contents);

		std::vector<BottleDao> result;
		std::string jsonList {"["};
		bool flag{false};
		for (const auto& content : contents) {
			BottleDao bottle{
				std::stoi(content.at("id").second),
				std::stoi(content.at("bottleMainId").second),
				content.at("thrower").second,
				content.at("throwGroup").second,
				content.at("content").second,
				content.at("filePath").second,
				std::stoull(content.at("timeStamp").second),
				std::stoi(content.at("reportCount").second),
				std::stoi(content.at("available").second)
			};
			jsonList += (flag ? "," : "") + std::move(bottle.getJson().dump());
			flag = true;
			std::cout << "json -> " << bottle.getJson() << "\n";
			result.push_back(std::move(bottle));
		};
		jsonList += "]";

		std::cout << "jsons -> " << jsonList << "\n";
		StopWatch();
	}
	static void testSimpleSQLQuery(void) {
		StartWatch();

		SQL_Handle handle;
		auto content{ handle.simpleQuery("SELECT * from bottle where bottleMainId = 1000000") };
		printMap(content);
		
		BottleDao bottle{ 
			std::stoi(content.at("id").second),
			std::stoi(content.at("bottleMainId").second),
			content.at("thrower").second,
			content.at("throwGroup").second,
			content.at("content").second,
			content.at("filePath").second,
			std::stoull(content.at("timeStamp").second),
			std::stoi(content.at("reportCount").second),
			std::stoi(content.at("available").second)
		};

		std::cout << bottle.toString();
		std::cout << bottle.getJson() << "\n";

		StopWatch();
	}
private:
	static void printMap(const auto& m) {
		std::cout << "========================\n";
		for (auto& [key, value] : m) {
			std::cout << std::format("key:{}, type:{}, value:{}\n", key, value.first, value.second);
		}
		std::cout << "========================\n";
		std::cout.flush();
	}
	
	static void printMaps(const auto& vec) {
		std::cout << "========================\n";
		for (const auto& item : vec) {
			for (auto& [key, value] : item) {
				std::cout << std::format("key:{}, type:{}, value:{}\n", key, value.first, value.second);
			}
			std::cout << "========================\n";
		}
		std::cout.flush();
	}

	inline static std::chrono::system_clock::time_point start, end;

	static void StartWatch(void) {
		start = std::chrono::high_resolution_clock::now();
	};

	static void StopWatch(void) {
		end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		std::cout << "\033[42mtime consumed " << elapsed.count() << " ms\033[0m\n";
		std::cout.flush();
	};
};

#endif