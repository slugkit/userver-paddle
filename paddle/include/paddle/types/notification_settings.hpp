#pragma once

#include <paddle/types/enums.hpp>
#include <paddle/types/events.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/timestamp.hpp>

#include <paddle/types/response.hpp>

namespace paddle {

struct NotificationSetting {
    NotificationSettingId id;
    std::string description;
    NotificationSettingType type;
    std::string destination;
    bool active;
    std::int32_t api_version;
    bool include_sensitive_fields;
    std::vector<events::EventType> subscribed_events;
    std::string endpoint_secret_key;
    events::TrafficSource traffic_source;
};

using NotificationSettingsResponse = Response<NotificationSetting, MetaPaginated>;

}  // namespace paddle

namespace paddle {

// NotificationSetting
template <typename Format>
Format Serialize(const NotificationSetting& setting, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = setting.id;
    builder["description"] = setting.description;
    builder["type"] = setting.type;
    builder["destination"] = setting.destination;
    builder["active"] = setting.active;
    builder["api_version"] = setting.api_version;
    builder["include_sensitive_fields"] = setting.include_sensitive_fields;
    builder["subscribed_events"] = setting.subscribed_events;
    builder["endpoint_secret_key"] = setting.endpoint_secret_key;
    builder["traffic_source"] = setting.traffic_source;
    return builder.ExtractValue();
}

template <typename Value>
NotificationSetting Parse(const Value& value, userver::formats::parse::To<NotificationSetting>) {
    NotificationSetting setting;
    setting.id = value["id"].template As<NotificationSettingId>();
    setting.description = value["description"].template As<std::string>();
    setting.type = value["type"].template As<NotificationSettingType>();
    setting.destination = value["destination"].template As<std::string>();
    setting.active = value["active"].template As<bool>();
    setting.api_version = value["api_version"].template As<std::int32_t>();
    setting.include_sensitive_fields = value["include_sensitive_fields"].template As<bool>();
    setting.subscribed_events = value["subscribed_events"].template As<std::vector<events::EventType>>();
    setting.endpoint_secret_key = value["endpoint_secret_key"].template As<std::string>();
    setting.traffic_source = value["traffic_source"].template As<events::TrafficSource>();
    return setting;
}

}  // namespace paddle
