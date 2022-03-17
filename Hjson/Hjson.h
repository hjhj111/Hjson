#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<memory>
#include<iostream>
#include<fstream>
#include<sstream>

using std::shared_ptr;
using std::make_shared;

namespace Hjson
{
	class Json;
	using Array = std::vector<Json>;
	using Object = std::unordered_map<std::string, Json>;

	inline bool in_range(long x, long lower, long upper) {
		return (x >= lower && x <= upper);
	}

	inline std::string ReadFile(std::string file_path_name)
	{
		std::ifstream inf(file_path_name);
		if (inf.fail())
		{
			std::cout << "open file failed" << std::endl;
			return "";
		}
		//inf.seekg(0, std::ios::end);
		//const int length = inf.tellg();
		//inf.seekg(0, std::ios::beg);
		
		std::stringstream outs;
		outs << inf.rdbuf();
		return outs.str();
	}

	class Value
	{
	public:
		enum Type
		{
			NUL,
			INT,
			DOUBLE,
			BOOL,
			STRING,
			OBJECT,
			ARRAY,
		};

	public:
		Value(Type type)
			:type_(type)
		{

		}

		virtual std::pair<bool, int> IntValue() const
		{
			return std::make_pair(false, 0);
		}

		virtual std::pair<bool, double> DoubleValue() const
		{
			return std::make_pair(false, 0);
		}

		virtual std::pair<bool, bool> BoolValue() const
		{
			return std::make_pair(false, true);
		}

		virtual std::pair<bool, std::string> StrValue() const
		{
			return std::make_pair(false, "null string");
		}

		//virtual Object ObjectValue() const;
		
		//virtual Array ArrayValue() const ;

		virtual std::pair<bool, const Json& > at(const std::string& key) const;
		
		virtual std::pair<bool, const Json& > at(size_t i)const;

		virtual std::string GetRawStr() const=0;

		Type GetType()
		{
			return type_;
		}

	private:
		Type type_;
	};

	class JsonNull: public Value
	{
	public:
		JsonNull()
			:Value(Value::NUL)
		{

		}
		std::string GetRawStr() const override
		{
			return "NUL";
		}
	};
	
	class JsonInt: public Value
	{
	public:
		explicit JsonInt(int raw)
			:Value(Value::INT),
			raw_(raw)
		{

		}
		std::string GetRawStr() const override;
		std::pair<bool, int> IntValue() const override
		{
			return std::make_pair(true, raw_);
		}

	private:
		int raw_;
	};

	class JsonDouble: public Value
	{
	public:
		explicit JsonDouble(double raw)
			:Value(Value::DOUBLE),
			raw_(raw)
		{

		}
		std::string GetRawStr() const override;
		std::pair<bool, double> DoubleValue() const override
		{
			return std::make_pair(true, raw_);
		}

	private:
		double raw_;
	};

	class JsonBool : public Value
	{
	public:
		explicit JsonBool(bool raw)
			:Value(Value::BOOL),
			raw_(raw)
		{

		}
		std::string GetRawStr() const override;
		std::pair<bool, bool> BoolValue() const override
		{
			return std::make_pair(true, raw_);
		}

	private:
		bool raw_;
	};

	class JsonStr : public Value
	{
	public:
		explicit JsonStr(const std::string& raw)
			:Value(Value::STRING),
			raw_(raw)
		{

		}
		explicit JsonStr(std::string&& raw)
			:Value(Value::STRING),
			raw_(std::move(raw))
		{

		}
		explicit JsonStr(const char* raw)
			:Value(Value::STRING),
			raw_(raw)
		{

		}
		std::string GetRawStr() const override;
		std::pair<bool, std::string> StrValue() const override
		{
			return std::make_pair(true, raw_);
		}
	private:
		std::string raw_;
	};

	class JsonObject : public Value
	{
	public: 
		explicit JsonObject(const Object& raw)
			:Value(Value::OBJECT),
			raw_(raw)
		{

		}

		explicit JsonObject(Object&& raw)
			:Value(Value::OBJECT),
			raw_(std::move(raw))
		{

		}

		std::string GetRawStr() const override;	
		
