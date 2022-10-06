/*
    About: Some convenient functions for string operations. It depends on standard c++.
    
	Auther: Xu Shengpan (jack_1227x@163.com)
	Date: 2018/1/1
*/

#ifndef  STRING_UTILITY_H
#define  STRING_UTILITY_H

#include <string>
#include <sstream>
#include <map>
#include <vector>

template<typename T>
inline std::string get_string(T num)
{
    std::ostringstream strm;
    strm.setf(std::ios::fixed);
    strm << num;
    return strm.str();
}

template<typename T>
inline std::string get_string(T num, int width, char fillchar = '0')
{
    std::ostringstream strm;
    strm.width(width);
    strm.fill(fillchar);
    strm.setf(std::ios::fixed);
    strm << num;
    return strm.str();
}

template<typename T>
inline T get_number(const std::string& str)
{
    std::istringstream strm(str);
    T t;
    strm >> t;
    return t;
}

template <typename NumberType>
inline std::vector<std::string> sort_by_number(const std::vector<std::string>& strs)
{
    std::map<NumberType, std::string> i_str_map;
    for (auto str : strs) {
        NumberType i = get_number<NumberType>(str);
        i_str_map[i] = str;
    }
    std::vector<std::string> strs1(strs.size());
    int i = 0;
    for (auto iter = i_str_map.begin(); iter != i_str_map.end(); ++iter, ++i) {
        strs1[i] = iter->second;
    }
    return strs1;
}

inline bool get_boolean(const std::string& bool_str)
{
    if (bool_str == "true" || bool_str == "TRUE")
        return true;
    else
        return false;
}

inline std::string get_boolean_string(bool b)
{
    if (b)
        return "true";
    else
        return "false";
}

inline std::vector<std::string> string_split(const std::string &str, const char pattern = ',')
{
    std::vector<std::string> res;
    std::stringstream input(str);
    std::string temp;
    while (std::getline(input, temp, pattern)) {
        res.push_back(temp);
    }
    return res;
}

//Remove white space at both ends of the string.
inline void trim(std::string& str)
{
    if (!str.empty()) {
        str.erase(0, str.find_first_not_of(' '));
        str.erase(str.find_last_not_of(' ') + 1);
    }
}

inline std::string add_double_quot(const std::string& str)
{
    char ch = '\"';
    return ch + str + ch;
}

inline std::string remove_double_quot(const std::string& str)
{
    return std::string(str.begin() + 1, str.end() - 1);
}

inline std::string get_merged_string(const std::vector<std::string>& strs, char connect_char = ',')
{
    std::string str;
    for (int i = 0; i < strs.size(); ++i) {
        str += strs[i];
        if (i < strs.size() - 1)
            str += connect_char;
    }
    return str;
}

#endif