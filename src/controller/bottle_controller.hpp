/*
 * @File	  : bottle_controller.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2023/03/18 20:23
 * @Introduce : 漂流瓶controller
*/

#pragma once
#ifndef BOTTLE_CONTROLLER
#define BOTTLE_CONTROLLER
#include <future>
#include <chrono>
#include "httplib.h"
#include "common/utils/http_util.hpp"
#include "nlohmann/json.hpp"
#include "bcrypt.h"

using json = nlohmann::json;
using namespace std::chrono;
using namespace std::chrono_literals;
using StatusCode = HTTPUtil::StatusCodeHandle::status;
using StatusCodeHandle = HTTPUtil::StatusCodeHandle;
class BottleController final {
public:
    BottleController(httplib::Server& svr, std::string_view secret, std::string_view issuer, BottleService* bottle) :
        m_svr{ svr }, m_secret{ secret }, m_issuer { issuer }, m_bottle{ bottle } {
    };
    ~BottleController() = default;
	void controller(void){
        HTTPUtil::getHTTP("/test", [](const httplib::Request& req, httplib::Response& resp) {
            resp.set_content(R"-({"message":"Hello World!"})-", "application/json");
            });

        HTTPUtil::getHTTP("/getUser", [&](const httplib::Request& req, httplib::Response& resp) {
            auto sec{ req.get_header_value("secret") };
            if (sec.compare(m_secret) == 0) {
                resp.set_content(m_bottle->getToken(), "application/json");
            }
            else {
                resp.status = 401;
                resp.set_content(StatusCodeHandle::stateJSONResultSimple(401).dump(), "application/json");
            }});
        
        HTTPUtil::getHTTP("/getBottle", [&](const httplib::Request& req, httplib::Response& resp) {
            int id{ -1 };
            if (req.has_param("id")) {
                id = std::stoi(req.get_param_value("id"));
            }
            bool verifySQL { self::CheckParameter(std::to_string(id)) };
            if (verifySQL)
            {
                resp.set_content(m_bottle->getBottle(id), "application/json");
            }
            else {
                resp.set_content("error", "application/json");
            }
        });

        HTTPUtil::postHTTP("/verify", [&](const httplib::Request& req, httplib::Response& resp) {
            constexpr auto timeout{200ms};
            //auto [pass , msg]  { verifyToken(req, resp) };
            auto future { std::async(std::launch::async, [&](const httplib::Request& req,httplib::Response& resp) -> verify_token_result {
                return verifyToken(req, resp);
            }, std::cref(req), std::ref(resp))};

            std::future_status status{ future.wait_for(timeout) };

            if (status == std::future_status::timeout) {
                resp.set_content(StatusCodeHandle::stateJSONResultSimple(408).dump(), "application/json");
            }
            else {
                auto p{ future.get() };
                if (p.pass) {
                    resp.set_content("ok", "application/json");
                }
                else
                {
                    resp.set_content(StatusCodeHandle::stateJSONResultSimple(p.code, p.msg).dump(), "application/json");
                }
            }
            });
	}
private:
    struct verify_token_result {
        bool pass;
        std::string msg;
        int code;
    };

    verify_token_result verifyToken(const httplib::Request& req, httplib::Response& resp) {
        verify_token_result result;
        auto j = json::parse(req.body);
        std::string passwd{ j["password"] };
        auto decode_token{ jwt::decode(req.get_header_value("token")) };
        const auto passwd_jwt{ decode_token.get_payload_claim("password").as_string() };
        bool verify_passwd{ bcrypt::validatePassword(passwd, passwd_jwt) };

        if (!verify_passwd)
        {
            result.msg = "Password verification failed";
            result.pass = false;
            result.code = 401;
            return result;
        }

        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{ m_secret })
            .with_issuer(m_issuer);

        try {
            verifier.verify(decode_token);
        }
        catch (const std::exception& e) {
            //std::cout << "except" << std::endl;
            result.msg = e.what();
            result.pass = false;
            result.code = 401;
            return result;
        }
        result.pass = true;
        result.code = 200;
        //std::cout << "final" << std::endl;
        return result;
    };
	BottleController() = delete;
	httplib::Server& m_svr;
	const std::string m_secret;
    const std::string m_issuer;
    BottleService *m_bottle;
};

#endif