		std::pair<bool, const Json& > at(const std::string& key) const override;
	private:
		Object raw_;
	};

	class JsonArray : public Value
	{
	public:
		explicit JsonArray(const Array& raw)
			:Value(Value::ARRAY),
			raw_(raw)
		{

		}

		explicit JsonArray(Array&& raw)
			:Value(Value::ARRAY),
			raw_(move(raw))
		{

		}

		std::string GetRawStr() const override;

		std::pair<bool, const Json& > at(size_t i)const override;

	private:
		Array raw_;
	};

	class Json
	{
	public:
		enum Type
		{
			ARRAY,
			OBJECT,
			NUL
		};
		enum ParseModel
		{
			NOCOMMENTS,
			COMMENTS
		};

		Json()
			:value_(make_shared<JsonNull>())
		{
			
		}

		Json(std::nullptr_t)
			:value_(make_shared<JsonNull>())
		{

		}

		Json(double value)
			:value_(make_shared<JsonDouble>(value))
		{

		}

		Json(int value)
			:value_(make_shared<JsonInt>(value))
		{

		}

		Json(bool value)
			:value_(make_shared<JsonBool>(value))
		{

		}

		Json(const std::string& value)
			:value_(make_shared<JsonStr>(value))
		{

		}

		Json(std::string&& value)
			:value_(make_shared<JsonStr>(value))
		{

		}

		Json(const char* value)
			:value_(make_shared<JsonStr>(value))
		{

		}

		Json(const Array& values)
			:value_(make_shared<JsonArray>(values))
		{

		}

		Json(Array&& values)
			:value_(make_shared<JsonArray>(values))
		{

		}

		Json(const Object& values)
			:value_(make_shared<JsonObject>(values))
		{

		}

		Json(Object&& values)
			:value_(make_shared<JsonObject>(values))
		{

		}

		static std::pair<std::string, Json> Parse(const std::string& in, ParseModel model = NOCOMMENTS);

		std::pair<bool,int> IntValue() const
		{
			return value_->IntValue();
		}

		std::pair<bool, double> DoubleValue() const
		{
			return value_->DoubleValue();
		}

		std::pair<bool, bool> BoolValue() const
		{
			return value_->BoolValue();
		}

		std::pair<bool, std::string> StrValue() const
		{
			return value_->StrValue();
		}

		//Object ObjectValue() const 
		//{
		//	return value_->ObjectValue();
		//}

		//Array ArrayValue() const
		//{
		//	return value_->ArrayValue();
		//}

		std::pair<bool, const Json& > at(const std::string& key) const;

		std::pair<bool, const Json& > at(size_t i)const;

		std::string GetRawStr() const
		{	
			//if (value_->GetType()==Value::INT)
			//{
			//	std::cout <<value_->GetRawStr();
			//	//return value_->GetRawStr();
			//}
			return value_->GetRawStr();
		}

		Value& GetValue()const
		{
			return *value_;
		}

	private:
		//Type type_;
		shared_ptr<Value> value_;
	};
	
	class JsonParser
	{
	public:		
		JsonParser(const std::string& str, Json::ParseModel parse_model, std::string& err)
			:str_(str),
			parse_model_(parse_model),
			err_(err)
		{
			failed_ = false;
			i = 0;
		}

