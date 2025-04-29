#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <curl/curl.h>

using namespace std;

// Callback for curl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Download HTML using libcurl
string downloadHTML(const string& url) {
    CURL* curl;
    CURLcode res;
    string html;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return html;
}

// Naive function to extract <table>...</table> blocks
vector<string> extractTables(const string& html) {
    vector<string> tables;
    size_t pos = 0;
    while ((pos = html.find("<table", pos)) != string::npos) {
        size_t end = html.find("</table>", pos);
        if (end == string::npos) break;
        end += 8; // length of </table>
        tables.push_back(html.substr(pos, end - pos));
        pos = end;
    }
    return tables;
}

void writeTableToCSV(const string& table, int index) {
    string filename = "table_" + to_string(index) + ".csv";
    ofstream file(filename);

    bool inRow = false;
    bool inCell = false;
    string cellData;

    for (size_t i = 0; i < table.size(); ++i) {
        if (table.compare(i, 4, "<tr>") == 0) {
            inRow = true;
            i += 3;
        } else if (table.compare(i, 5, "</tr>") == 0) {
            file << "\n";
            inRow = false;
            i += 4;
        } else if (inRow && (table.compare(i, 4, "<td>") == 0 || table.compare(i, 4, "<th>") == 0)) {
            inCell = true;
            cellData = "";
            i += 3;
        } else if (inCell && (table.compare(i, 5, "</td>") == 0 || table.compare(i, 5, "</th>") == 0)) {
            file << "\"" << cellData << "\",";
            inCell = false;
            i += 4;
        } else if (inCell) {
            cellData += table[i];
        }
    }

    file.close();
}

int main() {
    string url = "https://en.wikipedia.org/wiki/List_of_largest_companies_in_the_United_States_by_revenue";
    string html = downloadHTML(url);
    vector<string> tables = extractTables(html);

    for (size_t i = 0; i < tables.size(); ++i) {
        writeTableToCSV(tables[i], i + 1);
    }

    cout << "Extracted " << tables.size() << " tables." << endl;
    return 0;
}
