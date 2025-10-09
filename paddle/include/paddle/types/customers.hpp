#pragma once

#include <paddle/types/enums.hpp>
#include <paddle/types/ids.hpp>
#include <paddle/types/response.hpp>
#include <paddle/types/timestamp.hpp>

namespace paddle::customers {

struct Customer {
    CustomerId id;
    std::optional<std::string> name;
    Email email;
    bool marketing_consent;
    Status status;
    JSON custom_data;
    std::string locale;
    Timestamp created_at;
    Timestamp updated_at;
    JSON import_meta;

    template <typename T>
    auto GetCustomData() const -> T {
        return Parse(custom_data, userver::formats::parse::To<T>{});
    }
    template <typename T>
    auto SetCustomData(const T& custom_data) -> void {
        this->custom_data = Serialize(custom_data, userver::formats::serialize::To<JSON>{});
    }
};

struct CustomerUpdateRequest {
    std::optional<std::string> name;
    Email email;
    Status status;
    JSON custom_data;
    std::string locale;
};

struct Address {
    AddressId id;
    CustomerId customer_id;
    std::optional<std::string> description;
    std::optional<std::string> first_line;
    std::optional<std::string> second_line;
    std::optional<std::string> city;
    std::optional<std::string> postal_code;
    std::optional<std::string> region;
    std::string country_code;
    JSON custom_data;
    Status status;
    Timestamp created_at;
    Timestamp updated_at;
    JSON import_meta;

    template <typename T>
    auto GetCustomData() const -> T {
        return Parse(custom_data, userver::formats::parse::To<T>{});
    }
    template <typename T>
    auto SetCustomData(const T& custom_data) -> void {
        this->custom_data = Serialize(custom_data, userver::formats::serialize::To<JSON>{});
    }
};

struct Contact {
    std::string name;
    Email email;
};

struct Business {
    BusinessId id;
    CustomerId customer_id;
    std::optional<std::string> name;
    std::optional<std::string> company_number;
    std::optional<std::string> tax_identifier;
    Status status;
    std::vector<Contact> contacts;
    Timestamp created_at;
    Timestamp updated_at;
    JSON custom_data;
    JSON import_meta;

    template <typename T>
    auto GetCustomData() const -> T {
        return Parse(custom_data, userver::formats::parse::To<T>{});
    }
    template <typename T>
    auto SetCustomData(const T& custom_data) -> void {
        this->custom_data = Serialize(custom_data, userver::formats::serialize::To<JSON>{});
    }
};

}  // namespace paddle::customers

