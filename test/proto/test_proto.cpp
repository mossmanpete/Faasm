#include <catch/catch.hpp>
#include <iostream>

#include <proto/faasm.pb.h>
#include <util/util.h>

namespace tests {

    std::vector<uint8_t> createInput(int start, int length) {
        std::vector<uint8_t> result;
        result.reserve((unsigned long) length);

        for (int i = start; i < start + length; i++) {
            result.emplace_back((uint8_t) i);
        }

        return result;
    }

    TEST_CASE("Test protobuf classes", "[proto]") {
        message::Message funcCall;

        std::string user = "foobar user";
        std::string func = "foobar func";
        std::string resultKey = "dummy result";

        std::vector<uint8_t> inputData = createInput(0, 100);
        std::vector<uint8_t> outputData = createInput(120, 50);

        funcCall.set_user(user);
        funcCall.set_function(func);
        funcCall.set_resultkey(resultKey);

        funcCall.set_inputdata(inputData.data(), 100);
        funcCall.set_outputdata(outputData.data(), 50);

        funcCall.set_isasync(true);

        REQUIRE(funcCall.type() == message::Message_MessageType_CALL);
        REQUIRE(user == funcCall.user());
        REQUIRE(func == funcCall.function());
        REQUIRE(resultKey == funcCall.resultkey());

        // Check serialisation round trip
        std::string serialised = funcCall.SerializeAsString();

        message::Message newFuncCall;
        newFuncCall.ParseFromString(serialised);

        REQUIRE(user == newFuncCall.user());
        REQUIRE(func == newFuncCall.function());
        REQUIRE(resultKey == newFuncCall.resultkey());
        REQUIRE(newFuncCall.isasync());

        // Check input/ output data
        const std::string actualStrInput = newFuncCall.inputdata();
        const std::string actualStrOutput = newFuncCall.outputdata();

        const std::vector<uint8_t> actualBytesInput = util::stringToBytes(actualStrInput);
        const std::vector<uint8_t> actualBytesOutput = util::stringToBytes(actualStrOutput);

        REQUIRE(inputData == actualBytesInput);
        REQUIRE(outputData == actualBytesOutput);
    }
}