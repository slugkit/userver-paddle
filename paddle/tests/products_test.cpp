#include <paddle/types/product.hpp>

#include <userver/utest/utest.hpp>

#include <boost/uuid/string_generator.hpp>

namespace paddle {

namespace {
const auto kSingleProduct = R"({
    "id": "pro_01k2jggszecjqbcsrtaphs2ctv",
    "name": "SaaS Business",
    "tax_category": "saas",
    "type": "standard",
    "description": "SlugKit SaaS solution for medium-sized business",
    "image_url": "https://dev.slugkit.dev/favicon.png",
    "custom_data": {
        "slug": "saas-business"
    },
    "status": "active",
    "import_meta": null,
    "created_at": "2025-08-13T20:04:08.302Z",
    "updated_at": "2025-08-13T20:14:06.729Z"
})";

const auto kProductsResponse = R"({
	"data": [
		{
			"id": "pro_01k2jggszecjqbcsrtaphs2ctv",
			"name": "SaaS Business",
			"tax_category": "saas",
			"type": "standard",
			"description": "SlugKit SaaS solution for medium-sized business",
			"image_url": "https://dev.slugkit.dev/favicon.png",
			"custom_data": {
				"slug": "saas-business"
			},
			"status": "active",
			"import_meta": null,
			"created_at": "2025-08-13T20:04:08.302Z",
			"updated_at": "2025-08-13T20:14:06.729Z"
		},
		{
			"id": "pro_01k2jg8q37nnx510hdd4d6e2rf",
			"name": "SaaS Startup",
			"tax_category": "saas",
			"type": "standard",
			"description": "SaaS subscription plan for a small company",
			"image_url": "https://dev.slugkit.dev/favicon.png",
			"custom_data": {
				"slug": "saas-startup"
			},
			"status": "active",
			"import_meta": null,
			"created_at": "2025-08-13T19:59:43.207Z",
			"updated_at": "2025-08-13T20:14:44.818Z"
		},
		{
			"id": "pro_01k2jfwwcvmscf97tnxxa0v81t",
			"name": "SaaS Indy",
			"tax_category": "saas",
			"type": "standard",
			"description": "SlugKit subscription plan for individual developers",
			"image_url": "https://dev.slugkit.dev/favicon.png",
			"custom_data": {
				"slug": "saas-developer"
			},
			"status": "active",
			"import_meta": null,
			"created_at": "2025-08-13T19:53:15.419Z",
			"updated_at": "2025-08-13T20:15:09.484Z"
		}
	],
	"meta": {
		"request_id": "1070cdd1-69ee-46de-b7e1-e1c6fbbf0913",
		"pagination": {
			"per_page": 50,
			"next": "https://sandbox-api.paddle.com/products?after=pro_01k2jfwwcvmscf97tnxxa0v81t",
			"has_more": false,
			"estimated_total": 3
		}
	}
})";

const auto kRequestId = RequestId{boost::uuids::string_generator()("1070cdd1-69ee-46de-b7e1-e1c6fbbf0913")};
}  // namespace

UTEST(ProductsTest, Parse) {
    userver::formats::json::Value json = userver::formats::json::FromString(kSingleProduct);
    auto product = Parse(json, userver::formats::parse::To<products::Product>());
    EXPECT_EQ(product.id, ProductId("pro_01k2jggszecjqbcsrtaphs2ctv"));
    EXPECT_EQ(product.name, "SaaS Business");
    EXPECT_EQ(product.tax_category, "saas");
    EXPECT_EQ(product.type, CatalogType::kStandard);
    EXPECT_EQ(product.description, "SlugKit SaaS solution for medium-sized business");
}

UTEST(ProductsTest, ParseResponse) {
    userver::formats::json::Value json = userver::formats::json::FromString(kProductsResponse);
    auto response = Parse(json, userver::formats::parse::To<products::ProductsResponse>());
    EXPECT_EQ(response.data.size(), 3);
    EXPECT_EQ(response.meta.request_id, kRequestId);
    EXPECT_EQ(response.meta.pagination.per_page, 50);
}

}  // namespace paddle