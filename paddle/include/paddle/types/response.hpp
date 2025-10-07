#pragma once

#include <paddle/types/ids.hpp>

#include <paddle/types/formats.hpp>

#include <boost/uuid/uuid.hpp>

#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace paddle {

struct Pagination {
    std::int32_t per_page;
    std::string next;
    bool has_more;
    std::int32_t estimated_total;
};

struct Meta {
    RequestId request_id;
};

struct MetaPaginated {
    RequestId request_id;
    Pagination pagination;
};

template <typename T, typename M>
struct Response {
    using ResponseType = T;
    using DataType = std::vector<ResponseType>;
    using MetaType = M;

    DataType data;
    MetaType meta;
};

template <typename T, typename M = Meta>
struct SingleObjectResponse {
    using ResponseType = T;
    using DataType = ResponseType;
    using MetaType = M;

    DataType data;
    MetaType meta;
};

/// @brief Response with cursor
/// @tparam T The type of the data
/// @return A tuple containing the data, the cursor, and a boolean indicating if
/// there are more pages
template <typename T>
using ResponseWithCursor = std::tuple<std::vector<T>, std::string, bool>;

}  // namespace paddle

namespace paddle {

// Pagination
template <typename Format>
auto Serialize(Pagination pagination, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["per_page"] = pagination.per_page;
    builder["next"] = pagination.next;
    builder["has_more"] = pagination.has_more;
    builder["estimated_total"] = pagination.estimated_total;
    return builder.ExtractValue();
}

template <typename Value>
auto Parse(const Value& value, userver::formats::parse::To<Pagination>) -> Pagination {
    Pagination pagination;
    pagination.per_page = value["per_page"].template As<std::int32_t>();
    pagination.next = value["next"].template As<std::string>();
    pagination.has_more = value["has_more"].template As<bool>();
    pagination.estimated_total = value["estimated_total"].template As<std::int32_t>();
    return pagination;
}

// Meta
template <typename Format>
auto Serialize(Meta meta, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["request_id"] = meta.request_id;
    return builder.ExtractValue();
}

template <typename Value>
auto Parse(const Value& value, userver::formats::parse::To<Meta>) -> Meta {
    Meta meta;
    meta.request_id = value["request_id"].template As<RequestId>();
    return meta;
}

// MetaPaginated
template <typename Format>
auto Serialize(MetaPaginated meta, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["request_id"] = meta.request_id;
    builder["pagination"] = meta.pagination;
    return builder.ExtractValue();
}

template <typename Value>
auto Parse(const Value& value, userver::formats::parse::To<MetaPaginated>) -> MetaPaginated {
    MetaPaginated meta;
    meta.request_id = value["request_id"].template As<RequestId>();
    meta.pagination = value["pagination"].template As<Pagination>();
    return meta;
}

// Response
template <typename T, typename M, typename Format>
auto Serialize(Response<T, M> response, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["data"] = response.data;
    builder["meta"] = response.meta;
    return builder.ExtractValue();
}

template <typename T, typename M, typename Value>
auto Parse(const Value& value, userver::formats::parse::To<Response<T, M>>) -> Response<T, M> {
    Response<T, M> response;
    response.data = value["data"].template As<typename Response<T, M>::DataType>();
    response.meta = value["meta"].template As<typename Response<T, M>::MetaType>();
    return response;
}

// SingleObjectResponse
template <typename T, typename M, typename Format>
auto Serialize(SingleObjectResponse<T, M> response, userver::formats::serialize::To<Format>) -> Format {
    typename Format::Builder builder;
    builder["data"] = response.data;
    builder["meta"] = response.meta;
    return builder.ExtractValue();
}

template <typename T, typename M, typename Value>
auto Parse(const Value& value, userver::formats::parse::To<SingleObjectResponse<T, M>>) -> SingleObjectResponse<T, M> {
    SingleObjectResponse<T, M> response;
    response.data = value["data"].template As<typename SingleObjectResponse<T, M>::DataType>();
    response.meta = value["meta"].template As<typename SingleObjectResponse<T, M>::MetaType>();
    return response;
}

}  // namespace paddle
