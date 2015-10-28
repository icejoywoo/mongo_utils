#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdint.h>
#include <string>
#include <vector>

typedef struct _IPItem {
    uint64_t start;
    uint64_t end;
    std::string country;
} IPItem;

typedef std::vector<IPItem> IPTable;

static void split_string(const std::string& string_list, std::vector< std::string >& result, const char delimiter = ',') {
    const char* start = string_list.c_str();
    const char* p;
    for(p = start; *p; ++p)
    {
        if(*p == delimiter)
        {
            if(p != start)
            {
                result.push_back(std::string(start, p - start));
            }
            start = ++p;
        }
    }
    if(p != start)
    {
        result.push_back(std::string(start, p - start));
    }
}

static uint64_t ip2int(const std::string& ip) {
    std::vector<std::string> result;
    split_string(ip, result, '.');
    if (result.size() == 4 ) {
        uint64_t a = atoi(result[0].c_str());
        uint64_t b = atoi(result[1].c_str());
        uint64_t c = atoi(result[2].c_str());
        uint64_t d = atoi(result[3].c_str());
        return a * 16777216 + b * 65536 + c * 256 + d;
    } else {
        printf("%s\n", ip.c_str());
        return -1;
    }
}

class IPLib {
    public:
        IPLib() {}
        ~IPLib() {}
        int LoadDict(const std::string& dict) {
            std::ifstream fin(dict.c_str());
            std::string line;
            while (std::getline(fin, line)) {
                std::vector<std::string> fields;
                split_string(line, fields, '|');
                uint32_t start = ip2int(fields[0]);
                uint32_t end = ip2int(fields[1]);
                IPItem ip_item = {start, end, fields[2]};
                m_ip_table.push_back(ip_item);
            }
            fin.close();
            return 0;
        }

        int GetCountry(const std::string& ip, std::string& country) {
            uint64_t ip_int = ip2int(ip);
            size_t floor = 0;
            size_t ceil = m_ip_table.size() - 1;
            size_t mid = 0;
            while (floor <= ceil) {
                mid = (floor + ceil) / 2;
                if (m_ip_table[mid].start <= ip_int && ip_int <= m_ip_table[mid].end) {
                    country = m_ip_table[mid].country;
                    return 0;
                }

                if (ip_int < m_ip_table[mid].start) {
                    ceil = mid - 1;
                } else {
                    floor = mid + 1;
                }
            }
            return -1;
        }

        void CleanUp() {
            IPTable(m_ip_table).swap(m_ip_table);
        }

    private:
        IPTable m_ip_table;
};