		Json Parse(int depth)
		{
			if (depth > max_depth) {
				return fail("exceeded maximum nesting depth");
			}

			char ch = GetNextToken();
			if (failed_)
				return Json();

			if (ch == '-' || (ch >= '0' && ch <= '9')) {
				i--;
				return ParseNumber();
			}

			if (ch == 't')
				return expect("true", true);

			if (ch == 'f')
				return expect("false", false);

			if (ch == 'n')
				return expect("null", Json());

			if (ch == '"')
				return ParseString();

			if (ch == '{') {
				Object data;
				ch = GetNextToken();
				if (ch == '}')
					return data;

				while (1) {
					if (ch != '"')
						//return fail("expected '\"' in object, got " + esc(ch));
						return fail("expected '\"' in object, got " + ch);
					std::string key = ParseString();
					if (failed_)
						return Json();

					ch = GetNextToken();
					if (ch != ':')
						//return fail("expected ':' in object, got " + esc(ch));
						return fail("expected ':' in object, got " + ch);

					data[std::move(key)] = Parse(depth + 1);
					if (failed_)
						return Json();

					ch = GetNextToken();
					if (ch == '}')
						break;
					if (ch != ',')
						//return fail("expected ',' in object, got " + esc(ch));
						return fail("expected ',' in object, got " + ch);

					ch = GetNextToken();
				}
				return data;
			}

			if (ch == '[') {
				Array data;
				ch = GetNextToken();
				if (ch == ']')
					return data;

				while (1) {
					i--;
					data.push_back(Parse(depth + 1));
					if (failed_)
						return Json();

					ch = GetNextToken();
					if (ch == ']')
						break;
					if (ch != ',')
						//return fail("expected ',' in list, got " + esc(ch));
						return fail("expected ',' in list, got " + ch);

					ch = GetNextToken();
					(void)ch;
				}
				return data;
			}

			//return fail("expected value, got " + esc(ch));
			return fail("expected value, got " + ch);
		}

	private:
		const Json::ParseModel parse_model_;
		bool failed_;
		const std::string& str_;
		std::string err_;
		size_t i;
		int max_depth = 100;

		Json fail(std::string&& msg) {
			return fail(move(msg), Json());
		}

		template <typename T>
		T fail(std::string&& msg, const T err_ret) {
			if (!failed_)
				err_ = std::move(msg);
			failed_ = true;
			return err_ret;
		}

		void ConsumeWhitespace()
		{
			while (str_[i] == ' ' || str_[i] == '\r' || str_[i] == '\n' || str_[i] == '\t')
				i++;
		}

		bool ConsumeComment()
		{
			bool comment_found = false;
			if (str_[i] == '/') {
				i++;
				if (i == str_.size())
					return fail("unexpected end of input after start of comment", false);
				if (str_[i] == '/') { // inline comment
					i++;
					// advance until next line, or end of input
					while (i < str_.size() && str_[i] != '\n') {
						i++;
					}
					comment_found = true;
				}
				else if (str_[i] == '*') { // multiline comment
					i++;
					if (i > str_.size() - 2)
						return fail("unexpected end of input inside multi-line comment", false);
					// advance until closing tokens
					while (!(str_[i] == '*' && str_[i + 1] == '/')) {
						i++;
						if (i > str_.size() - 2)
							return fail("unexpected end of input inside multi-line comment", false);
					}
					i += 2;
					comment_found = true;
				}
				else
					return fail("malformed comment", false);
			}
			return comment_found;
		}

		void ConsumeGarbage()
		{
			ConsumeWhitespace();
			if (parse_model_ == Json::ParseModel::COMMENTS) {
				bool comment_found = false;
				do {
					comment_found = ConsumeComment();
					if (failed_) return;
					ConsumeWhitespace();
				} while (comment_found);
			}
		}

		char GetNextToken()
		{
			ConsumeGarbage();
			if (failed_) return static_cast<char>(0);
			if (i == str_.size())
				return fail("unexpected end of input", static_cast<char>(0));
			return str_[i++];
		}

