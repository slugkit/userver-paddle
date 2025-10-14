#include <paddle/types/subscriptions.hpp>

#include <unordered_map>

namespace paddle::subscriptions {

auto Item::GetDelta() const -> ItemDelta {
    ItemDelta delta;
    delta.type = ItemDelta::Type::kAdded;
    delta.original_price_id = price.id;
    delta.status = status;
    delta.quantity = quantity;
    delta.recurring = recurring;
    delta.previously_billed_at = previously_billed_at;
    delta.next_billed_at = next_billed_at;
    delta.trial_dates = trial_dates;
    delta.price_id = price.id;
    delta.product_id = product.id;
    return delta;
}

auto Item::GetDelta(const Item& other) const -> ItemDelta {
    ItemDelta delta;
    delta.type = ItemDelta::Type::kUnchanged;
    delta.original_price_id = price.id;
    if (status != other.status) {
        delta.status = other.status;
        delta.type = ItemDelta::Type::kUpdated;
    }
    if (quantity != other.quantity) {
        delta.quantity = other.quantity;
        delta.type = ItemDelta::Type::kUpdated;
    }
    if (recurring != other.recurring) {
        delta.recurring = other.recurring;
        delta.type = ItemDelta::Type::kUpdated;
    }
    if (previously_billed_at != other.previously_billed_at) {
        delta.previously_billed_at = other.previously_billed_at;
        delta.type = ItemDelta::Type::kUpdated;
    }
    if (next_billed_at != other.next_billed_at) {
        delta.next_billed_at = other.next_billed_at;
        delta.type = ItemDelta::Type::kUpdated;
    }
    if (trial_dates != other.trial_dates) {
        delta.trial_dates = other.trial_dates;
        delta.type = ItemDelta::Type::kUpdated;
    }
    if (price.id != other.price.id) {
        delta.price_id = other.price.id;
        delta.type = ItemDelta::Type::kUpdated;
    }
    if (product.id != other.product.id) {
        delta.product_id = other.product.id;
        delta.type = ItemDelta::Type::kUpdated;
    }
    return delta;
}

auto SubscriptionDelta::operator[](const PriceId& price_id) const -> const ItemDelta* {
    auto iter = std::find_if(items.begin(), items.end(), [&price_id](const ItemDelta& item) {
        return item.original_price_id == price_id;
    });
    if (iter == items.end()) {
        return nullptr;
    }
    return &*iter;
}

auto Subscription::GetDelta() const -> SubscriptionDelta {
    SubscriptionDelta delta;

    delta.status = status;
    delta.first_billed_at = first_billed_at;
    delta.next_billed_at = next_billed_at;
    delta.paused_at = paused_at;
    delta.canceled_at = canceled_at;

    delta.items.reserve(items.size());
    for (const auto& item : items) {
        delta.items.push_back(item.GetDelta());
        delta.added_items.push_back(&delta.items.back());
    }
    return delta;
}

auto Subscription::GetDelta(const Subscription& other) const -> SubscriptionDelta {
    SubscriptionDelta delta;
    if (status != other.status) {
        delta.status = other.status;
    }
    if (first_billed_at != other.first_billed_at) {
        delta.first_billed_at = other.first_billed_at;
    }
    if (next_billed_at != other.next_billed_at) {
        delta.next_billed_at = other.next_billed_at;
    }
    if (paused_at != other.paused_at) {
        delta.paused_at = other.paused_at;
    }
    if (canceled_at != other.canceled_at) {
        delta.canceled_at = other.canceled_at;
    }
    delta.items.reserve(items.size() + other.items.size());
    for (const auto& item : items) {
        if (other[item.price.id] == nullptr) {
            delta.items.push_back(ItemDelta{
                ItemDelta::Type::kRemoved,
                item.price.id,
                item.status,
                item.quantity,
                item.recurring,
                item.previously_billed_at,
                item.next_billed_at,
                item.trial_dates,
                item.price.id,
                item.product.id
            });
            delta.removed_items.push_back(&delta.items.back());
        } else {
            delta.items.push_back(item.GetDelta(*other[item.price.id]));
            if (delta.items.back().type == ItemDelta::Type::kUpdated) {
                delta.updated_items.push_back(&delta.items.back());
            }
        }
    }
    for (const auto& item : other.items) {
        if ((*this)[item.price.id] == nullptr) {
            delta.items.push_back(ItemDelta{
                ItemDelta::Type::kAdded,
                item.price.id,
                item.status,
                item.quantity,
                item.recurring,
                item.previously_billed_at,
                item.next_billed_at,
                item.trial_dates,
                item.price.id,
                item.product.id
            });
            delta.added_items.push_back(&delta.items.back());
        }
    }
    return delta;
}

auto SubscriptionDelta::HasChanges() const -> bool {
    return !removed_items.empty() || !added_items.empty() || !updated_items.empty();
}

auto SubscriptionDelta::HasRemovedItems() const -> bool {
    return !removed_items.empty();
}

auto SubscriptionDelta::HasAddedItems() const -> bool {
    return !added_items.empty();
}

auto SubscriptionDelta::HasUpdatedItems() const -> bool {
    return !updated_items.empty();
}

auto Subscription::operator[](const PriceId& price_id) const -> const Item* {
    auto iter =
        std::find_if(items.begin(), items.end(), [&price_id](const Item& item) { return item.price.id == price_id; });
    if (iter == items.end()) {
        return nullptr;
    }
    return &*iter;
}

}  // namespace paddle::subscriptions