/*
 * @File	  : http_util.hpp
 * @Coding	  : utf-8
 * @Author    : Bing
 * @Time      : 2023/03/19 14:02
 * @Introduce : HTTP工具相关
*/


#pragma once

#ifndef HTTP_UTIL
#define HTTP_UTIL
#include <memory>
#include "jwt-cpp/jwt.h"

using json = nlohmann::json;

class HTTPUtil final{
public:
	inline static void inject(httplib::Server *svr) {
		sm_svr_ptr = svr ;
	}
    inline static void free(void) {
        //静态类释放指针是否有点
		//delete sm_svr_ptr;
		//sm_svr_ptr = nullptr;
	}

    enum RequestMethodType {
        Get,
        Post,
        Delete,
        Put,
        Patch
    };

    inline static void HeaderHandle(std::string_view uri, RequestMethodType type, httplib::Server::Handler handler, int status = 200) {
        sm_svr_ptr->Options(uri.data(), [status](const httplib::Request&, httplib::Response& resp) {
            resp.status = status;
            });
        switch (type)
        {
        case Get:
            sm_svr_ptr->Get(uri.data(), std::move(handler));
            break;
        case Post:
            sm_svr_ptr->Post(uri.data(), std::move(handler));
            break;
        case Delete:
            sm_svr_ptr->Delete(uri.data(), std::move(handler));
            break;
        case Put:
            sm_svr_ptr->Put(uri.data(), std::move(handler));
            break;
        case Patch:
            sm_svr_ptr->Patch(uri.data(), std::move(handler));
            break;
        default:
            break;
        }
    };

    inline static void getHTTP(std::string_view uri, httplib::Server::Handler handler) {
        HeaderHandle(std::move(uri), RequestMethodType::Get, std::move(handler), 200);
    }

    inline static void patchHTTP(std::string_view uri, httplib::Server::Handler handler) {
        HeaderHandle(std::move(uri), RequestMethodType::Patch, std::move(handler), 200);
    }

    inline static void deleteHTTP(std::string_view uri, httplib::Server::Handler handler) {
        HeaderHandle(std::move(uri), RequestMethodType::Delete, std::move(handler), 200);
    }

    inline static void postHTTP(std::string_view uri, httplib::Server::Handler handler) {
        HeaderHandle(std::move(uri), RequestMethodType::Post, std::move(handler), 200);
    }

    inline static void putHTTP(std::string_view uri, httplib::Server::Handler handler) {
        HeaderHandle(std::move(uri), RequestMethodType::Put, std::move(handler), 200);
    }

    class StatusCodeHandle final{
    public:
        enum status {
            NotExistThisStatusCode = 0,
            Continue = 100,
            SwitchingProtocol = 101,
            Processing = 102,
            EarlyHints = 103,
            OK = 200,
            Created = 201,
            Accepted = 202,
            Non_AuthoritativeInformation = 203,
            NoContent = 204,
            ResetContent = 205,
            PartialContent = 206,
            Multi_Status = 207,
            AlreadyReported = 208,
            IMUsed = 226,
            MultipleChoices = 300,
            MovedPermanently = 301,
            Found = 302,
            SeeOther = 303,
            NotModified = 304,
            TemporaryRedirect = 307,
            PermanentRedirect = 308,
            BadRequest = 400,
            Unauthorized = 401,
            PaymentRequired = 402,
            Forbidden = 403,
            NotFound = 404,
            MethodNotAllowed = 405,
            NotAcceptable = 406,
            ProxyAuthenticationRequired = 407,
            RequestTimeout = 408,
            Conflict = 409,
            Gone = 410,
            LengthRequired = 411,
            PreconditionFailed = 412,
            ContentTooLarge = 413,
            URITooLong = 414,
            UnsupportedMediaType = 415,
            RangeNotSatisfiable = 416,
            ExpectationFailed = 417,
            IAmATeapot = 418,
            MisdirectedRequest = 421,
            UnprocessableContent = 422,
            Locked = 423,
            FailedDependency = 424,
            TooEarly = 425,
            UpgradeRequired = 426,
            PreconditionRequired = 428,
            TooManyRequests = 429,
            RequestHeaderFieldsTooLarge = 431,
            UnavailableForLegalReasons = 451,
            InternalServerError = 500,
            NotImplemented = 501,
            BadGateway = 502,
            ServiceUnavailable = 503,
            GatewayTimeout = 504,
            HTTPVersionNotSupported = 505,
            VariantAlsoNegotiates = 506,
            InsufficientStorage = 507,
            LoopDetected = 508,
            NotExtended = 510,
            NetworkAuthenticationRequired = 511
        };

