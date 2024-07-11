#pragma once
#include <string>
#include <vector>

namespace tnl {

	class CsvCell {
	public:
		CsvCell(const std::string& s) {
			str_ = s;
			int_ = std::atoi(s.c_str());
			float_ = (float)std::atof(s.c_str());
			bool_ = (s == "TRUE") ? true : false;
		}
		std::string& getString() { return str_; }
		int getInt() { return int_; }
		float getFloat() { return float_; }
		bool getBool() { return bool_; }
	private:
		std::string str_ = "";
		int int_ = 0;
		float float_ = 0;
		bool bool_ = false;
	};

	template<typename T>
	struct CsvTraits;
	template<>
	struct CsvTraits<int> {
		static constexpr auto Name = "int";
		static constexpr int Value = 0;
	};
	template<>
	struct CsvTraits<float> {
		static constexpr auto Name = "float";
		static constexpr int Value = 1;
	};
	template<>
	struct CsvTraits<std::string> {
		static constexpr auto Name = "string";
		static constexpr int Value = 2;
	};
	template<>
	struct CsvTraits<CsvCell> {
		static constexpr auto Name = "CsvCell";
		static constexpr int Value = 3;
	};

	//----------------------------------------------------------------------------------------------
	// Csv Loader
	// arg1... ファイルパス
	// ret.... テンプレート指定されたデータの２次元配列
	// tips... テンプレート指定できるのは以下の型
	// ....... int
	// ....... float
	// ....... std::string
	// ....... tnl::CsvCell
	// ....... テンプレート指定を省略した場合 tnl::CsvCell になります
	template< class T = CsvCell >
	std::vector<std::vector<T>> LoadCsv(const std::string& file_path){
		std::vector<std::vector<T>> ret;

		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "r");

		if (fp) {
			char buff[1024] = { 0 };
			while (fgets(buff, sizeof(buff), fp)) {
				std::string line = buff;
				std::vector<T> data;

				while (1) {
					size_t c = line.find(",");
					if (c == std::string::npos) {
						c = line.find("\n");
					}
					std::string s = line.substr(0, c);

					if constexpr (CsvTraits<int>::Value == CsvTraits<T>::Value) {
						data.emplace_back( std::atoi(s.c_str()) );
					}
					if constexpr (CsvTraits<float>::Value == CsvTraits<T>::Value) {
						data.emplace_back((float)std::atof(s.c_str()));
					}
					if constexpr (CsvTraits<std::string>::Value == CsvTraits<T>::Value) {
						data.emplace_back(std::move(s));
					}
					if constexpr (CsvTraits<CsvCell>::Value == CsvTraits<T>::Value) {
						data.emplace_back( CsvCell(s) );
					}

					line = line.substr(c + 1, line.length() - (c + 1));
					if (line.empty() || line == "/n") break;
				}
				ret.emplace_back(std::move(data));
				memset(buff, 0, sizeof(buff));
			}

			fclose(fp);
		}
		return ret;
	}

}