namespace paddle::customers {

template <typename Format>
Format Serialize(const Customer& customer, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = customer.id;
    builder["name"] = customer.name;
    builder["email"] = customer.email;
    builder["marketing_consent"] = customer.marketing_consent;
    builder["status"] = customer.status;
    builder["custom_data"] = customer.custom_data;
    builder["locale"] = customer.locale;
    builder["created_at"] = customer.created_at;
    builder["updated_at"] = customer.updated_at;
    builder["import_meta"] = customer.import_meta;
    return builder.ExtractValue();
}

template <typename Value>
Customer Parse(const Value& value, userver::formats::parse::To<Customer>) {
    Customer customer;
    customer.id = value["id"].template As<CustomerId>();
    customer.name = value["name"].template As<std::optional<std::string>>();
    customer.email = value["email"].template As<Email>();
    customer.marketing_consent = value["marketing_consent"].template As<bool>();
    customer.status = value["status"].template As<Status>();
    customer.custom_data = value["custom_data"].template As<JSON>();
    customer.locale = value["locale"].template As<std::string>();
    customer.created_at = value["created_at"].template As<Timestamp>();
    customer.updated_at = value["updated_at"].template As<Timestamp>();
    customer.import_meta = value["import_meta"].template As<JSON>();
    return customer;
}

// CustomerUpdateRequest

template <typename Format>
Format Serialize(const CustomerUpdateRequest& customer_update_request, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["name"] = customer_update_request.name;
    builder["email"] = customer_update_request.email;
    builder["status"] = customer_update_request.status;
    builder["custom_data"] = customer_update_request.custom_data;
    builder["locale"] = customer_update_request.locale;
    return builder.ExtractValue();
}

template <typename Value>
CustomerUpdateRequest Parse(const Value& value, userver::formats::parse::To<CustomerUpdateRequest>) {
    CustomerUpdateRequest customer_update_request;
    customer_update_request.name = value["name"].template As<std::optional<std::string>>();
    customer_update_request.email = value["email"].template As<Email>();
    customer_update_request.status = value["status"].template As<Status>();
    customer_update_request.custom_data = value["custom_data"].template As<JSON>();
    customer_update_request.locale = value["locale"].template As<std::string>();
    return customer_update_request;
}

// Address
template <typename Format>
Format Serialize(const Address& address, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = address.id;
    builder["customer_id"] = address.customer_id;
    builder["description"] = address.description;
    builder["first_line"] = address.first_line;
    builder["second_line"] = address.second_line;
    builder["city"] = address.city;
    builder["postal_code"] = address.postal_code;
    builder["region"] = address.region;
    builder["country_code"] = address.country_code;
    builder["custom_data"] = address.custom_data;
    builder["status"] = address.status;
    builder["created_at"] = address.created_at;
    builder["updated_at"] = address.updated_at;
    builder["import_meta"] = address.import_meta;
    return builder.ExtractValue();
}

template <typename Value>
Address Parse(const Value& value, userver::formats::parse::To<Address>) {
    Address address;
    address.id = value["id"].template As<AddressId>();
    address.customer_id = value["customer_id"].template As<CustomerId>();
    address.description = value["description"].template As<std::optional<std::string>>();
    address.first_line = value["first_line"].template As<std::optional<std::string>>();
    address.second_line = value["second_line"].template As<std::optional<std::string>>();
    address.city = value["city"].template As<std::optional<std::string>>();
    address.postal_code = value["postal_code"].template As<std::optional<std::string>>();
    address.region = value["region"].template As<std::optional<std::string>>();
    address.country_code = value["country_code"].template As<std::string>();
    address.custom_data = value["custom_data"].template As<JSON>();
    address.status = value["status"].template As<Status>();
    address.created_at = value["created_at"].template As<Timestamp>();
    address.updated_at = value["updated_at"].template As<Timestamp>();
    address.import_meta = value["import_meta"].template As<JSON>();
    return address;
}

// Contact
template <typename Format>
Format Serialize(const Contact& contact, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["name"] = contact.name;
    builder["email"] = contact.email;
    return builder.ExtractValue();
}

template <typename Value>
Contact Parse(const Value& value, userver::formats::parse::To<Contact>) {
    Contact contact;
    contact.name = value["name"].template As<std::string>();
    contact.email = value["email"].template As<Email>();
    return contact;
}

// Business

template <typename Format>
Format Serialize(const Business& business, userver::formats::serialize::To<Format>) {
    typename Format::Builder builder;
    builder["id"] = business.id;
    builder["customer_id"] = business.customer_id;
    builder["name"] = business.name;
    builder["company_number"] = business.company_number;
    builder["tax_identifier"] = business.tax_identifier;
    builder["status"] = business.status;
    builder["contacts"] = business.contacts;
    builder["created_at"] = business.created_at;
    builder["updated_at"] = business.updated_at;
    builder["custom_data"] = business.custom_data;
    builder["import_meta"] = business.import_meta;
    return builder.ExtractValue();
}

template <typename Value>
Business Parse(const Value& value, userver::formats::parse::To<Business>) {
    Business business;
    business.id = value["id"].template As<BusinessId>();
    business.customer_id = value["customer_id"].template As<CustomerId>();
    business.name = value["name"].template As<std::optional<std::string>>();
    business.company_number = value["company_number"].template As<std::optional<std::string>>();
    business.tax_identifier = value["tax_identifier"].template As<std::optional<std::string>>();
    business.status = value["status"].template As<Status>();
    business.contacts = value["contacts"].template As<std::vector<Contact>>();
    business.created_at = value["created_at"].template As<Timestamp>();
    business.updated_at = value["updated_at"].template As<Timestamp>();
    business.custom_data = value["custom_data"].template As<JSON>();
    business.import_meta = value["import_meta"].template As<JSON>();
    return business;
}

}  // namespace paddle::customers