        const inline static json stateJSONResult(status state, std::string_view msg = "") {
            auto now{ std::chrono::system_clock::now() };
            std::time_t t = std::chrono::system_clock::to_time_t(std::move(now));
            std::tm local_tm = *std::localtime(&t);
            std::ostringstream oss;
            oss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");

            json result;
            result["data"] = oss.str();
            result["code"] = state;

            if (!msg.empty()) {
                result["comment"] = msg;
            }

            switch (state)
            {
            case BadRequest:
                result["message"] = "Bad Request";
                break;
            case Unauthorized:
                result["message"] = "Unauthorized";
                break;
            case PaymentRequired:
                result["message"] = "Payment Required";
                break;
            case Forbidden:
                result["message"] = "Forbidden";
                break;
            case NotFound:
                result["message"] = "Not Found";
                break;
            case MethodNotAllowed:
                result["message"] = "Method Not Allowed";
                break;
            case NotAcceptable:
                result["message"] = "Not Acceptable";
                break;
            case ProxyAuthenticationRequired:
                result["message"] = "Proxy Authentication Required";
                break;
            case RequestTimeout:
                result["message"] = "Request Timeout";
                break;
            case Conflict:
                result["message"] = "Conflict";
                break;
            case Gone:
                result["message"] = "Gone";
                break;
            case LengthRequired:
                result["message"] = "Length Required";
                break;
            case PreconditionFailed:
                result["message"] = "Precondition Failed";
                break;
            case ContentTooLarge:
                result["message"] = "Content Too Large";
                break;
            case URITooLong:
                result["message"] = "URI Too Long";
                break;
            case UnsupportedMediaType:
                result["message"] = "Unsupported Media Type";
                break;
            case RangeNotSatisfiable:
                result["message"] = "Range Not Satisfiable";
                break;
            case ExpectationFailed:
                result["message"] = "Expectation Failed";
                break;
            case IAmATeapot:
                result["message"] = "I'm a teapot";
                break;
            case MisdirectedRequest:
                result["message"] = "Misdirected Request";
                break;
            case UnprocessableContent:
                result["message"] = "Unprocessable Content";
                break;
            case Locked:
                result["message"] = "Locked";
                break;
            case FailedDependency:
                result["message"] = "Failed Dependency";
                break;
            case TooEarly:
                result["message"] = "Too Early";
                break;
            case UpgradeRequired:
                result["message"] = "Upgrade Required";
                break;
            case PreconditionRequired:
                result["message"] = "Precondition Required";
                break;
            case TooManyRequests:
                result["message"] = "Too Many Requests";
                break;
            case RequestHeaderFieldsTooLarge:
                result["message"] = "Request Header Fields Too Large";
                break;
            case UnavailableForLegalReasons:
                result["message"] = "Unavailable For Legal Reasons";
                break;
            case InternalServerError:
                result["message"] = "Internal Server Error";
                break;
            case NotImplemented:
                result["message"] = "Not Implemented";
                break;
            case BadGateway:
                result["message"] = "Bad Gateway";
                break;
            case ServiceUnavailable:
                result["message"] = "Service Unavailable";
                break;
            case GatewayTimeout:
                result["message"] = "Gateway Timeout";
                break;
            case HTTPVersionNotSupported:
                result["message"] = "HTTP Version Not Supported";
                break;
            case VariantAlsoNegotiates:
                result["message"] = "Variant Also Negotiates";
                break;
            case InsufficientStorage:
                result["message"] = "Insufficient Storage";
                break;
            case LoopDetected:
                result["message"] = "Loop Detected";
                break;
            case NotExtended:
                result["message"] = "Not Extended";
                break;
            case NetworkAuthenticationRequired:
                result["message"] = "Network Authentication Required";
                break;
            default:
                result["message"] = "Not Exist This Status Code";
                break;
            }
            return result;
        };

