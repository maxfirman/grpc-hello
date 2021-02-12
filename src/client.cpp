/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "../proto/data.grpc.pb.h"

using data::DataRequest;
using data::DataResponse;
using data::DataService;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class DataClient
{
public:
	DataClient(std::shared_ptr<Channel> channel)
		: stub_(DataService::NewStub(channel)) {}

	// Assembles the client's payload, sends it and presents the response back
	// from the server.
	void GetData(const std::string &dataset_id)
	{

		// Data we are sending to the server.
		DataRequest request;
		request.set_name(dataset_id);

		// Container for the data we expect from the server.
		DataResponse reply;

		// Context for the client. It could be used to convey extra information to
		// the server and/or tweak certain RPC behaviors.
		ClientContext context;

		// The actual RPC.
		Status status = stub_->GetData(&context, request, &reply);

		// Act upon its status.
		if (status.ok())
		{
			std::cout << "RPC Suceeded! Recieved:" << std::endl;
			for (auto record : reply.records())
				std::cout << record.int_val() << " " << record.float_val() << " " << record.string_val() << std::endl;
		}
		else
		{
			std::cout << status.error_code() << ": " << status.error_message()
				<< std::endl;
			std::cout << "RPC failed" << std::endl;
		}
	}

private:
	std::unique_ptr<DataService::Stub> stub_;
};

std::string simple_test() {
	//std::cout << grpc::InsecureChannelCredentials() << std::endl;
	std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
	DataRequest request;
	request.set_name("test");
	return request.name();
}

int main(int argc, char **argv)
{
	// Instantiate the client. It requires a channel, out of which the actual RPCs
	// are created. This channel models a connection to an endpoint specified by
	// the argument "--target=" which is the only expected argument.
	// We indicate that the channel isn't authenticated (use of
	// InsecureChannelCredentials()).
	std::string target_str;
	std::string arg_str("--target");
	if (argc > 1)
	{
		std::string arg_val = argv[1];
		size_t start_pos = arg_val.find(arg_str);
		if (start_pos != std::string::npos)
		{
			start_pos += arg_str.size();
			if (arg_val[start_pos] == '=')
			{
				target_str = arg_val.substr(start_pos + 1);
			}
			else
			{
				std::cout << "The only correct argument syntax is --target=" << std::endl;
				return 0;
			}
		}
		else
		{
			std::cout << "The only acceptable argument is --target=" << std::endl;
			return 0;
		}
	}
	else
	{
		target_str = "localhost:50051";
	}
	DataClient data_service(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
	data_service.GetData("hello");
	std::cin.get();
	return 0;
}