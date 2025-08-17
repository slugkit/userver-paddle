#pragma once

#include <userver/formats/parse/boost_optional.hpp>
#include <userver/formats/parse/boost_uuid.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/parse/to.hpp>
#include <userver/formats/serialize/boost_optional.hpp>
#include <userver/formats/serialize/boost_uuid.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/formats/serialize/to.hpp>
#include <userver/utils/strong_typedef.hpp>

#include <userver/formats/json.hpp>

namespace paddle {

using JSON = userver::formats::json::Value;

}  // namespace paddle
