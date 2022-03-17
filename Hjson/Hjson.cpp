#include "Hjson.h"

namespace Hjson
{
	std::pair<std::string,Json> Hjson::Json::Parse(const std::string& in, ParseModel model)
	{
		std::string err;
		JsonParser parser(in, model,err);
		Json ret = parser.Parse(0);
		//garbage		
		return std::make_pair(err, ret);
	}

	std::pair<bool, const Json&> Json::at(const std::string& key) const
	{
		return value_->at(key);
	}

	std::pair<bool, const Json&> Json::at(size_t i) const
	{
		return value_->at(i);
	}

	std::string JsonObject::GetRawStr() const
	{
		//bool beg_flg = true;
		//std::string ret;
		//ret += "{";
		//for (const auto& kv : raw_)
		//{
		//	if (!beg_flg)
		//	{
		//		ret += ",";
		//	}
		//	ret += RawStr::GetRawStr(kv.first);
		//	ret += ": ";
		//	ret+=kv.second.GetRawStr();
		//	beg_flg = false;
		//}
		//ret += "}";
		return RawStr::GetRawStr(raw_);
	}

	std::pair<bool, const Json&> JsonObject::at(const std::string& key) const
	{
		auto iter = raw_.find(key);
		if (iter != raw_.end())
		{
			return std::make_pair(true, iter->second);
		}
		return std::make_pair(false, Json());
	}

	std::string JsonArray::GetRawStr() const
	{
		return RawStr::GetRawStr(raw_);
	}
	std::pair<bool, const Json&> JsonArray::at(size_t i) const
	{
		if (i < raw_.size())
		{
			return std::make_pair(true, raw_[i]);
		}
		return std::make_pair(false, Json());;
	}
	std::string JsonInt::GetRawStr() const
	{		
		//return std::to_string(raw_);
		return RawStr::GetRawStr(raw_);		
	}
	std::string JsonDouble::GetRawStr() const
	{
		//return std::to_string(raw_);
		return RawStr::GetRawStr(raw_);
	}
	std::string JsonBool::GetRawStr() const
	{
		//return raw_?"true":"false";
		return RawStr::GetRawStr(raw_);
	}
	std::string JsonStr::GetRawStr() const
	{
		//return "\""+raw_+"\"";
		return RawStr::GetRawStr(raw_);
	}
	std::pair<bool, const Json&> Value::at(const std::string& key) const
	{
		return std::make_pair(false, Json());
	}
	std::pair<bool, const Json&> Value::at(size_t i) const
	{
		return std::make_pair(false, Json());
	}
}