        constexpr inline static status getStatus(int code) {
            switch (code)
            {
            case Continue:
                return Continue;
            case SwitchingProtocol:
                return SwitchingProtocol;
            case Processing:
                return Processing;
            case EarlyHints:
                return EarlyHints;
            case OK:
                return OK;
            case Created:
                return Created;
            case Accepted:
                return Accepted;
            case Non_AuthoritativeInformation:
                return Non_AuthoritativeInformation;
            case NoContent:
                return NoContent;
            case ResetContent:
                return ResetContent;
            case PartialContent:
                return PartialContent;
            case Multi_Status:
                return Multi_Status;
            case AlreadyReported:
                return AlreadyReported;
            case IMUsed:
                return IMUsed;
            case MultipleChoices:
                return MultipleChoices;
            case MovedPermanently:
                return MovedPermanently;
            case Found:
                return Found;
            case SeeOther:
                return SeeOther;
            case NotModified:
                return NotModified;
            case TemporaryRedirect:
                return TemporaryRedirect;
            case PermanentRedirect:
                return PermanentRedirect;
            case BadRequest:
                return BadRequest;
            case Unauthorized:
                return Unauthorized;
            case PaymentRequired:
                return PaymentRequired;
            case Forbidden:
                return Forbidden;
            case NotFound:
                return NotFound;
            case MethodNotAllowed:
                return MethodNotAllowed;
            case NotAcceptable:
                return NotAcceptable;
            case ProxyAuthenticationRequired:
                return ProxyAuthenticationRequired;
            case RequestTimeout:
                return RequestTimeout;
            case Conflict:
                return Conflict;
            case Gone:
                return Gone;
            case LengthRequired:
                return LengthRequired;
            case PreconditionFailed:
                return PreconditionFailed;
            case ContentTooLarge:
                return ContentTooLarge;
            case URITooLong:
                return URITooLong;
            case UnsupportedMediaType:
                return UnsupportedMediaType;
            case RangeNotSatisfiable:
                return RangeNotSatisfiable;
            case ExpectationFailed:
                return ExpectationFailed;
            case IAmATeapot:
                return IAmATeapot;
            case MisdirectedRequest:
                return MisdirectedRequest;
            case UnprocessableContent:
                return UnprocessableContent;
            case Locked:
                return Locked;
            case FailedDependency:
                return FailedDependency;
            case TooEarly:
                return TooEarly;
            case UpgradeRequired:
                return UpgradeRequired;
            case PreconditionRequired:
                return PreconditionRequired;
            case TooManyRequests:
                return TooManyRequests;
            case RequestHeaderFieldsTooLarge:
                return RequestHeaderFieldsTooLarge;
            case UnavailableForLegalReasons:
                return UnavailableForLegalReasons;
            case InternalServerError:
                return InternalServerError;
            case NotImplemented:
                return NotImplemented;
            case BadGateway:
                return BadGateway;
            case ServiceUnavailable:
                return ServiceUnavailable;
            case GatewayTimeout:
                return GatewayTimeout;
            case HTTPVersionNotSupported:
                return HTTPVersionNotSupported;
            case VariantAlsoNegotiates:
                return VariantAlsoNegotiates;
            case InsufficientStorage:
                return InsufficientStorage;
            case LoopDetected:
                return LoopDetected;
            case NotExtended:
                return NotExtended;
            case NetworkAuthenticationRequired:
                return NetworkAuthenticationRequired;
            default:
                return NotExistThisStatusCode;
            }
        };

        const inline static json stateJSONResultSimple(int code, std::string_view msg = "") {
            return stateJSONResult(getStatus(code), msg);
        };
    private:
        StatusCodeHandle(void) = delete;
        ~StatusCodeHandle(void) = delete;
        StatusCodeHandle(const StatusCodeHandle&) = delete;
        StatusCodeHandle(StatusCodeHandle&&) = delete;
        StatusCodeHandle& operator=(const StatusCodeHandle&) = delete;
        StatusCodeHandle& operator=(StatusCodeHandle&&) = delete;
    };
private:
	inline static httplib::Server* sm_svr_ptr;
};

#endif