		std::string ParseString()
		{
			std::string out;

			if (i == str_.size())
				return fail("unexpected end of input in string", "");
			char ch = str_[i];
			//if (ch != '"')
			//{
			//	return fail("no \" in the head of a string", "");
			//}
			//i++;
			while (str_[i] != '"')
			{
				out += str_[i];
				i++;
			}
			i++;
			return out;
			/*long last_escaped_codepoint = -1;
			while (true) {
				if (i == str.size())
					return fail("unexpected end of input in string", "");

				char ch = str[i++];

				if (ch == '"') {
					encode_utf8(last_escaped_codepoint, out);
					return out;
				}

				if (in_range(ch, 0, 0x1f))
					return fail("unescaped " + esc(ch) + " in string", "");

				// The usual case: non-escaped characters
				if (ch != '\\') {
					encode_utf8(last_escaped_codepoint, out);
					last_escaped_codepoint = -1;
					out += ch;
					continue;
				}

				// Handle escapes
				if (i == str.size())
					return fail("unexpected end of input in string", "");

				ch = str[i++];

				if (ch == 'u') {
					// Extract 4-byte escape sequence
					string esc = str.substr(i, 4);
					// Explicitly check length of the substring. The following loop
					// relies on std::string returning the terminating NUL when
					// accessing str[length]. Checking here reduces brittleness.
					if (esc.length() < 4) {
						return fail("bad \\u escape: " + esc, "");
					}
					for (size_t j = 0; j < 4; j++) {
						if (!in_range(esc[j], 'a', 'f') && !in_range(esc[j], 'A', 'F')
							&& !in_range(esc[j], '0', '9'))
							return fail("bad \\u escape: " + esc, "");
					}

					long codepoint = strtol(esc.data(), nullptr, 16);

					// JSON specifies that characters outside the BMP shall be encoded as a pair
					// of 4-hex-digit \u escapes encoding their surrogate pair components. Check
					// whether we're in the middle of such a beast: the previous codepoint was an
					// escaped lead (high) surrogate, and this is a trail (low) surrogate.
					if (in_range(last_escaped_codepoint, 0xD800, 0xDBFF)
						&& in_range(codepoint, 0xDC00, 0xDFFF)) {
						// Reassemble the two surrogate pairs into one astral-plane character, per
						// the UTF-16 algorithm.
						encode_utf8((((last_escaped_codepoint - 0xD800) << 10)
							| (codepoint - 0xDC00)) + 0x10000, out);
						last_escaped_codepoint = -1;
					}
					else {
						encode_utf8(last_escaped_codepoint, out);
						last_escaped_codepoint = codepoint;
					}

					i += 4;
					continue;
				}

				encode_utf8(last_escaped_codepoint, out);
				last_escaped_codepoint = -1;

				if (ch == 'b') {
					out += '\b';
				}
				else if (ch == 'f') {
					out += '\f';
				}
				else if (ch == 'n') {
					out += '\n';
				}
				else if (ch == 'r') {
					out += '\r';
				}
				else if (ch == 't') {
					out += '\t';
				}
				else if (ch == '"' || ch == '\\' || ch == '/') {
					out += ch;
				}
				else {
					return fail("invalid escape character " + esc(ch), "");
				}
			}*/
		}

		Json ParseNumber()
		{
			size_t start_pos = i;

			if (str_[i] == '-')
				i++;

			// Integer part
			if (str_[i] == '0') {
				i++;
				if (in_range(str_[i], '0', '9'))
					return fail("leading 0s not permitted in numbers");
			}
			else if (in_range(str_[i], '1', '9')) {
				i++;
				while (in_range(str_[i], '0', '9'))
					i++;
			}
			else {
				//return fail("invalid " + esc(str[i]) + " in number");
				return fail("invalid " + std::string{ str_[i] } + "in number");
			}

			if (str_[i] != '.' && str_[i] != 'e' && str_[i] != 'E'
				&& (i - start_pos) <= static_cast<size_t>(std::numeric_limits<int>::digits10)) {
				return std::atoi(str_.c_str() + start_pos);
			}

			// Decimal part
			if (str_[i] == '.') {
				i++;
				if (!in_range(str_[i], '0', '9'))
					return fail("at least one digit required in fractional part");

				while (in_range(str_[i], '0', '9'))
					i++;
			}

			// Exponent part
			if (str_[i] == 'e' || str_[i] == 'E') {
				i++;

				if (str_[i] == '+' || str_[i] == '-')
					i++;

				if (!in_range(str_[i], '0', '9'))
					return fail("at least one digit required in exponent");

				while (in_range(str_[i], '0', '9'))
					i++;
			}

			return std::strtod(str_.c_str() + start_pos, nullptr);
		}
		
		Json expect(const std::string& expected, Json res) {
			if (i == 0)
			{
				return fail("expect wrong i(position of str_)", Json());
			}
			i--;
			if (str_.compare(i, expected.length(), expected) == 0) {
				i += expected.length();
				return res;
			}
			else {
				return fail("parse error: expected " + expected + ", got " + str_.substr(i, expected.length()));
			}
		}
	};

