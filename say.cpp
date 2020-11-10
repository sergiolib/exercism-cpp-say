#include "say.h"
#include <algorithm>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

namespace say {
  vector<string> digitsTable = {"", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve"};
  vector<string> teenPrefixesTable = {"", "", "twen", "thir", "four",
    "fif", "six", "seven", "eigh", "nine"};
  vector<string> tyPrefixesTable = {"", "", "twen", "thir", "for",
    "fif", "six", "seven", "eigh", "nine"};

  string translateTenth(int n) {
    int first_digit = n / 10;
    int second_digit = n % 10;
    string s;

    if (n < 13) {
      return digitsTable[n];
    } else if (n < 20) { 
      s = teenPrefixesTable[second_digit];
      s.append("teen");
      return s;
    } else {
      s = tyPrefixesTable[first_digit];
      s.append("ty");
      if (second_digit > 0) {
	s.append("-");
      }
      s.append(digitsTable[second_digit]);
      return s;
    }
  }

  string translateHundreds(int n) {
    int hundred = n / 100;
    int tenths = n % 100;
    string s;

    if (hundred > 0) {
      s.append(digitsTable[hundred] + " hundred");
      if (tenths > 0) {
	s.append(" ");
      }
    }
    if (tenths > 0) {
      s.append(translateTenth(tenths));
    }
    return s;
  }

  int extractPartsPortionFromN(long n, string part) {
    if (part.compare(" billion") == 0) {
      return (n / 1000000000) % 1000;
    } else if (part.compare(" million") == 0) {
      return (n / 1000000) % 1000;
    } else if (part.compare(" thousand") == 0) {
      return (n / 1000) % 1000;
    } else {
      return n % 1000;
    }
    throw invalid_argument(part + " was a non understood part of the number");
  }

  string translatePart(long n, string part) {
    string s;
    int part_n = extractPartsPortionFromN(n, part);

    if (part_n > 0) {
      s = translateHundreds(part_n) + part;
    }
    return s;
  }

  string partsToString(vector<string> parts) {
    stringstream s;
    string res;

    // Concatenate parts with spaces in between
    copy(parts.begin(), parts.end(),
	 ostream_iterator<string>(s, string(" ").c_str()));
    res = s.str();
    // One trailing whitespace needs to be eliminated from the end of the string
    return res.substr(0, res.size() - 1);
  }

  vector<string> extractParts(long n) {
    vector<string> parts;
    vector<string> partNames = {" billion", " million", " thousand", ""};
    string segment;

    for (auto part: partNames) {
      segment = translatePart(n, part);
      if (segment.length() > 0) {
	parts.push_back(segment);
      }
    }
    return parts;
  }

  string in_english(long n) {
    string s;
    vector<string> parts;

    if (n < 0) { // negative numbers
      throw domain_error("n must be 0 or bigger");
    } else if (n == 0) { // number == 0
      s = "zero";
    } else if (n < 1000L * 1000L * 1000L * 1000L) { // number less than a trillion
      parts = extractParts(n);
      s = partsToString(parts);
    } else { // number bigger or equal to a trillion
      throw domain_error("n must be less than one trillion");
    }
    return s;
  }
}  // namespace say
