#pragma once

#include <memory>
#if defined(_WIN32) || defined(_WIN64)
#pragma warning(push)
#pragma warning(disable: 4005)
#pragma warning(disable: 4251)
#pragma warning(disable: 4996)
#endif
#include <grpcpp/channel.h>
#include "../GrpcLib/Tweet.pb.h"
#include "../GrpcLib/Tweet.grpc.pb.h"
#if defined(_WIN32) || defined(_WIN64)
#pragma warning(pop)
#endif

namespace tweet {

class Client {
public:
    Client(std::unique_ptr<proto::TweetService::Stub> stub) : stub_(std::move(stub)) {}
    Client(const std::shared_ptr<grpc::Channel> channel) : stub_(proto::TweetService::NewStub(channel)) {}
    proto::TweetOut Tweet(proto::TweetIn in);
    proto::FollowOut Follow(proto::FollowIn in);
    proto::ShowOut Show(proto::ShowIn in);
    proto::LoginOut Login(proto::LoginIn in);
    proto::LogoutOut Logout(proto::LogoutIn in);
    proto::RegisterOut Register(proto::RegisterIn in);

protected:
    std::unique_ptr<proto::TweetService::Stub> stub_;
};
} // namespace tweet