	class RawStr
	{
	public:
		//static std::string GetRawStr() {
		//	return "null";
		//}

		static std::string GetRawStr(double value) {
			std::string out;
			/*if (std::isfinite(value)) {
				char buf[32];
				snprintf(buf, sizeof buf, "%.17g", value);
				out += buf;
			}
			else {
				out += "null";
			}*/
			out += std::to_string(value);
			return out;
		}

		static std::string GetRawStr(int value) {
			std::string out;
			/*char buf[32];
			snprintf(buf, sizeof buf, "%d", value);
			out += buf;*/
			out += std::to_string(value);
			return out;
		}

		static std::string GetRawStr(bool value) {
			std::string out;
			out += value ? "true" : "false";
			return out;
		}

		static std::string GetRawStr(const std::string& value) {
			std::string out;
			out += '"';
			out += value;
			/*for (size_t i = 0; i < value.length(); i++) {
				const char ch = value[i];
				if (ch == '\\') {
					out += "\\\\";
				}
				else if (ch == '"') {
					out += "\\\"";
				}
				else if (ch == '\b') {
					out += "\\b";
				}
				else if (ch == '\f') {
					out += "\\f";
				}
				else if (ch == '\n') {
					out += "\\n";
				}
				else if (ch == '\r') {
					out += "\\r";
				}
				else if (ch == '\t') {
					out += "\\t";
				}
				else if (static_cast<uint8_t>(ch) <= 0x1f) {
					char buf[8];
					snprintf(buf, sizeof buf, "\\u%04x", ch);
					out += buf;
				}
				else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(value[i + 1]) == 0x80
					&& static_cast<uint8_t>(value[i + 2]) == 0xa8) {
					out += "\\u2028";
					i += 2;
				}
				else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(value[i + 1]) == 0x80
					&& static_cast<uint8_t>(value[i + 2]) == 0xa9) {
					out += "\\u2029";
					i += 2;
				}
				else {
					out += ch;
				}
			}*/
			out += '"';
			return out;
		}

		static std::string GetRawStr(std::string&& value) {
			std::string out;
			out += '"';
			out += value;
			/*for (size_t i = 0; i < value.length(); i++) {
				const char ch = value[i];
				if (ch == '\\') {
					out += "\\\\";
				}
				else if (ch == '"') {
					out += "\\\"";
				}
				else if (ch == '\b') {
					out += "\\b";
				}
				else if (ch == '\f') {
					out += "\\f";
				}
				else if (ch == '\n') {
					out += "\\n";
				}
				else if (ch == '\r') {
					out += "\\r";
				}
				else if (ch == '\t') {
					out += "\\t";
				}
				else if (static_cast<uint8_t>(ch) <= 0x1f) {
					char buf[8];
					snprintf(buf, sizeof buf, "\\u%04x", ch);
					out += buf;
				}
				else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(value[i + 1]) == 0x80
					&& static_cast<uint8_t>(value[i + 2]) == 0xa8) {
					out += "\\u2028";
					i += 2;
				}
				else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(value[i + 1]) == 0x80
					&& static_cast<uint8_t>(value[i + 2]) == 0xa9) {
					out += "\\u2029";
					i += 2;
				}
				else {
					out += ch;
				}
			}*/
			out += '"';
			return out;
		}

		static std::string GetRawStr(const Array& values) {
			std::string out;
			bool first = true;
			out += "[";
			for (const auto& value : values) {
				if (!first)
					out += ", ";
				out += value.GetRawStr();
				first = false;
			}
			out += "]";
			return out;
		}

		static std::string GetRawStr(Array&& values) {
			std::string out;
			bool first = true;
			out += "[";
			for (const auto& value : values) {
				if (!first)
					out += ", ";
				out += value.GetRawStr();
				first = false;
			}
			out += "]";
			return out;
		}

		static std::string GetRawStr(const Object& values) {
			std::string out;
			bool first = true;
			out += "{";
			for (const auto& kv : values) {
				if (!first)
					out += ", ";
				out += GetRawStr(kv.first);
				out += ": ";
				out += kv.second.GetRawStr();
				first = false;
			}
			out += "}";
			return out;
		}
	};
}