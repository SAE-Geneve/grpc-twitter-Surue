#include "Server.h"

namespace tweet {
grpc::Status Server::Tweet(
    grpc::ServerContext* context,
    const proto::TweetIn* request,
    proto::TweetOut* response)
{
    const auto value = storage_->Tweet(context->peer(), request->content());
    response->set_error(!value);
    return grpc::Status::OK;
}

grpc::Status Server::Follow(
    grpc::ServerContext* context,
    const proto::FollowIn* request,
    proto::FollowOut* response)
{
    const auto value = storage_->Follow(context->peer(), request->user());
    response->set_error(!value);
    return grpc::Status::OK;
}

grpc::Status Server::Show(
    grpc::ServerContext* context,
    const proto::ShowIn* request,
    proto::ShowOut* response)
{
    const auto value = storage_->Show(context->peer(), request->user());
    response->set_error(false);
	for (const auto& i : value) {
        proto::TweetIn tweet;
		tweet.set_user(i.name);
		tweet.set_content(i.text);
		tweet.set_time(i.time);
		*response->add_tweets() = tweet;
    }
    return grpc::Status::OK;
}

grpc::Status Server::Login(
    grpc::ServerContext* context,
    const proto::LoginIn* request,
    proto::LoginOut* response)
{
    const auto value = storage_->Login(context->peer(), request->user(), request->password());
    response->set_error(!value);
    return grpc::Status::OK;
}

grpc::Status Server::Logout(
    grpc::ServerContext* context,
    const proto::LogoutIn* request,
    proto::LogoutOut* response)
{
    const auto value = storage_->Logout(context->peer());
    response->set_error(!value);
    return grpc::Status::OK;
}

grpc::Status Server::Register(
    grpc::ServerContext* context,
    const proto::RegisterIn* request,
    proto::RegisterOut* response)
{
    const auto value = storage_->Register(context->peer(), request->user(), request->password());
    response->set_error(!value);
    return grpc::Status::OK;
}
} // namespace tweet
