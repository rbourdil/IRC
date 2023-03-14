#ifndef DEBUG_HPP
#define DEBUG_HPP

const char	lookup[][5] = { "\\0", "\\SOH", "\\STX", "\\ETX", "\\EOT", "\\ENQ", \
							"\\ACK", "\\a", "\\b", "\\t", "\\n", "\\v", "\\f", "\\r", \
							"\\SO", "\\SI", "\\DLE", "\\DC1", "\\DC2", "\\DC3", "\\DC4", \
							"\\NAK", "\\SYN", "\\ETB", "\\CAN", "\\EM", "\\SUB", "\\ESC", \
							"\\FS", "\\GS", "\\RS", "\\US" };

std::string	get_raw(std::string s)
{
	std::string	raw;
	for (std::string::iterator it = s.begin(); it != s.end(); ++it)
	{
		if (*it < 32)
			raw.append(lookup[static_cast<size_t>(*it)]);
		else
			raw.push_back(*it);
	}
	return (raw);
}

#endif
