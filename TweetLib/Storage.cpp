#include "Storage.h"
#include <algorithm>

namespace tweet {

bool Storage::Tweet(const std::string& context, const std::string& text)
{
    std::scoped_lock l(mutex_);
    // Check if you are already login.
    auto it = contextNames_.find(context);
    if (it == contextNames_.end()) { return false; }
    // Create a tweet with user name current.
    TweetValue tv{};
    tv.name = it->second;
    auto now = std::chrono::system_clock::now();
    auto now_s = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto value = now_s.time_since_epoch();
    tv.time = value.count();
    tv.text = text;
    nameTweets_.insert({it->second, tv});
    return true;
}

bool Storage::Follow(const std::string& context, const std::string& name)
{
    std::scoped_lock l(mutex_);
    // Check if you are already login.
    auto it = contextNames_.find(context);
    if (it == contextNames_.end()) { return false; }
    // Check if user name is not current one.
    if (name == it->second) { return false; }
    // Check if user name is existing.
    auto it_user = namePasses_.find(name);
    if (it_user == namePasses_.end()) { return false; }
    // Check if this user is already registered in the DB.
    auto range = followers_.equal_range(it->second);
    if (std::find_if(
            range.first,
            range.second,
            [name](const auto& value) { return value.second == name; }) == range.second) {
        followers_.insert({it->second, name});
        return true;
    }
    return false;
}

const std::vector<TweetValue> Storage::Show(
    const std::string& context,
    const std::string& name)
{
    std::scoped_lock l(mutex_);
    // Check if you are already login.
    auto it = contextNames_.find(context);
    if (it == contextNames_.end()) { return {}; }
    bool found = false;
    // Check if current user.
    if (name == it->second) { found = true; } else {
        // Check if the follower is in.
        auto range = followers_.equal_range(it->second);
        found = std::find_if(
                    range.first,
                    range.second,
                    [name](const auto& value) { return value.second == name; }) != range.second;
    }
    // If the follower is in or this is current user.
    if (found) {
        // Transform the current range into a vector.
        std::vector<TweetValue> ret{};
        auto tweet_range = nameTweets_.equal_range(name);
        std::transform(
            tweet_range.first,
            tweet_range.second,
            std::back_inserter(ret),
            [](const auto& value) { return value.second; });
        return ret;
    }
    // Not found so return empty vector.
    return {};
}

bool Storage::Login(
    const std::string& context,
    const std::string& name,
    const std::string& pass)
{
    std::scoped_lock l(mutex_);
    // Check if you are already login.
    auto it = namePasses_.find(name);
    if (it == namePasses_.end()) { return false; }
    // Check the password.
    if (pass == it->second) {
        contextNames_.insert({context, name});
        return true;
    }
    return false;
}

bool Storage::Logout(const std::string& context)
{
    std::scoped_lock l(mutex_);
    auto it = contextNames_.find(context);
    if (it == contextNames_.end()) { return false; }
    contextNames_.erase(it);
    return true;
}

bool Storage::Register(
    const std::string& context,
    const std::string& name,
    const std::string& pass)
{
    std::scoped_lock l(mutex_);
    // Check if you are already login.
    auto it = namePasses_.find(name);
    if (it != namePasses_.end()) { return false; }
    namePasses_.insert({name, pass});
    contextNames_.insert({context, name});
    return true;
}

} // End namespace tweet.
