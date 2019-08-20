#include "EventLogFilterParams.h"
#include <libethcore/Common.h>
#include <libethcore/CommonJS.h>

using namespace dev;
using namespace dev::eth;
using namespace dev::event;

/*
from json received from client create EventLogFilterParams, the json should be like :
{
  "groupId": "1",
  "fromBlock": "0x1",
  "toBlock": "0x2",
  "address": "0x8888f1f195afa192cfee860698584c030f4c9db1",
  "topics": [
    "0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b",
    null,
    [
      "0x000000000000000000000000a94f5374fce5edbc8e2a8697c15331677e6ebf0b",
      "0x0000000000000000000000000aff3454fce5edbc8cca8697c15331677e6ebccc"
    ]
  ]
}
*/

bool EventLogFilterParams::getFilterIDField(const Json::Value& _json, std::string& _filterID)
{
    // filterID field
    if (_json.isMember("filterID"))
    {  // filterID field not exist
        _filterID = _json["filterID"].asString();
        return !_filterID.empty();
    }

    return false;
}

bool EventLogFilterParams::getGroupIDField(const Json::Value& _json, GROUP_ID& _groupID)
{
    // groupID field
    if (_json.isMember("groupID"))
    {  // groupID field not exist
        int groupID = _json["groupID"].asInt();
        if (validGroupID(groupID))
        {
            _groupID = (GROUP_ID)groupID;
            return true;
        }
        // invalid groupid value
    }

    return false;
}

bool EventLogFilterParams::getFromBlockField(const Json::Value& _json, BlockNumber& _startBlock)
{
    // fromBlock field
    if (_json.isMember("fromBlock"))
    {
        std::string strFromBlock = _json["fromBlock"].asString();
        _startBlock = (strFromBlock == "latest" ? MAX_BLOCK_NUMBER : jsToBlockNumber(strFromBlock));
    }
    else
    {
        _startBlock = MAX_BLOCK_NUMBER;
    }

    return true;
}

bool EventLogFilterParams::getToBlockField(const Json::Value& _json, BlockNumber& _endBlock)
{
    // toBlock field
    if (_json.isMember("toBlock"))
    {
        std::string strEndBlock = _json["toBlock"].asString();
        _endBlock = (strEndBlock == "latest" ? MAX_BLOCK_NUMBER : jsToBlockNumber(strEndBlock));
    }
    else
    {
        _endBlock = MAX_BLOCK_NUMBER;
    }

    return true;
}

bool EventLogFilterParams::getAddressField(
    const Json::Value& _json, EventLogFilterParams::Ptr params)
{
    // address field
    if (!_json.isMember("address"))
    {
        return false;
    }

    if (_json["address"].isArray())
    {  // Multiple addresses
        for (auto i : _json["address"])
        {
            params->addAddress(jsToAddress(i.asString()));
        }
    }
    else
    {  // Single address
        params->addAddress(jsToAddress(_json["address"].asString()));
    }

    return true;
}

bool EventLogFilterParams::getTopicField(const Json::Value& _json, EventLogFilterParams::Ptr params)
{
    if (!_json.isMember("topics"))
    {
        return false;
    }

    if (_json["topics"].isArray() && _json["topics"].size() >= eth::MAX_NUM_TOPIC_EVENT_LOG)
    {  // there is at most 4 topics in one event log
        return false;
    }

    if (_json["topics"].isArray())
    {
        for (Json::ArrayIndex i = 0; i < _json["topics"].size(); i++)
        {
            if (_json["topics"][i].isArray())
            {
                for (auto t : _json["topics"][i])
                {
                    if (!t.isNull())
                    {
                        params->addTopic(i, jsToFixed<32>(t.asString()));
                    }
                }
            }
            else if (!_json["topics"][i].isNull())
            {
                params->addTopic(i, jsToFixed<32>(_json["topics"][i].asString()));
            }
        }
    }
    else
    {
        params->addTopic(0, jsToFixed<32>(_json["topics"].asString()));
    }

    return true;
}

EventLogFilterParams::Ptr EventLogFilterParams::buildEventLogFilterParamsObject(
    const std::string& _json)
{
    // ADD TO DO
    EVENT_LOG(ERROR) << LOG_BADGE("buildEventLogFilterParamsObject") << LOG_KV("json", _json);
    return nullptr;
}
